#include "render.h"

struct RENDER {
	hPPU ppu;
} RENDER;

hRENDER creat_RENDER(hPPU ppu) {
	hRENDER result = (hRENDER)SDL_malloc(sizeof(RENDER));
	result->ppu = ppu;
	return result;
}

void destr_RENDER(hRENDER render) {
	// Proxy class; doesn't own any of its references
}

void switchBackdropBrushList(hRENDER render, Uint8 bankIndex, Uint16 brushListIndex) {
	switchBackdropBrushList_PPU(render->ppu, bankIndex, brushListIndex);
}

void setBackdropPaletteColor(hRENDER render, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	setBackdropPaletteColor_PPU(render->ppu, paletteIndex, colorIndex, r, g, b, t);
}

void setBackdropStroke(hRENDER render, Uint8 backdropIndex, Uint8 x, Uint8 y, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	setBackdropStroke_PPU(render->ppu, backdropIndex, x, y, brushIndex, bankIndex, paletteIndex, hFlip, vFlip, mask0);
}

void setBackdropControl(hRENDER render, Uint8 backdropIndex, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost) {
	setBackdropControl_PPU(render->ppu, backdropIndex, dX, dY, visible, topmost);
}

void switchActorBrushList(hRENDER render, Uint8 bankIndex, Uint16 brushListIndex) {
	switchActorBrushList_PPU(render->ppu, bankIndex, brushListIndex);
}

void setActorPaletteColor(hRENDER render, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	setActorPaletteColor_PPU(render->ppu, paletteIndex, colorIndex, r, g, b, t);
}

void setActorStroke(hRENDER render, Uint8 actorIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	setActorStroke_PPU(render->ppu, actorIndex, brushIndex, bankIndex, paletteIndex, hFlip, vFlip, mask0);
}

void setActorControl(hRENDER render, Uint8 actorIndex, Sint16 x, Sint16 y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer) {
	setActorControl_PPU(render->ppu, actorIndex, x, y, sizeX, sizeY, visible, layer);
}

void setFullLayerClips(hRENDER render, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight, SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom) {
	setFullLayerClips_PPU(render->ppu, layerIndex, leftWidth, rightWidth, topHeight, bottomHeight, invertLeft, invertRight, invertTop, invertBottom);
}

void setLayerClips(hRENDER render, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight) {
	setLayerClips_PPU(render->ppu, layerIndex, leftWidth, rightWidth, topHeight, bottomHeight);
}