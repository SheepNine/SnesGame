#pragma once
#include "SDL.h"
#include "mapper.h"
#include "bb.h"

typedef struct PPU* hPPU;

hPPU creat_PPU(hMapper bgMapper, hMapper spriteMapper);
void destr_PPU(hPPU ppu);

void switchBgBank_PPU(hPPU ppu, Uint8 bank, int page);
void switchSpriteBank_PPU(hPPU ppu, Uint8 bank, int page);

void setBgColor_PPU(hPPU ppu, Uint8 swatch, Uint8 color, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setSpriteColor_PPU(hPPU ppu, Uint8 swatch, Uint8 color, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);

void setBgBrush_PPU(hPPU ppu, Uint8 layer, Uint8 x, Uint8 y, Uint8 glyphIndex, Uint8 bankIndex, Uint8 swatchIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setBgControl_PPU(hPPU ppu, Uint8 layer, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost);

void scan_PPU(hPPU ppu, hBB bb);