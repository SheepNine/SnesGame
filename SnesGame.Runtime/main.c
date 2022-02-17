#include "SDL.h"
#include <stdio.h>
#include "bb.h"
#include "mapper.h"
#include "ppu.h"
#include "perf.h"
#include "sc.h"
#include "joydriver.h"
#include "keydriver.h"
#include "gamepad.h"
#include "api/snes_runtime.h"
#include "init.h"
#include "render.h"
#include "update.h"
#include "dr.h"
#include "dp.h"

// --- BackBuffer ---

hBB bb;
hMapper romMapper;
hPPU ppu;

ChannelVoice voiceBuffer[64];
int voiceBufferHead = 0;
int samplesInCurrentVoiceSet = 0;


Uint32 heartbeatCallback(Uint32 interval, void* __) {
	__;
	SDL_UserEvent userEvent;
	userEvent.type = SDL_USEREVENT;
	userEvent.code = 0;
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user = userEvent;
	SDL_PushEvent(&event);
	return interval;
}

#define R_WIDTH 248
#define R_HEIGHT 247

SDL_AudioSpec have;
hSC soundChannels[8];

void AnAudioCallback(void* __1, Uint8* stream, int __2) {
	__1; __2;
	Sint16* writePtr = (Sint16*)stream;

	// If the audio playback is more than a whole sample ahead, just drop back
	while (samplesInCurrentVoiceSet < -512) {
		samplesInCurrentVoiceSet += 512;
	}

	for (int sampleIndex = 0; sampleIndex < have.samples; sampleIndex++) {
		while (samplesInCurrentVoiceSet <= 0 && voiceBufferHead > 0) {
			for (int i = 0; i < 8; i++) {
				switch (voiceBuffer[i].type) {
				case CV_SILENCE:
					silence_SC(soundChannels[i]);
					break;
				case CV_SQUARE: {
					SquareWaveParams params = voiceBuffer[i].waveParams.square;
					playNote_SC(soundChannels[i], params.length,
						params.volumeLow, params.volumeHigh,
						params.volumeShift.dir, params.volumeShift.speed, params.volumeShift.edgeBehaviour,
						params.periodLow, params.periodHigh,
						params.periodShift.dir, params.periodShift.speed, params.periodShift.edgeBehaviour);
					break;
				}
				case CV_NOISE: {
					NoiseParams params = voiceBuffer[i].waveParams.noise;
					playNoise_SC(soundChannels[i], params.initialRegister, params.tapBit, params.maxLength, params.period);
					break;
				}
				}
			}

			for (int i = 0; i < voiceBufferHead - 8; i++) { // SKETCHY: not copying all of them
				voiceBuffer[i] = voiceBuffer[i + 8];
			}

			voiceBufferHead -= 8;
			samplesInCurrentVoiceSet += 960;
		}

		writePtr[sampleIndex] = 0;
		for (int c = 0; c < 8; c++)
			writePtr[sampleIndex] += getNextSample_SC(soundChannels[c]);

		samplesInCurrentVoiceSet--;
	}
}

extern int libMain(char* title, pInitCallback initFunc, pUpdateCallback updateFunc, pRenderCallback renderFunc, pDeinitCallback deinitFunc, SDL_RWops* recordStream, SDL_RWops* playbackStream) {
	int result = 0;
	bb = creat_BB();
	romMapper = creat_Mapper(1);
	ppu = creat_PPU(romMapper);
	for (int i = 0; i < 8; i++)
		soundChannels[i] = creat_SC();

	{
		hINIT init = creat_INIT(romMapper);
		initFunc(init);
		destr_INIT(init);
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) == 0) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_WARN);

		SDL_AudioDeviceID dev;
		SDL_AudioSpec want;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = 48000;
		want.format = AUDIO_S16;
		want.channels = 1;
		want.samples = 512;
		want.callback = AnAudioCallback;
		dev = SDL_OpenAudioDevice(NULL, SDL_FALSE, &want, &have, 0);

		if (dev == 0) {
			return 1;
		}

		SDL_Window* window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			770, 770,
			SDL_WINDOW_RESIZABLE);
		if (window != NULL) {
			// Error check the lock/unlock/update
			// Check the window surface format just in case it isn't 8bpp ARGB

			SDL_PauseAudioDevice(dev, 0);
			SDL_AddTimer(20, heartbeatCallback, NULL);

			Uint64 frameCounter = 0;
			hJD jd = NULL;
			hGP gp = creat_GP();
			hKD kd = creat_KD(gp);
			hDR dr = creat_DR(gp, recordStream);
			hDP dp = creat_DP(gp, playbackStream);

			SDL_bool loop = SDL_TRUE;
			SDL_Event event;
			while (loop) {
				SDL_WaitEvent(&event);
				switch (event.type) {
				case SDL_QUIT:
					loop = SDL_FALSE;
					break;
				case SDL_JOYDEVICEADDED:
					if (jd == NULL) {
						jd = creat_JD(event.jdevice.which, gp);
					}
					break;
				case SDL_JOYDEVICEREMOVED:
					if (jd != NULL) {
						jd = handleDeviceRemoved_JD(jd, &event.jdevice);
					}
					// if JD == NULL, find a new joystick?
					break;
				case SDL_JOYBUTTONDOWN:
					handleJoyButtonDown_JD(jd, &event.jbutton);
					break;
				case SDL_JOYBUTTONUP:
					handleJoyButtonUp_JD(jd, &event.jbutton);
					break;
				case SDL_JOYAXISMOTION:
					handleJoyAxisMotion_JD(jd, &event.jaxis);
					break;
				case SDL_JOYHATMOTION:
					handleJoyHatMotion_JD(jd, &event.jhat);
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						loop = SDL_FALSE;
					}
					else if (event.key.keysym.sym == SDLK_F11) {
						if (SDL_LogGetPriority(SDL_LOG_CATEGORY_CUSTOM) == SDL_LOG_PRIORITY_WARN) {
							SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_DEBUG);
						}
						else{
							SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_WARN);
						}
					}
					else if (event.key.keysym.sym == SDLK_F1) { debugToggleBackdrop_PPU(ppu, 0); }
					else if (event.key.keysym.sym == SDLK_F2) { debugToggleBackdrop_PPU(ppu, 1); }
					else if (event.key.keysym.sym == SDLK_F3) { debugToggleBackdrop_PPU(ppu, 2); }
					else if (event.key.keysym.sym == SDLK_F4) { debugToggleBackdrop_PPU(ppu, 3); }
					else if (event.key.keysym.sym == SDLK_F5) { debugToggleActor_PPU(ppu, 0); }
					else if (event.key.keysym.sym == SDLK_F6) { debugToggleActor_PPU(ppu, 1); }
					else if (event.key.keysym.sym == SDLK_F7) { debugToggleActor_PPU(ppu, 2); }
					else if (event.key.keysym.sym == SDLK_F8) { debugToggleActor_PPU(ppu, 3); }
					else if (event.key.keysym.sym == SDLK_F9) { toggleCrtMode_BB(bb); }
					else {
						handleKeyDown_KD(kd, &event.key);
					}
					break;
				case SDL_KEYUP:
					handleKeyUp_KD(kd, &event.key);
					break;
				case SDL_USEREVENT:
					{
						ChannelVoice voices[8];
						for (int i = 0; i < 8; i++) {
							voices[i].type = CV_SUSTAIN;
						}
						playback_DP(dp);
						record_DR(dr);
						hUPDATE update = creat_UPDATE(gp, voices, frameCounter++);
						updateFunc(update);
						destr_UPDATE(update);
						advanceFrame_GP(gp);

						{
							SDL_LockAudioDevice(dev);
							if (voiceBufferHead < 64) {
								ChannelVoice* writePtr = voiceBuffer + voiceBufferHead;
								for (int i = 0; i < 8; i++) {
									writePtr[i] = voices[i];
								}
								voiceBufferHead += 8;
							}
							SDL_UnlockAudioDevice(dev);
						}
					}
					{
						hRENDER render = creat_RENDER(ppu);
						renderFunc(render);
						destr_RENDER(render);
					}
					hPerf perf = creat_Perf(SDL_LOG_CATEGORY_CUSTOM);
					fill_BB(bb, 0x9B, 0x6A, 0x12);
					scan_PPU(ppu, bb);
					logInterval_Perf(perf, "Scan");
					SDL_Surface* surface = SDL_GetWindowSurface(window);
					if (surface != NULL) {
						SDL_LockSurface(surface);
						logInterval_Perf(perf, "Pre-blit");
						blit_BB(bb, surface);
						logInterval_Perf(perf, "Blit");
						SDL_UnlockSurface(surface);
						SDL_UpdateWindowSurface(window);
					}
					logInterval_Perf(perf, "Post-blit");
					logTarget_Perf(perf, "update and render", 18000);
					destr_Perf(perf);
					break;
				}
			}

			if (jd != NULL) {
				destr_JD(jd);
			}
			if (kd != NULL) {
				destr_KD(kd);
			}
			destr_DR(dr);
			destr_DP(dp);
			destr_GP(gp);

			SDL_CloseAudioDevice(dev);
			SDL_DestroyWindow(window);
		} else {
			fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
			result = 2;
		}
		SDL_Quit();
	} else {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		result = 1;
	}

	for (int i = 0; i < 8; i++)
		destr_SC(soundChannels[i]);
	destr_PPU(ppu);
	destr_Mapper(romMapper);
	destr_BB(bb);

	deinitFunc();
	return result;
}
