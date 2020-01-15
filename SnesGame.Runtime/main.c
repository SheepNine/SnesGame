#include "SDL.h"
#include <stdio.h>

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

int main(int argc, char** argv) {
	int result = 0;
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Window* window = SDL_CreateWindow(
				"SnesGame",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				640, 480,
				0);
		if (window != NULL) {
			SDL_Surface* surface = SDL_GetWindowSurface(window);
			if (surface != NULL) {
				// Error check the lock/unlock/update
				// Check the window surface format just in case it isn't 8bpp ARGB
				SDL_LockSurface(surface);
				for (int y = 0; y < 480; y++) {
					for (int x = 0; x < 640; x++) {
						int i = 640 * y + x;
						Uint32 r = (255 * x) / 639;
						Uint32 b = (255 * y) / 479;
						((Uint32*)surface->pixels)[i] = (r << 16) | (b);
					}
				}
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
						x = (x + 1) % (640 * 2);
						y = (y + 1) % (480 * 2);
						int i = 640 * (y >= 480 ? (480 * 2 - 1) - y : y) + (x >= 640 ? (640 * 2 - 1) - x : x);
						SDL_LockSurface(surface);
						((Uint32*)surface->pixels)[i] = 0x00FFFFFF;
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
