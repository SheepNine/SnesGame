#include "SDL.h"
#include <stdio.h>
#include "bb.h"
#include "mapper.h"
#include "ppu.h"
#include "perf.h"

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

SDL_bool silent = SDL_TRUE;
SDL_bool newHalfPeriod = 250;

Sint16 stepDeltas[17][16] = {
   {     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,      0,      0,      0,      0 },
   {     6,   -13,    16,   -20,    26,   -36,    60,  -256,  -256,    60,   -36,    26,    -20,     16,    -13,      6 },
   {    13,   -27,    32,   -39,    50,   -71,   120,  -512,  -512,   120,   -71,    50,    -39,     32,    -27,     13 },
   {    19,   -40,    48,   -59,    76,  -107,   180,  -768,  -768,   180,  -107,    76,    -59,     48,    -40,     19 },
   {    26,   -55,    64,   -78,   101,  -142,   240, -1024, -1024,   240,  -142,   101,    -78,     64,    -55,     26 },
   {    32,   -68,    80,   -98,   127,  -178,   300, -1280, -1280,   300,  -178,   127,    -98,     80,    -68,     32 },
   {    38,   -81,    96,  -117,   151,  -213,   360, -1536, -1536,   360,  -213,   151,   -117,     96,    -81,     38 },
   {    45,   -95,   112,  -137,   177,  -249,   420, -1792, -1792,   420,  -249,   177,   -137,    112,    -95,     45 },
   {    51,  -108,   128,  -157,   202,  -284,   480, -2048, -2048,   480,  -284,   202,   -157,    128,   -108,     51 },
   {    58,  -122,   144,  -177,   228,  -320,   540, -2304, -2304,   540,  -320,   228,   -177,    144,   -122,     58 },
   {    64,  -136,   161,  -196,   252,  -355,   600, -2560, -2560,   600,  -355,   252,   -196,    161,   -136,     64 },
   {    70,  -149,   177,  -216,   278,  -391,   660, -2816, -2816,   660,  -391,   278,   -216,    177,   -149,     70 },
   {    77,  -163,   192,  -235,   303,  -426,   720, -3072, -3072,   720,  -426,   303,   -235,    192,   -163,     77 },
   {    83,  -176,   208,  -255,   329,  -462,   780, -3328, -3328,   780,  -462,   329,   -255,    208,   -176,     83 },
   {    90,  -190,   224,  -274,   353,  -496,   839, -3584, -3584,   839,  -496,   353,   -274,    224,   -190,     90 },
   {    96,  -203,   240,  -294,   379,  -532,   899, -3840, -3840,   899,  -532,   379,   -294,    240,   -203,     96 },
   {   102,  -217,   257,  -314,   404,  -567,   958, -4095, -4095,   958,  -567,   404,   -314,    257,   -217,    102 }
};

Sint16 output = 0;
Sint16 stepDelta[16] = { -742, 1606, -1900, 2324, -2990, 4204, -7106, 30340, 30340, -7106, 4204, -2990, 2324, -1900, 1606, -742 };

// Recorded state for current half-wave
int avPos = 0;
int avHalfPeriod = 0;
Sint8 avPrevStepVolume = 0;
Sint8 avCurrStepVolume = 0;

void AnAudioCallback(void* userdata, Uint8* stream, int len) {
	Sint16* writePtr = (Sint16*)stream;
	for (int i = 0; i < have.samples; i++) {
		if (avPos == avHalfPeriod) {
			avPos = 0;
			avPrevStepVolume = avCurrStepVolume;

			if (avCurrStepVolume == 0) {
				if (silent == SDL_FALSE) {
					avHalfPeriod = newHalfPeriod;
					avCurrStepVolume = 8;
				}
			}
			else {
				if (silent) {
					avHalfPeriod = 16;
					avCurrStepVolume = 0;
				}
				else {
					avHalfPeriod = newHalfPeriod;
					avCurrStepVolume = -avCurrStepVolume;

					if (avCurrStepVolume > 0) {
						avCurrStepVolume -= 1;
						if (avCurrStepVolume == 0) {
							silent = SDL_TRUE;
						}
					}
				}
			}
		}


		if (avCurrStepVolume != 0 || avPrevStepVolume != 0) {
			if (avPos < 16) {
				if (avPrevStepVolume > avCurrStepVolume) {
					output += stepDeltas[avPrevStepVolume - avCurrStepVolume][avPos];
				}
				else {
					output -= stepDeltas[avCurrStepVolume - avPrevStepVolume][avPos];
				}
			}
			avPos += 1;
		}
		else {
			avPos = avHalfPeriod;
		}


		writePtr[i] = output;
	}
}

int main(int argc, char** argv) {
	int result = 0;
	bb = creat_BB();
	fill_BB(bb, 128, 128, 128);
	bgMapper = creat_Mapper(1);
	spriteMapper = creat_Mapper(2);
	ppu = creat_PPU(bgMapper, spriteMapper);

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
						silent = !silent;
					}
					if (event.key.keysym.sym == SDLK_q) { newHalfPeriod = 20; }
					if (event.key.keysym.sym == SDLK_a) { newHalfPeriod = 110; }
					if (event.key.keysym.sym == SDLK_s) { newHalfPeriod = 150; }
					if (event.key.keysym.sym == SDLK_d) { newHalfPeriod = 200; }
					if (event.key.keysym.sym == SDLK_f) { newHalfPeriod = 250; }
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

	destr_PPU(ppu);
	destr_Mapper(bgMapper);
	destr_Mapper(spriteMapper);
	destr_BB(bb);
	return result;
}
