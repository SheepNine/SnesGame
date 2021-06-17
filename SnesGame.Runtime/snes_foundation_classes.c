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
