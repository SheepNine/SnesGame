#include "ppu.h"


struct PPU {
	hMapper bgMapper;
	hMapper spriteMapper;

	Uint8 bgPalette[256];
	Uint8 spritePalette[256];

	Uint8 brushMaps[8192];
	Uint8 bgControls[4];

	Uint8 layerClips[16];

	Uint8 spriteBrushes[248];
	Uint8 spriteControls[372];
} PPU;

hPPU creat_PPU(hMapper bgMapper, hMapper spriteMapper) {
	hPPU result = (hPPU)SDL_malloc(sizeof(PPU));
	result->bgMapper = bgMapper;
	result->spriteMapper = spriteMapper;
	SDL_memset(result->bgPalette, 0, 256);
	SDL_memset(result->spritePalette, 0, 256);
	SDL_memset(result->brushMaps, 0, 8192);
	SDL_memset(result->bgControls, 0, 4);
	SDL_memset(result->layerClips, 0, 16);
	SDL_memset(result->spriteBrushes, 0, 248);
	SDL_memset(result->spriteControls, 0, 372);
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
	*swatchIndex = (brush[0] & 0x1C) >> 2;
	*bankIndex = (brush[0] & 0x3);
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

void _unpackHClips_PPU(Uint8* clipSet, Uint8* leftWidth, Uint8* rightWidth, SDL_bool* invertLeft, SDL_bool* invertRight) {
	*leftWidth = clipSet[0] & 0x7F;
	*invertLeft = (clipSet[0] & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
	*rightWidth = clipSet[1] & 0x7F;
	*invertRight = (clipSet[1] & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
}

void _unpackVClips_PPU(Uint8* clipSet, Uint8* topWidth, Uint8* bottomWidth, SDL_bool* invertTop, SDL_bool* invertBottom) {
	*topWidth = clipSet[0] & 0x7F;
	*invertTop = (clipSet[0] & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
	*bottomWidth = clipSet[1] & 0x7F;
	*invertBottom = (clipSet[1] & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
}

void _packClips_PPU(Uint8* clipSet, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight,
	    SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom) {
	clipSet[0] = leftWidth | (invertLeft ? 0x80 : 0x00);
	clipSet[1] = rightWidth | (invertRight ? 0x80 : 0x00);
	clipSet[2] = topHeight | (invertTop ? 0x80 : 0x00);
	clipSet[3] = bottomHeight | (invertBottom ? 0x80 : 0x00);
}

void setFullLayerClips_PPU(hPPU ppu, Uint8 layer, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight,
	    SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom) {
	_packClips_PPU(ppu->layerClips + 4 * layer, leftWidth, rightWidth, topHeight, bottomHeight,
		invertLeft, invertRight, invertTop, invertBottom);
}

void setLayerClips_PPU(hPPU ppu, Uint8 layer, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight) {
	setFullLayerClips_PPU(ppu, layer, leftWidth, rightWidth, topHeight, bottomHeight, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE);
}

void setSpriteBrush_PPU(hPPU ppu, Uint8 sprite, Uint8 glyphIndex, Uint8 bankIndex, Uint8 swatchIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	_packBrush_PPU(ppu->spriteBrushes + 2 * sprite, glyphIndex, bankIndex, swatchIndex, hFlip, vFlip, mask0);
}

Uint8 _packSpriteSize_PPU(Uint8 sizeX, Uint8 sizeY) {
	if (sizeX == 1 && sizeY == 1) return 0;
	if (sizeX == 2 && sizeY == 2) return 1;
	if (sizeX == 4 && sizeY == 4) return 2;
	if (sizeX == 8 && sizeY == 8) return 3;
	if (sizeX == 2 && sizeY == 4) return 4;
	if (sizeX == 4 && sizeY == 8) return 5;
	if (sizeX == 4 && sizeY == 2) return 6;
	if (sizeX == 8 && sizeY == 4) return 7;
	return 0;
}

Uint8 _unpackSpriteSize_PPU(Uint8 size, Uint8* sizeX, Uint8* sizeY) {
	if (size == 0) { *sizeX = 1; *sizeY = 1; }
	if (size == 1) { *sizeX = 2; *sizeY = 2; }
	if (size == 2) { *sizeX = 4; *sizeY = 4; }
	if (size == 3) { *sizeX = 8; *sizeY = 8; }
	if (size == 4) { *sizeX = 2; *sizeY = 4; }
	if (size == 5) { *sizeX = 4; *sizeY = 8; }
	if (size == 6) { *sizeX = 4; *sizeY = 2; }
	if (size == 7) { *sizeX = 8; *sizeY = 4; }
	return 0;
}

void _packSpriteControl_PPU(Uint8* control, int x, int y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer) {
	Uint8 yByte;
	SDL_bool invertY;
	if (y >= 0) {
		yByte = y;
		invertY = SDL_FALSE;
	}
	else {
		yByte = -y;
		invertY = SDL_TRUE;
	}

	Uint8 xByte;
	SDL_bool invertX;
	if (x >= 0) {
		xByte = x;
		invertX = SDL_FALSE;
	}
	else {
		xByte = -x;
		invertX = SDL_TRUE;
	}

	Uint8 size = _packSpriteSize_PPU(sizeX, sizeY);

	control[0] = xByte;
	control[1] = yByte;
	control[2] = (invertX ? 0x80 : 0x00) | (invertY ? 0x40 : 0x00) | (layer << 4) | (visible ? 0x08 : 0x00) | (size);
}

void _unpackSpriteControl_PPU(Uint8* control, int* x, int* y, Uint8* sizeX, Uint8* sizeY, SDL_bool* visible, Uint8* layer) {
	*x = control[0];
	if ((control[2] & 0x80) != 0) {
		*x = -*x;
	}
	*y = control[1];
	if ((control[2] & 0x40) != 0) {
		*y = -*y;
	}
	_unpackSpriteSize_PPU(control[2] & 0x07, sizeX, sizeY);
	*visible = (control[2] & 0x08) == 0 ? SDL_FALSE : SDL_TRUE;
	*layer = (control[2] & 0x3) >> 4;
}

void setSpriteControl_PPU(hPPU ppu, Uint8 sprite, int x, int y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer) {
	_packSpriteControl_PPU(ppu->spriteControls + 3 * sprite, x, y, sizeX, sizeY, visible, layer);
}

void _scanBackground_PPU(hPPU ppu, Uint8 layer, hSL sl) {
	Uint8 dX, dY;
	SDL_bool visible, topmost;
	_unpackBgControl_PPU(ppu->bgControls + layer, &dX, &dY, &visible, &topmost);

	if (!visible) {
		return;
	}

	int srcScanLine = getLine_SL(sl) + dY;
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

void _scanSprites_PPU(hPPU ppu, Uint8 layer, hSL sl) {
	Uint8* brush = ppu->spriteBrushes;
	Uint8* control = ppu->spriteControls;

	for (int i = 0; i < 124; i++) {
		int x, y;
		Uint8 sizeX, sizeY, spriteLayer;
		SDL_bool visible;
		_unpackSpriteControl_PPU(control, &x, &y, &sizeX, &sizeY, &visible, &spriteLayer);

		if (visible && spriteLayer == layer) {
			Uint8 glyphIndex, bankIndex, swatchIndex;
			SDL_bool hFlip, vFlip, mask0;
			_unpackBrush_PPU(brush, &glyphIndex, &bankIndex, &swatchIndex, &hFlip, &vFlip, &mask0);

			int localY;
			if (vFlip) {
				localY = (y + 8 * sizeY) - (getLine_SL(sl) + 1);
			}
			else {
				localY = getLine_SL(sl) - y;
			}
			if (localY >= 0 && localY < 8 * sizeY) {
				Uint8 bar = localY % 8;
				glyphIndex += 16 * (localY / 8);

				int destX;
				int dX;
				if (hFlip) {
					destX = x + 8 * (sizeX - 1);
					dX = -8;
				}
				else {
					destX = x;
					dX = 8;
				}

				for (int i = 0; i < sizeX; i++) {
					scanBar_SL(sl, getBar_Mapper(ppu->spriteMapper, bankIndex, glyphIndex, bar),
						    _getSwatch_PPU(ppu->spritePalette, swatchIndex), destX, hFlip, mask0);

					glyphIndex += 1;
					destX += dX;
				}
			}
		}

		brush += 2;
		control += 3;
	}
}

void scan_PPU(hPPU ppu, hBB bb) {
	for (int i = 0; i < 248; i++) {
		hSL sl = creat_SL(bb, i);
		_scanBackground_PPU(ppu, 0, sl);
		_scanSprites_PPU(ppu, 0, sl);
		
		destr_SL(sl);
	}
}