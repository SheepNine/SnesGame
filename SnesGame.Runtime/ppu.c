#include "ppu.h"


struct PPU {
	hMapper bgMapper;
	hMapper spriteMapper;

	Uint8 bgPalette[256];
	Uint8 spritePalette[256];

	Uint8 brushMaps[8192];
	Uint8 bgControls[4];
} PPU;

hPPU creat_PPU(hMapper bgMapper, hMapper spriteMapper) {
	hPPU result = (hPPU)SDL_malloc(sizeof(PPU));
	result->bgMapper = bgMapper;
	result->spriteMapper = spriteMapper;
	SDL_memset(result->bgPalette, 0, 256);
	SDL_memset(result->spritePalette, 0, 256);
	SDL_memset(result->brushMaps, 0, 8192);
	SDL_memset(result->bgControls, 0, 4);
	return result;
}

void destr_PPU(hPPU ppu) {
	SDL_free(ppu);
}

void switchBgBank_PPU(hPPU ppu, Uint8 bank, int page) {
	SDL_assert(bank < 4);
	switchBank_Mapper(ppu->bgMapper, bank, page);
}

void switchSpriteBank_PPU(hPPU ppu, Uint8 bank, int page) {
	SDL_assert(bank < 4);
	switchBank_Mapper(ppu->spriteMapper, bank, page);
}

void _packBrush_PPU(Uint8* brush, Uint8 glyphIndex, Uint8 bankIndex, Uint8 swatchIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	brush[0] = (mask0 ? 0x80 : 0x00) | (hFlip ? 0x40 : 0x00) | (vFlip ? 0x20 : 0x00) | (swatchIndex << 2) | (bankIndex);
	brush[1] = glyphIndex;
}

void _unpackBrush_PPU(Uint8* brush, Uint8* glyphIndex, Uint8* bankIndex, Uint8* swatchIndex, SDL_bool* hFlip, SDL_bool* vFlip, SDL_bool* mask0) {
	*mask0 = (brush[0] & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
	*hFlip = (brush[0] & 0x40) == 0 ? SDL_FALSE : SDL_TRUE;
	*vFlip = (brush[0] & 0x20) == 0 ? SDL_FALSE : SDL_TRUE;
	*swatchIndex = (brush[0] & 0x18) >> 2;
	*bankIndex = (brush[0] & 0x7);
	*glyphIndex = brush[1];
}

void _packColor_PPU(Uint8* color, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	color[0] = (b & 0xF8) | ((g >> 3) & 0x07);
	color[1] = (r & 0xF8) | (g >> 6) | (t ? 0x04 : 0x00);
}

void _packBgControl_PPU(Uint8* control, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost) {
	control[0] = (visible ? 0x80 : 0x00) | (topmost ? 0x40 : 0x00) | (dX << 3) | (dY);
}

void _unpackBgControl_PPU(Uint8* control, Uint8* dX, Uint8* dY, SDL_bool* visible, SDL_bool* topmost) {
	*visible = (*control & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
	*topmost = (*control & 0x40) == 0 ? SDL_FALSE : SDL_TRUE;
	*dX = (*control & 0x28) >> 3;
	*dY = (*control & 0x07);
}

Uint8* _getSwatch_PPU(Uint8* palette, Uint8 swatchIndex) {
	return palette + 32 * swatchIndex;
}

void _setPaletteColor_PPU(Uint8* palette, Uint8 swatchIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	_packColor_PPU(palette + 32 * swatchIndex + 2 * colorIndex, r, g, b, t);
}

void setBgColor_PPU(hPPU ppu, Uint8 swatch, Uint8 color, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	_setPaletteColor_PPU(ppu->bgPalette, swatch, color, r, g, b, t);
}

void setSpriteColor_PPU(hPPU ppu, Uint8 swatch, Uint8 color, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	_setPaletteColor_PPU(ppu->spritePalette, swatch, color, r, g, b, t);
}

void setBgBrush_PPU(hPPU ppu, Uint8 layer, Uint8 x, Uint8 y, Uint8 glyphIndex, Uint8 bankIndex, Uint8 swatchIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	_packBrush_PPU(ppu->brushMaps + 2 * (32 * (32 * layer + y) + x), glyphIndex, bankIndex, swatchIndex, hFlip, vFlip, mask0);
}

void setBgControl_PPU(hPPU ppu, Uint8 layer, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost) {
	_packBgControl_PPU(ppu->bgControls + layer, dX, dY, visible, topmost);
}

void scanLayer_PPU(hPPU ppu, Uint8 layer, int destScanline, hSL sl) {
	Uint8 dX, dY;
	SDL_bool visible, topmost;
	_unpackBgControl_PPU(ppu->bgControls + layer, &dX, &dY, &visible, &topmost);

	if (!visible) {
		return;
	}

	int srcScanLine = destScanline + dY;
	int brushBarIndex = srcScanLine / 8;
	int barIndex = srcScanLine % 8;

	Uint8* brushBar = ppu->brushMaps + (64 * (32 * layer + brushBarIndex));

	int destX = -dX;
	for (int i = 0; i < 32; i++) {
		Uint8 glyphIndex, bankIndex, swatchIndex;
		SDL_bool hFlip, vFlip, mask0;
		_unpackBrush_PPU(brushBar, &glyphIndex, &bankIndex, &swatchIndex, &hFlip, &vFlip, &mask0);

		scanBar_SL(sl, getBar_Mapper(ppu->bgMapper, bankIndex, glyphIndex, vFlip ? 7 - barIndex : barIndex),
			_getSwatch_PPU(ppu->bgPalette, swatchIndex), destX, hFlip, mask0);

		brushBar += 2;
		destX += 8;
	}
}

void scan_PPU(hPPU ppu, hBB bb) {
	for (int i = 0; i < 248; i++) {
		hSL sl = creat_SL(bb, i);
		scanLayer_PPU(ppu, 0, i, sl);
		destr_SL(sl);
	}
}