#include "SDL.h"
#include <stdio.h>
#include "bb.h"
#include "mapper.h"
#include "ppu.h"
#include "perf.h"
#include "sw.h"

// --- BackBuffer ---

hBB bb;
hMapper bgMapper;
hMapper spriteMapper;
hPPU ppu;

void readGlyphList(char *filename, Uint8 *glyphlist) {
	SDL_memset(glyphlist, 0, 8192);
	SDL_RWops* rw = SDL_RWFromFile(filename, "r");
	if (rw != NULL) {
		SDL_RWread(rw, glyphlist, 8192, 1);
		SDL_RWclose(rw);
	}
}

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

void handleJoyEvent(SDL_Event* event) {
	switch (event->type) {
	case SDL_JOYDEVICEADDED:
		SDL_JoystickID joyId = SDL_JoystickInstanceID(SDL_JoystickOpen(event->jdevice.which));
		SDL_Log("Added Joystick (device index:%i, instance id %i)", event->jdevice.which, joyId);
		break;
	case SDL_JOYDEVICEREMOVED:
		SDL_Log("Removed Joystick (instance ID:%i)", event->jdevice.which);
		SDL_JoystickClose(SDL_JoystickFromInstanceID(event->jdevice.which));
		break;
	case SDL_JOYBUTTONDOWN:
		//SDL_Log("Joy button down (button: %u, instance ID: %i)", event->jbutton.button, event->jbutton.which);
		switch (event->jbutton.button) {
		case 0:
			SDL_Log("+X");
			break;
		case 1:
			SDL_Log("+A");
			break;
		case 2:
			SDL_Log("+B");
			break;
		case 3:
			SDL_Log("+Y");
			break;
		case 4:
			SDL_Log("+ZL");
			break;
		case 5:
			SDL_Log("+ZR");
			break;
		case 8:
			SDL_Log("+SELECT");
			break;
		case 9:
			SDL_Log("+START");
			break;
		}
		break;
	case SDL_JOYBUTTONUP:
		//SDL_Log("Joy button up (button: %u, instance ID: %i)", event->jbutton.button, event->jbutton.which);
		switch (event->jbutton.button) {
		case 0:
			SDL_Log("-X");
			break;
		case 1:
			SDL_Log("-A");
			break;
		case 2:
			SDL_Log("-B");
			break;
		case 3:
			SDL_Log("-Y");
			break;
		case 4:
			SDL_Log("-ZL");
			break;
		case 5:
			SDL_Log("-ZR");
			break;
		case 8:
			SDL_Log("-SELECT");
			break;
		case 9:
			SDL_Log("-START");
			break;
		}
		break;
	case SDL_JOYAXISMOTION:
		//SDL_Log("Joy axis motion (axis: %u, value: %i, instance ID: %i)", event->jaxis.axis, event->jaxis.value, event->jaxis.which);
		switch (event->jaxis.axis) {
		case 0:
			switch (event->jaxis.value) {
			case -32768:
				SDL_Log("+L -R");
				break;
			case -256:
				SDL_Log("-L -R");
				break;
			case 32767:
				SDL_Log("-L +R");
				break;
			}
			break;
		case 4:
			switch (event->jaxis.value) {
			case -32768:
				SDL_Log("+U -D");
				break;
			case -256:
				SDL_Log("-U -D");
				break;
			case 32767:
				SDL_Log("-U +D");
				break;
			}
			break;
		}
		break;
	}
}

SDL_AudioSpec have;

SDL_bool halfPeriod = 250; // 0 => silent
Sint8 volume = 8;
SDL_bool lowerWave = SDL_FALSE;
hSW sw;

// Recorded state for current half-wave
int avHalfwaveTimer = 0;

void AnAudioCallback(void* userdata, Uint8* stream, int len) {
	Sint16* writePtr = (Sint16*)stream;
	for (int i = 0; i < have.samples; i++) {
		if (avHalfwaveTimer == 0) {
			if (halfPeriod == 0) {
				if (!isSilent_SW(sw)) {
					setTargetVolume_SW(sw, 0);
					avHalfwaveTimer = 16;
				}
			}
			else {
				lowerWave = lowerWave ? SDL_FALSE : SDL_TRUE;
				avHalfwaveTimer = halfPeriod;
				setTargetVolume_SW(sw, lowerWave ? -volume : volume);
			}
		}
		else {
			avHalfwaveTimer -= 1;
		}

		writePtr[i] = getNextSample_SW(sw);
	}
}

int main(int argc, char** argv) {
	int result = 0;
	bb = creat_BB();
	fill_BB(bb, 128, 128, 128);
	bgMapper = creat_Mapper(1);
	spriteMapper = creat_Mapper(2);
	ppu = creat_PPU(bgMapper, spriteMapper);
	sw = creat_SW();

	Uint8 glyphList[8192];
	readGlyphList("..\\resources\\default.glyphset", glyphList);
	loadPage_Mapper(bgMapper, 0, glyphList);
	loadPage_Mapper(spriteMapper, 1, glyphList);

	switchSpriteBank_PPU(ppu, 3, 1);

	setBgColor_PPU(ppu, 0, 0x0, 0x00, 0x00, 0x00, SDL_FALSE); // Black
	setBgColor_PPU(ppu, 0, 0x1, 0x00, 0x00, 0xAA, SDL_FALSE); // Blue
	setBgColor_PPU(ppu, 0, 0x2, 0x00, 0xAA, 0x00, SDL_FALSE); // Green
	setBgColor_PPU(ppu, 0, 0x3, 0x00, 0xAA, 0xAA, SDL_FALSE); // Cyan
	setBgColor_PPU(ppu, 0, 0x4, 0xAA, 0x00, 0x00, SDL_FALSE); // Red
	setBgColor_PPU(ppu, 0, 0x5, 0xAA, 0x00, 0xAA, SDL_FALSE); // Magenta
	setBgColor_PPU(ppu, 0, 0x6, 0xAA, 0x55, 0x00, SDL_FALSE); // Brown
	setBgColor_PPU(ppu, 0, 0x7, 0xAA, 0xAA, 0xAA, SDL_FALSE); // Light gray
	setBgColor_PPU(ppu, 0, 0x8, 0x55, 0x55, 0x55, SDL_FALSE); // Dark gray
	setBgColor_PPU(ppu, 0, 0x9, 0x55, 0x55, 0xFF, SDL_FALSE); // Light blue
	setBgColor_PPU(ppu, 0, 0xA, 0x55, 0xFF, 0x55, SDL_FALSE); // Light green
	setBgColor_PPU(ppu, 0, 0xB, 0x55, 0xFF, 0xFF, SDL_FALSE); // Light cyan
	setBgColor_PPU(ppu, 0, 0xC, 0xFF, 0x55, 0x55, SDL_FALSE); // Light red
	setBgColor_PPU(ppu, 0, 0xD, 0xFF, 0x55, 0xFF, SDL_FALSE); // Light magenta
	setBgColor_PPU(ppu, 0, 0xE, 0xFF, 0xFF, 0x55, SDL_FALSE); // Yellow
	setBgColor_PPU(ppu, 0, 0xF, 0xFF, 0xFF, 0xFF, SDL_FALSE); // White

	setSpriteColor_PPU(ppu, 7, 0x0, 0x00, 0x00, 0x00, SDL_FALSE); // Black
	setSpriteColor_PPU(ppu, 7, 0x1, 0x00, 0x00, 0xAA, SDL_FALSE); // Blue
	setSpriteColor_PPU(ppu, 7, 0x2, 0x00, 0xAA, 0x00, SDL_FALSE); // Green
	setSpriteColor_PPU(ppu, 7, 0x3, 0x00, 0xAA, 0xAA, SDL_FALSE); // Cyan
	setSpriteColor_PPU(ppu, 7, 0x4, 0xAA, 0x00, 0x00, SDL_FALSE); // Red
	setSpriteColor_PPU(ppu, 7, 0x5, 0xAA, 0x00, 0xAA, SDL_FALSE); // Magenta
	setSpriteColor_PPU(ppu, 7, 0x6, 0xAA, 0x55, 0x00, SDL_FALSE); // Brown
	setSpriteColor_PPU(ppu, 7, 0x7, 0xAA, 0xAA, 0xAA, SDL_FALSE); // Light gray
	setSpriteColor_PPU(ppu, 7, 0x8, 0x55, 0x55, 0x55, SDL_FALSE); // Dark gray
	setSpriteColor_PPU(ppu, 7, 0x9, 0x55, 0x55, 0xFF, SDL_FALSE); // Light blue
	setSpriteColor_PPU(ppu, 7, 0xA, 0x55, 0xFF, 0x55, SDL_FALSE); // Light green
	setSpriteColor_PPU(ppu, 7, 0xB, 0x55, 0xFF, 0xFF, SDL_FALSE); // Light cyan
	setSpriteColor_PPU(ppu, 7, 0xC, 0xFF, 0x55, 0x55, SDL_FALSE); // Light red
	setSpriteColor_PPU(ppu, 7, 0xD, 0xFF, 0x55, 0xFF, SDL_FALSE); // Light magenta
	setSpriteColor_PPU(ppu, 7, 0xE, 0xFF, 0xFF, 0x55, SDL_FALSE); // Yellow
	setSpriteColor_PPU(ppu, 7, 0xF, 0xFF, 0xFF, 0xFF, SDL_FALSE); // White

	setBgControl_PPU(ppu, 0, 0, 0, SDL_TRUE, SDL_FALSE);

	setSpriteControl_PPU(ppu, 0, 4, 9, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 0, 1, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setSpriteControl_PPU(ppu, 1, 25, 11, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 1, 1, 3, 7, SDL_TRUE, SDL_FALSE, SDL_FALSE);
	setSpriteControl_PPU(ppu, 2, 42, 14, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 2, 1, 3, 7, SDL_TRUE, SDL_TRUE, SDL_TRUE);
	setSpriteControl_PPU(ppu, 3, 60, 20, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 3, 1, 3, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setSpriteControl_PPU(ppu, 4,  -5,  -5, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 4, 1, 3, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setSpriteControl_PPU(ppu, 5,  -5, 243, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 5, 1, 3, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setSpriteControl_PPU(ppu, 6, 243, 243, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 6, 1, 3, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setSpriteControl_PPU(ppu, 7, 243,  -5, 2, 2, SDL_TRUE, 0); setSpriteBrush_PPU(ppu, 7, 1, 3, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);

	setLayerClips_PPU(ppu, 0, 1, 3, 4, 9);

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			int glyph;
			if (y > 28) {
				glyph = 49;
			}
			else if (y == 28) {
				glyph = 34;
			}
			else {
				glyph = 16;
			}

			setBgBrush_PPU(ppu, 0, x, y, glyph, 0, 0, x % 4 == 1, x % 4 == 3, SDL_TRUE);
		}
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
			return;
		}

		SDL_Window* window = SDL_CreateWindow(
			"SnesGame",
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

			SDL_bool loop = SDL_TRUE;
			SDL_Event event;
			while (loop) {
				SDL_WaitEvent(&event);
				switch (event.type) {
				case SDL_QUIT:
					loop = SDL_FALSE;
					break;
				case SDL_JOYDEVICEADDED:
				case SDL_JOYDEVICEREMOVED:
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
				case SDL_JOYAXISMOTION:
					handleJoyEvent(&event);
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
						halfPeriod = 0;
					}
					if (event.key.keysym.sym == SDLK_q) { halfPeriod = 20; volume = 8; }
					if (event.key.keysym.sym == SDLK_a) { halfPeriod = 110; volume = 8; }
					if (event.key.keysym.sym == SDLK_s) { halfPeriod = 150; volume = 8; }
					if (event.key.keysym.sym == SDLK_d) { halfPeriod = 200; volume = 8; }
					if (event.key.keysym.sym == SDLK_f) { halfPeriod = 250; volume = 8; }

					if (event.key.keysym.sym == SDLK_z) { halfPeriod = 250; volume = 6; }
					if (event.key.keysym.sym == SDLK_x) { halfPeriod = 250; volume = 4; }
					if (event.key.keysym.sym == SDLK_c) { halfPeriod = 250; volume = 2; }
					if (event.key.keysym.sym == SDLK_v) { halfPeriod = 250; volume = 1; }
					break;
				case SDL_USEREVENT:
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

	destr_SW(sw);
	destr_PPU(ppu);
	destr_Mapper(bgMapper);
	destr_Mapper(spriteMapper);
	destr_BB(bb);
	return result;
}
