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
