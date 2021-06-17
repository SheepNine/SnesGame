#include <wtypes.h>
#include "snes_api.h"
#include "snes_foundation_classes.h"

SDL_bool controllerState[12];

#define BL_ASCII 0

void initFunc(hINIT init) {
	allocBrushLists(init, 1);
	loadBrushList(init, BL_ASCII, "..\\resources\\ascii.glyphset");
}

void updateFunc(hUPDATE update) {
	if (wasButtonPressed(update, GP_BUTTON_SL)) {
		// implement wasButtonPressed!
		//silenceChannel(update, 0);
	}
	if (wasButtonPressed(update, GP_BUTTON_ST)) {
		// implement wasButtonPressed!
		//playSquareNote(update, 0, 12000, 8, SD_NONE, 0, EB_SILENCE, 55, 55, SD_NONE, 0, EB_SILENCE);
	}
}

void renderFunc(hRENDER render) {
	switchBackdropBrushList(render, 0, BL_ASCII);
	setBackdropPaletteColor(render, 0, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setBackdropPaletteColor(render, 0, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setBackdropPaletteColor(render, 0, 0x2, 0x80, 0x55, 0x55, SDL_FALSE);

	setLayerClips(render, 0, 0, 0, 0, 0);
	setBackdropControl(render, 0, 0, 0, SDL_TRUE, SDL_FALSE);
	fillBackdrop(render, 0, 0, 0, 0, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	drawBackdropString(render, 0, "HELLO, WORLD!", 2, 2, 0, 0, SDL_TRUE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	return libMain("Tendies", *initFunc, *updateFunc, *renderFunc);
}