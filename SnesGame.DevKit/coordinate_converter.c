#include "api/snes_devkit.h"

struct CC {
	Sint32 worldLocusX;
	Sint32 worldLocusY;
	Sint32 screenLocusX;
	Sint32 screenLocusY;
} CC;

hCC creat_CC() {
	hCC result = (hCC)SDL_malloc(sizeof(hCC));
	result->worldLocusX = 0;
	result->worldLocusY = 0;
	result->screenLocusX = 124;
	result->screenLocusY = 124;
	return result;
}

void destr_CC(hCC cc) {
	SDL_free(cc);
}

void setWorldLocus_CC(hCC cc, Sint32 x, Sint32 y) {
	cc->worldLocusX = x;
	cc->worldLocusY = y;
}

void setScreenLocus_CC(hCC cc, Sint32 x, Sint32 y) {
	cc->screenLocusX = x;
	cc->screenLocusY = y;
}

void worldToScreen_CC(hCC cc, Sint32 worldX, Sint32 worldY, Sint32* screenX, Sint32* screenY) {
	*screenX = cc->screenLocusX + ((worldX - cc->worldLocusX) << 4) - ((worldY - cc->worldLocusY) << 4);
	*screenY = cc->screenLocusY - ((worldX - cc->worldLocusX) << 3) - ((worldY - cc->worldLocusY) << 3);
}

void screenToWorld_CC(hCC cc, Sint32 screenX, Sint32 screenY, Sint32* worldX, Sint32* worldY) {
	*worldX = cc->worldLocusX + ((screenX - cc->screenLocusX) >> 5) - ((screenY - cc->screenLocusY) >> 4);
	*worldY = cc->worldLocusY - ((screenX - cc->screenLocusX) >> 5) - ((screenY - cc->screenLocusY) >> 4);
}
