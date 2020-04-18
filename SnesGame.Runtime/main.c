#include "SDL.h"
#include <stdio.h>
#include "bb.h"
#include "mapper.h"
#include "ppu.h"

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

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Window* window = SDL_CreateWindow(
				"SnesGame",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				770, 770,
				SDL_WINDOW_RESIZABLE);
		if (window != NULL) {
			// Error check the lock/unlock/update
			// Check the window surface format just in case it isn't 8bpp ARGB

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
					scan_PPU(ppu, bb);
					SDL_Surface* surface = SDL_GetWindowSurface(window);
					if (surface != NULL) {
						SDL_LockSurface(surface);
						blit_BB(bb, surface);
						SDL_UnlockSurface(surface);
						SDL_UpdateWindowSurface(window);
					}
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

	destr_PPU(ppu);
	destr_Mapper(bgMapper);
	destr_Mapper(spriteMapper);
	destr_BB(bb);
	return result;
}
