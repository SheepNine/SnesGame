#include <wtypes.h>
#include "snes_api.h"

SDL_bool controllerState[12];

#define BL_DEFAULT 0
#define BL_ASCII 1
#define BB_2 2
#define BB_3 3
#define AB_3 3

void initFunc(hINIT init) {
	allocBrushLists(init, 2);
	loadBrushList(init, BL_DEFAULT, "..\\resources\\default.glyphset");
	loadBrushList(init, BL_ASCII, "..\\resources\\ascii.glyphset");
}

void updateFunc(hUPDATE update) {

	controllerState[ 0] = isButtonIn(update, GP_BUTTON_DU) == SDL_TRUE;
	controllerState[ 1] = isButtonIn(update, GP_BUTTON_DR) == SDL_TRUE;
	controllerState[ 2] = isButtonIn(update, GP_BUTTON_DD) == SDL_TRUE;
	controllerState[ 3] = isButtonIn(update, GP_BUTTON_DL) == SDL_TRUE;
	controllerState[ 4] = isButtonIn(update, GP_BUTTON_BX) == SDL_TRUE;
	controllerState[ 5] = isButtonIn(update, GP_BUTTON_BA) == SDL_TRUE;
	controllerState[ 6] = isButtonIn(update, GP_BUTTON_BB) == SDL_TRUE;
	controllerState[ 7] = isButtonIn(update, GP_BUTTON_BY) == SDL_TRUE;
	controllerState[ 8] = isButtonIn(update, GP_BUTTON_SL) == SDL_TRUE;
	controllerState[ 9] = isButtonIn(update, GP_BUTTON_ST) == SDL_TRUE;
	controllerState[10] = isButtonIn(update, GP_BUTTON_ZL) == SDL_TRUE;
	controllerState[11] = isButtonIn(update, GP_BUTTON_ZR) == SDL_TRUE;
}

void drawString(hRENDER render, Uint8 backdropIndex, char* data, Uint8 x, Uint8 y, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool mask0) {
	while (x < 32 && (*data) != 0) {
		setBackdropStroke(render, backdropIndex, x++, y, *(data++), bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, mask0);
	}
}

void fillBackdrop(hRENDER render, Uint8 backdropIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			setBackdropStroke(render, backdropIndex, x, y, brushIndex, bankIndex, paletteIndex, hFlip, vFlip, mask0);
		}
	}
}

void renderFunc(hRENDER render) {
	setBackdropPaletteColor(render, 0, 0x0, 0x00, 0x00, 0x00, SDL_FALSE); // Black
	setBackdropPaletteColor(render, 0, 0x1, 0x00, 0x00, 0xAA, SDL_FALSE); // Blue
	setBackdropPaletteColor(render, 0, 0x2, 0x00, 0xAA, 0x00, SDL_FALSE); // Green
	setBackdropPaletteColor(render, 0, 0x3, 0x00, 0xAA, 0xAA, SDL_FALSE); // Cyan
	setBackdropPaletteColor(render, 0, 0x4, 0xAA, 0x00, 0x00, SDL_FALSE); // Red
	setBackdropPaletteColor(render, 0, 0x5, 0xAA, 0x00, 0xAA, SDL_FALSE); // Magenta
	setBackdropPaletteColor(render, 0, 0x6, 0xAA, 0x55, 0x00, SDL_FALSE); // Brown
	setBackdropPaletteColor(render, 0, 0x7, 0xAA, 0xAA, 0xAA, SDL_FALSE); // Light gray
	setBackdropPaletteColor(render, 0, 0x8, 0x55, 0x55, 0x55, SDL_FALSE); // Dark gray
	setBackdropPaletteColor(render, 0, 0x9, 0x55, 0x55, 0xFF, SDL_FALSE); // Light blue
	setBackdropPaletteColor(render, 0, 0xA, 0x55, 0xFF, 0x55, SDL_FALSE); // Light green
	setBackdropPaletteColor(render, 0, 0xB, 0x55, 0xFF, 0xFF, SDL_FALSE); // Light cyan
	setBackdropPaletteColor(render, 0, 0xC, 0xFF, 0x55, 0x55, SDL_FALSE); // Light red
	setBackdropPaletteColor(render, 0, 0xD, 0xFF, 0x55, 0xFF, SDL_FALSE); // Light magenta
	setBackdropPaletteColor(render, 0, 0xE, 0xFF, 0xFF, 0x55, SDL_FALSE); // Yellow
	setBackdropPaletteColor(render, 0, 0xF, 0xFF, 0xFF, 0xFF, SDL_FALSE); // White

	setBackdropPaletteColor(render, 1, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setBackdropPaletteColor(render, 1, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setBackdropPaletteColor(render, 1, 0x2, 0xFF, 0x55, 0x55, SDL_TRUE);

	setActorPaletteColor(render, 7, 0x0, 0x00, 0x00, 0x00, SDL_FALSE); // Black
	setActorPaletteColor(render, 7, 0x1, 0x00, 0x00, 0xAA, SDL_FALSE); // Blue
	setActorPaletteColor(render, 7, 0x2, 0x00, 0xAA, 0x00, SDL_FALSE); // Green
	setActorPaletteColor(render, 7, 0x3, 0x00, 0xAA, 0xAA, SDL_FALSE); // Cyan
	setActorPaletteColor(render, 7, 0x4, 0xAA, 0x00, 0x00, SDL_FALSE); // Red
	setActorPaletteColor(render, 7, 0x5, 0xAA, 0x00, 0xAA, SDL_FALSE); // Magenta
	setActorPaletteColor(render, 7, 0x6, 0xAA, 0x55, 0x00, SDL_FALSE); // Brown
	setActorPaletteColor(render, 7, 0x7, 0xAA, 0xAA, 0xAA, SDL_FALSE); // Light gray
	setActorPaletteColor(render, 7, 0x8, 0x55, 0x55, 0x55, SDL_FALSE); // Dark gray
	setActorPaletteColor(render, 7, 0x9, 0x55, 0x55, 0xFF, SDL_FALSE); // Light blue
	setActorPaletteColor(render, 7, 0xA, 0x55, 0xFF, 0x55, SDL_FALSE); // Light green
	setActorPaletteColor(render, 7, 0xB, 0x55, 0xFF, 0xFF, SDL_FALSE); // Light cyan
	setActorPaletteColor(render, 7, 0xC, 0xFF, 0x55, 0x55, SDL_FALSE); // Light red
	setActorPaletteColor(render, 7, 0xD, 0xFF, 0x55, 0xFF, SDL_FALSE); // Light magenta
	setActorPaletteColor(render, 7, 0xE, 0xFF, 0xFF, 0x55, SDL_FALSE); // Yellow
	setActorPaletteColor(render, 7, 0xF, 0xFF, 0xFF, 0xFF, SDL_FALSE); // White

	setBackdropControl(render, 0, 0, 0, SDL_TRUE, SDL_FALSE);

	setActorControl(render, 0, 4, 9, 2, 2, SDL_TRUE, 0); setActorStroke(render, 0, 1, 0, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorControl(render, 1, 25, 11, 2, 2, SDL_TRUE, 0); setActorStroke(render, 1, 1, 0, 7, SDL_TRUE, SDL_FALSE, SDL_FALSE);
	setActorControl(render, 2, 42, 14, 2, 2, SDL_TRUE, 0); setActorStroke(render, 2, 1, 0, 7, SDL_TRUE, SDL_TRUE, SDL_TRUE);
	setActorControl(render, 3, 60, 20, 2, 2, SDL_TRUE, 0); setActorStroke(render, 3, 1, 0, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setActorControl(render, 4, -5, -5, 2, 2, SDL_TRUE, 0); setActorStroke(render, 4, 1, 0, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setActorControl(render, 5, -5, 243, 2, 2, SDL_TRUE, 0); setActorStroke(render, 5, 1, 0, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setActorControl(render, 6, 243, 243, 2, 2, SDL_TRUE, 0); setActorStroke(render, 6, 1, 0, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);
	setActorControl(render, 7, 243, -5, 2, 2, SDL_TRUE, 0); setActorStroke(render, 7, 1, 0, 7, SDL_FALSE, SDL_TRUE, SDL_TRUE);

	setActorControl(render, 8, 8 + 64, 8 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 9, 16 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 10, 8 + 64, 24 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 11, 0 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 12, 72 + 64, 8 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 13, 80 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 14, 72 + 64, 24 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 15, 64 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 16, 32 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 17, 48 + 64, 16 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 18, 24 + 64, 0 + 64, 1, 1, SDL_TRUE, 0);
	setActorControl(render, 19, 56 + 64, 0 + 64, 1, 1, SDL_TRUE, 0);

	setLayerClips(render, 0, 1, 3, 4, 9);

	switchBackdropBrushList(render, BB_2, BL_DEFAULT);
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

			setBackdropStroke(render, 0, x, y, glyph, BB_2, 0, x % 4 == 1, x % 4 == 3, SDL_TRUE);
		}
	}

	switchActorBrushList(render, AB_3, BL_DEFAULT);
	setActorStroke(render,  8, controllerState[ 0] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render,  9, controllerState[ 1] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 10, controllerState[ 2] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 11, controllerState[ 3] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 12, controllerState[ 4] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 13, controllerState[ 5] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 14, controllerState[ 6] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 15, controllerState[ 7] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 16, controllerState[ 8] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 17, controllerState[ 9] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 18, controllerState[10] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorStroke(render, 19, controllerState[11] ? 66 : 50, AB_3, 7, SDL_FALSE, SDL_FALSE, SDL_TRUE);


	switchBackdropBrushList(render, BB_3, BL_ASCII);
	setLayerClips(render, 1, 0, 0, 0, 0);
	setBackdropControl(render, 1, 0, 0, SDL_TRUE, SDL_TRUE);
	fillBackdrop(render, 1, 0, BB_3, 1, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	drawString(render, 1, "HELLO, WORLD!", 1, 1, BB_3, 1, SDL_TRUE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	return libMain("Tendies", *initFunc, *updateFunc, *renderFunc);
}