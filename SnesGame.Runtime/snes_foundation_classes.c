#include "api/snes_foundation_classes.h"

void drawBackdropString(hRENDER render, Uint8 backdropIndex, char* string, Uint8 x, Uint8 y, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool mask0) {
	while (x < 32 && (*string) != 0) {
		setBackdropStroke(render, backdropIndex, x++, y, *(string++), bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, mask0);
	}
}

void fillBackdrop(hRENDER render, Uint8 backdropIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			setBackdropStroke(render, backdropIndex, x, y, brushIndex, bankIndex, paletteIndex, hFlip, vFlip, mask0);
		}
	}
}

struct CS {
	Sint32 worldLocusX;
	Sint32 worldLocusY;
	Sint32 screenLocusX;
	Sint32 screenLocusY;
} CS;

hCS creat_CS() {
	hCS result = (hCS)SDL_malloc(sizeof(hCS));
	result->worldLocusX = 0;
	result->worldLocusY = 0;
	result->screenLocusX = 124;
	result->screenLocusY = 124;
	return result;
}

void destr_CS(hCS cs) {
	SDL_free(cs);
}

void setWorldLocus_CS(hCS cs, Sint32 x, Sint32 y) {
	cs->worldLocusX = x;
	cs->worldLocusY = y;
}

void setScreenLocus_CS(hCS cs, Uint8 x, Uint8 y) {
	cs->screenLocusX = x;
	cs->screenLocusY = y;
}

void worldToScreen_CS(hCS cs, Sint32 worldX, Sint32 worldY, Sint32* screenX, Sint32* screenY) {
	*screenX = cs->screenLocusX + ((worldX - cs->worldLocusX + cs->worldLocusY - worldY + (1 << 11)) >> 12);
	*screenY = cs->screenLocusY - ((worldX - cs->worldLocusX + worldY - cs->worldLocusY + (1 << 12)) >> 13);
}
