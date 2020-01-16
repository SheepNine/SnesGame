#include "SDL.h"
#include <stdio.h>

// --- BackBuffer ---

Uint8 bbR[248 * 248];
Uint8 bbG[248 * 248];
Uint8 bbB[248 * 248];

void bbClear(Uint8 r, Uint8 g, Uint8 b) {
	SDL_memset(bbR, r, 248 * 248);
	SDL_memset(bbG, g, 248 * 248);
	SDL_memset(bbB, b, 248 * 248);
}

void bbSet(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
	int i = (y * 248) + x;
	bbR[i] = r;
	bbG[i] = g;
	bbB[i] = b;
}

Uint32 bbGet(int x, int y) {
	int i = (y * 248) + x;
	return (bbR[i] << 16) | (bbG[i] << 8) | (bbB[i]);
}

void bbBlit(Uint32* dest) {
	int i = 0;
	for (int y = 0; y < 248; y++) {
		for (int x = 0; x < 248; x++) {
			int i = (y * 248) + x;
			dest[i] = bbGet(x, y);
		}
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

int main(int argc, char** argv) {
	int result = 0;
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Window* window = SDL_CreateWindow(
				"SnesGame",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				248, 248,
				0);
		if (window != NULL) {
			SDL_Surface* surface = SDL_GetWindowSurface(window);
			if (surface != NULL) {
				// Error check the lock/unlock/update
				// Check the window surface format just in case it isn't 8bpp ARGB
				bbClear(0x64, 0x95, 0xED);
				SDL_LockSurface(surface);
				bbBlit((Uint32*)surface->pixels);
				SDL_UnlockSurface(surface);
				SDL_UpdateWindowSurface(window);

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
					case SDL_USEREVENT:
						x = (x + 7) % (R_WIDTH * 2);
						y = (y + 12) % (R_HEIGHT * 2);
						int dX = x >= R_WIDTH ? (R_WIDTH * 2 - 1) - x : x;
						int dY = y >= R_HEIGHT ? (R_HEIGHT * 2 - 1) - y : y;
						bbSet(dX, dY, 0xFF, 0xFF, 0xFF);

						SDL_LockSurface(surface);
						bbBlit((Uint32*)surface->pixels);
						SDL_UnlockSurface(surface);
						SDL_UpdateWindowSurface(window);
						break;
					}
				}
			}
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
	return result;
}
