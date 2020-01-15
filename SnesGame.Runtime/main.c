#include "SDL.h"
#include <stdio.h>

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

				SDL_bool loop = SDL_TRUE;
				SDL_Event event;
				while (loop) {
					SDL_PollEvent(&event);
					switch (event.type) {
					case SDL_QUIT:
						loop = SDL_FALSE;
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
