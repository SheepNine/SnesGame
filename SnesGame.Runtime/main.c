#include "SDL.h"
#include <stdio.h>
#include "bb.h"
#include "mapper.h"
#include "ppu.h"
#include "perf.h"
#include "sc.h"
#include "joydriver.h"
#include "gamepad.h"

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

SDL_AudioSpec have;

hSC sc;

// Recorded state for current half-wave
int avHalfwaveTimer = 0;

void AnAudioCallback(void* userdata, Uint8* stream, int len) {
	Sint16* writePtr = (Sint16*)stream;
	for (int i = 0; i < have.samples; i++) {
		writePtr[i] = getNextSample_SC(sc);
	}
}

int main(int argc, char** argv) {
	int result = 0;
	bb = creat_BB();
	fill_BB(bb, 128, 128, 128);
	bgMapper = creat_Mapper(1);
	spriteMapper = creat_Mapper(2);
	ppu = creat_PPU(bgMapper, spriteMapper);
	sc = creat_SC();

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

	setSpriteControl_PPU(ppu,  8,  8 + 64,  8 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu,  9, 16 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 10,  8 + 64, 24 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 11,  0 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 12, 72 + 64,  8 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 13, 80 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 14, 72 + 64, 24 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 15, 64 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 16, 32 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 17, 48 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 18, 24 + 64,  0 + 64, 1, 1, SDL_TRUE, 0);
	setSpriteControl_PPU(ppu, 19, 56 + 64,  0 + 64, 1, 1, SDL_TRUE, 0);

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
			return 1;
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
					// controller state
					setSpriteBrush_PPU(ppu,  8, isEngaged_GP(gp, GP_BUTTON_DU) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu,  9, isEngaged_GP(gp, GP_BUTTON_DR) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 10, isEngaged_GP(gp, GP_BUTTON_DD) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 11, isEngaged_GP(gp, GP_BUTTON_DL) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 12, isEngaged_GP(gp, GP_BUTTON_BX) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 13, isEngaged_GP(gp, GP_BUTTON_BA) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 14, isEngaged_GP(gp, GP_BUTTON_BB) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 15, isEngaged_GP(gp, GP_BUTTON_BY) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 16, isEngaged_GP(gp, GP_BUTTON_SL) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 17, isEngaged_GP(gp, GP_BUTTON_ST) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 18, isEngaged_GP(gp, GP_BUTTON_ZL) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
					setSpriteBrush_PPU(ppu, 19, isEngaged_GP(gp, GP_BUTTON_ZR) == SDL_TRUE ? 66 : 50, 3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);

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
	destr_Mapper(bgMapper);
	destr_Mapper(spriteMapper);
	destr_BB(bb);
	return result;
}
