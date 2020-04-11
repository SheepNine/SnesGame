#include "SDL.h"
#include <stdio.h>

// --- BackBuffer ---

Uint8 bbR[248 * 248];
Uint8 bbG[248 * 248];
Uint8 bbB[248 * 248];

void readGlyphList(char *filename, Uint8 *glyphlist) {
	SDL_memset(glyphlist, 0, 8192);
	SDL_RWops* rw = SDL_RWFromFile(filename, "r");
	if (rw != NULL) {
		SDL_RWread(rw, glyphlist, 8192, 1);
		SDL_RWclose(rw);
	}
}

void bbClear(Uint8 r, Uint8 g, Uint8 b) {
	SDL_memset(bbR, r, 248 * 248);
	SDL_memset(bbG, g, 248 * 248);
	SDL_memset(bbB, b, 248 * 248);
}

void bbSet(int x, int y, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	int i = (y * 248) + x;
	bbR[i] = (r >> 1) + ((t ? bbR[i] : r) >> 1);
	bbG[i] = (g >> 1) + ((t ? bbG[i] : g) >> 1);
	bbB[i] = (b >> 1) + ((t ? bbB[i] : b) >> 1);
}

void ppuSetPixel(int x, int y, Uint8 pIndex, Uint8* palette) {
	SDL_assert(pIndex < 16);
	Uint8 paletteLow = palette[pIndex * 2];
	Uint8 paletteHigh = palette[pIndex * 2 + 1];

	SDL_bool a = (paletteHigh & 0x4) != 0;
	Uint8 r = paletteHigh & 0xF8;
	paletteHigh <<= 6;
	Uint8 b = paletteLow & 0xF8;
	paletteLow <<= 3;
	Uint8 g = (paletteHigh & 0xC0) | (paletteLow & 0x38);

	bbSet(x, y, r, g, b, a);
}

void ppuDrawBar(int x, int y, Uint8* data, Uint8* palette, SDL_bool hFlip) {
	Uint8 mask = hFlip ? 0x80 : 0x01;
	for (int i = 0; i < 8; i++) {
		Uint8 pIndex = 0;
		if ((data[0] & mask) != 0) pIndex |= 1;
		if ((data[1] & mask) != 0) pIndex |= 2;
		if ((data[2] & mask) != 0) pIndex |= 4;
		if ((data[3] & mask) != 0) pIndex |= 8;
		ppuSetPixel(x + i, y, pIndex, palette);
		mask = hFlip ? mask >> 1 : mask << 1;
	}
}

// Probably won't end up using this in the final version, as it will slice the data by
// scanline (desination data) instead of sprites (source data)
void ppuDrawGlyph(int x, int y, Uint8* data, Uint8* palette, SDL_bool vFlip, SDL_bool hFlip) {
	Uint8 barIndex = vFlip ? 7 * 4 : 0;
	for (int i = 0; i < 8; i++) {
		ppuDrawBar(x, y + i, &data[barIndex], palette, hFlip);
		barIndex = vFlip ? barIndex - 4 : barIndex + 4;
	}
}

void ppuSetBarPaletteIndex(Uint8* data, int x, Uint8 value) {
	SDL_assert(value < 16);
	SDL_assert(x < 8);
	Uint8 mask = 0x80 >> x;
	for (int i = 0; i < 4; i++)
		data[i] = ((value & (1 << i)) == 0) ? (data[i] & ~mask) : (data[i] | mask);
}

void ppuSetGlyphPaletteIndex(Uint8* data, int x, int y, Uint8 value) {
	SDL_assert(value < 16);
	SDL_assert(x < 8);
	SDL_assert(y < 8);
	ppuSetBarPaletteIndex(&data[4 * y], x, value);
}

void ppuPackPalette(Uint8 pIndex, Uint8* palette, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	SDL_assert(pIndex < 16);

	Uint8 paletteHigh = (r & 0xF8) | (g >> 6) | (t ? 0x04 : 0x00);
	Uint8 paletteLow = (b & 0xF8) | ((g >> 3) & 0x07);

	palette[2 * pIndex] = paletteLow;
	palette[2 * pIndex + 1] = paletteHigh;
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
	Uint8 palette[32];
	ppuPackPalette(0x0, palette, 0x00, 0x00, 0x00, SDL_FALSE); // Black
	ppuPackPalette(0x1, palette, 0x00, 0x00, 0xAA, SDL_FALSE); // Blue
	ppuPackPalette(0x2, palette, 0x00, 0xAA, 0x00, SDL_FALSE); // Green
	ppuPackPalette(0x3, palette, 0x00, 0xAA, 0xAA, SDL_FALSE); // Cyan
	ppuPackPalette(0x4, palette, 0xAA, 0x00, 0x00, SDL_FALSE); // Red
	ppuPackPalette(0x5, palette, 0xAA, 0x00, 0xAA, SDL_FALSE); // Magenta
	ppuPackPalette(0x6, palette, 0xAA, 0x55, 0x00, SDL_FALSE); // Brown
	ppuPackPalette(0x7, palette, 0xAA, 0xAA, 0xAA, SDL_FALSE); // Light gray
	ppuPackPalette(0x8, palette, 0x55, 0x55, 0x55, SDL_FALSE); // Dark gray
	ppuPackPalette(0x9, palette, 0x55, 0x55, 0xFF, SDL_FALSE); // Light blue
	ppuPackPalette(0xA, palette, 0x55, 0xFF, 0x55, SDL_FALSE); // Light green
	ppuPackPalette(0xB, palette, 0x55, 0xFF, 0xFF, SDL_FALSE); // Light cyan
	ppuPackPalette(0xC, palette, 0xFF, 0x55, 0x55, SDL_FALSE); // Light red
	ppuPackPalette(0xD, palette, 0xFF, 0x55, 0xFF, SDL_FALSE); // Light magenta
	ppuPackPalette(0xE, palette, 0xFF, 0xFF, 0x55, SDL_FALSE); // Yellow
	ppuPackPalette(0xF, palette, 0xFF, 0xFF, 0xFF, SDL_FALSE); // White

	Uint8 glyph_F[32] = { 0x7F, 0x7F, 0x7F, 0x7F, 0x03, 0x03, 0x03, 0xFF, 0x03, 0x03, 0x03, 0x07, 0x3F, 0x3F, 0x3F, 0x3F, 0x03, 0x03, 0x03, 0x7F, 0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x07, 0x00, 0x00, 0x00, 0x06 };

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
				for (int y = 0; y < 128; y++) {
					for (int x = 0; x < 248; x++) {
						ppuSetPixel(x, y, y >> 3, palette);
					}
				}
				ppuDrawGlyph(80, 192, glyph_F, palette, SDL_FALSE, SDL_FALSE);
				ppuDrawGlyph(89, 192, glyph_F, palette, SDL_FALSE, SDL_TRUE);
				ppuDrawGlyph(80, 201, glyph_F, palette, SDL_TRUE, SDL_FALSE);
				ppuDrawGlyph(89, 201, glyph_F, palette, SDL_TRUE, SDL_TRUE);
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
						bbSet(dX, dY, 0x80, 0x80, 0x80, SDL_TRUE);

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
