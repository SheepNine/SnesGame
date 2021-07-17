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

void setScreenLocus_CS(hCS cs, Sint32 x, Sint32 y) {
	cs->screenLocusX = x;
	cs->screenLocusY = y;
}

void worldToScreen_CS(hCS cs, Sint32 worldX, Sint32 worldY, Sint32* screenX, Sint32* screenY) {
	*screenX = cs->screenLocusX + ((worldX - cs->worldLocusX) << 4) - ((worldY - cs->worldLocusY) << 4);
	*screenY = cs->screenLocusY - ((worldX - cs->worldLocusX) << 3) - ((worldY - cs->worldLocusY) << 3);
}

void screenToWorld_CS(hCS cs, Sint32 screenX, Sint32 screenY, Sint32* worldX, Sint32* worldY) {
	*worldX = cs->worldLocusX + ((screenX - cs->screenLocusX) >> 5) - ((screenY - cs->screenLocusY) >> 4);
	*worldY = cs->worldLocusY - ((screenX - cs->screenLocusX) >> 5) - ((screenY - cs->screenLocusY) >> 4);
}


struct WB {
	Uint16 width;
	Uint16 height;
	Uint8* data;
} WB;

hWB creat_WB(Uint16 width, Uint16 height, Uint8* data) {
	hWB result = (hWB)SDL_malloc(sizeof(WB));
	result->width = width;
	result->height = height;
	result->data = SDL_malloc(width * height);
	SDL_memcpy(result->data, data, width * height);
	return result;
}

void destr_WB(hWB wb) {
	SDL_free(wb->data);
	SDL_free(wb);
}

Uint16 getWidth_WB(hWB wb) {
	return wb->width;
}

Uint16 getHeight_WB(hWB wb) {
	return wb->height;
}


void draw_WB(hRENDER render, Uint8 backdropIndex, SDL_bool topmost, hWB wb, Sint16 x, Sint16 y, Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0) {
	Uint8 dX = (8 - x) & 0x7;
	Uint8 dY = (8 - y) & 0x7;
	setBackdropControl(render, backdropIndex, dX, dY, SDL_TRUE, topmost);

	Sint16 srcX = (-x) >> 3;
	Sint16 srcY = (-y) >> 3;

	for (Uint8 y = 0; y < 32; y++)
	{
		for (Uint8 x = 0; x < 32; x++)
		{
			Uint8 brushIndex;
			if (srcX >= 0 && srcX < wb->width && srcY >= 0 && srcY < wb->height) {
				brushIndex = wb->data[wb->width * srcY + srcX];
			}
			else {
				brushIndex = 0;
			}
			setBackdropStroke(render, backdropIndex, x, y, brushIndex, bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, mask0);
			srcX += 1;
		}
		srcX -= 32;
		srcY += 1;
	}
}
