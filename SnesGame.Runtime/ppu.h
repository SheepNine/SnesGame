#pragma once
#include "SDL.h"
#include "mapper.h"
#include "bb.h"

typedef struct PPU* hPPU;

hPPU creat_PPU(hMapper romMapper);
void destr_PPU(hPPU ppu);

void debugToggleBackdrop_PPU(hPPU ppu, Uint8 layerIndex);
void debugToggleActor_PPU(hPPU ppu, Uint8 layerIndex);

void switchBackdropBrushList_PPU(hPPU ppu, Uint8 bankIndex, Uint16 brushListIndex);
void setBackdropPaletteColor_PPU(hPPU ppu, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setBackdropPalette_PPU(hPPU ppu, Uint8 paletteIndex, Uint8* data);
void setBackdropStroke_PPU(hPPU ppu, Uint8 backdropIndex, Uint8 x, Uint8 y, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setBackdropControl_PPU(hPPU ppu, Uint8 backdropIndex, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost);

void switchActorBrushList_PPU(hPPU ppu, Uint8 bankIndex, Uint16 brushListIndex);
void setActorPaletteColor_PPU(hPPU ppu, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setActorPalette_PPU(hPPU ppu, Uint8 paletteIndex, Uint8* data);
void setActorStroke_PPU(hPPU ppu, Uint8 actorIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setActorControl_PPU(hPPU ppu, Uint8 actorIndex, int x, int y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer);

void setFullLayerClips_PPU(hPPU ppu, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight,
	SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom);
void setLayerClips_PPU(hPPU ppu, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight);


void scan_PPU(hPPU ppu, hBB bb);