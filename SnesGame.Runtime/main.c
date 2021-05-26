#include "SDL.h"
#include <stdio.h>
#include "bb.h"
#include "mapper.h"
#include "ppu.h"
#include "perf.h"
#include "sc.h"
#include "joydriver.h"
#include "gamepad.h"
#include "api/snes_api.h"
#include "init.h"
#include "render.h"

// --- BackBuffer ---

hBB bb;
hMapper romMapper;
hPPU ppu;

Uint32 heartbeatCallback(Uint32 interval, void* param) {
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
hSC sc;

void AnAudioCallback(void* userdata, Uint8* stream, int len) {
	Sint16* writePtr = (Sint16*)stream;
	for (int i = 0; i < have.samples; i++) {
		writePtr[i] = getNextSample_SC(sc);
	}
}

extern int libMain(char* title, pInitCallback initFunc, pUpdateCallback updateFunc, pRenderCallback renderFunc) {
	int result = 0;
	bb = creat_BB();
	fill_BB(bb, 128, 128, 128);
	romMapper = creat_Mapper(1);
	ppu = creat_PPU(romMapper);
	sc = creat_SC();

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
		want.samples = 4096;
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
			int x = 0;
			int y = 0;

			hJD jd = NULL;
			hGP gp = creat_GP();

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
					if (event.key.keysym.sym == SDLK_F11) {
						if (SDL_LogGetPriority(SDL_LOG_CATEGORY_CUSTOM) == SDL_LOG_PRIORITY_WARN) {
							SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_DEBUG);
						}
						else{
							SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_WARN);
						}
					}
					if (event.key.keysym.sym == SDLK_SPACE) {
						silence_SC(sc);
					}
					if (event.key.keysym.sym == SDLK_q) {
						playNote_SC(sc, 65535,
							8, SD_NONE, 0, EB_SILENCE,
							250, 250, SD_NONE, 0, EB_SILENCE);
					}
					if (event.key.keysym.sym == SDLK_a) {
						playNote_SC(sc, 65535,
							8, SD_NONE, 0, EB_SILENCE,
							100, 250, SD_RISING, 1, EB_SILENCE);
					}
					if (event.key.keysym.sym == SDLK_s) {
					}
					if (event.key.keysym.sym == SDLK_d) {
					}
					if (event.key.keysym.sym == SDLK_f) {
					}

					if (event.key.keysym.sym == SDLK_z) {
					}
					if (event.key.keysym.sym == SDLK_x) {
					}
					if (event.key.keysym.sym == SDLK_c) {
					}
					if (event.key.keysym.sym == SDLK_v) {
					}
					break;
				case SDL_USEREVENT:
					updateFunc(NULL);
					{
						hRENDER render = creat_RENDER(ppu);
						renderFunc(render);
						destr_RENDER(render);
					}
					// controller state
					setActorStroke_PPU(ppu,  8, isEngaged_GP(gp, GP_BUTTON_DU) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu,  9, isEngaged_GP(gp, GP_BUTTON_DR) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 10, isEngaged_GP(gp, GP_BUTTON_DD) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 11, isEngaged_GP(gp, GP_BUTTON_DL) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 12, isEngaged_GP(gp, GP_BUTTON_BX) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 13, isEngaged_GP(gp, GP_BUTTON_BA) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 14, isEngaged_GP(gp, GP_BUTTON_BB) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 15, isEngaged_GP(gp, GP_BUTTON_BY) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 16, isEngaged_GP(gp, GP_BUTTON_SL) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 17, isEngaged_GP(gp, GP_BUTTON_ST) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 18, isEngaged_GP(gp, GP_BUTTON_ZL) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setActorStroke_PPU(ppu, 19, isEngaged_GP(gp, GP_BUTTON_ZR) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					hPerf perf = creat_Perf(SDL_LOG_CATEGORY_CUSTOM);
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

	destr_SC(sc);
	destr_PPU(ppu);
	destr_Mapper(romMapper);
	destr_BB(bb);
	return result;
}
