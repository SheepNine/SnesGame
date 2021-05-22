#include "ppu.h"


struct PPU {
	hMapper romMapper;

	Uint8 backdropPalettes[256];
	Uint8 backdropStrokes[8192];
	Uint8 backdropControls[4];

	Uint8 actorPalettes[256];
	Uint8 actorStrokes[248];
	Uint8 actorControls[372];

	Uint8 layerClips[16];
} PPU;

hPPU creat_PPU(hMapper romMapper) {
	hPPU result = (hPPU)SDL_malloc(sizeof(PPU));
	result->romMapper = romMapper;
	SDL_memset(result->backdropPalettes, 0, 256);
	SDL_memset(result->backdropStrokes, 0, 8192);
	SDL_memset(result->backdropControls, 0, 4);
	SDL_memset(result->actorPalettes, 0, 256);
	SDL_memset(result->actorStrokes, 0, 248);
	SDL_memset(result->actorControls, 0, 372);
	SDL_memset(result->layerClips, 0, 16);
	return result;
}

void destr_PPU(hPPU ppu) {
	SDL_free(ppu);
}

void switchBackdropBrushList_PPU(hPPU ppu, Uint8 bankIndex, int brushListIndex) {
	SDL_assert(bankIndex < 4);
	switchBackgroundBank_Mapper(ppu->romMapper, bankIndex, brushListIndex);
}

void switchActorBrushList_PPU(hPPU ppu, Uint8 bankIndex, int brushListIndex) {
	SDL_assert(bankIndex < 4);
	switchSpriteBank_Mapper(ppu->romMapper, bankIndex, brushListIndex);
}

void _packStroke_PPU(Uint8* stroke, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	stroke[0] = (mask0 ? 0x80 : 0x00) | (hFlip ? 0x40 : 0x00) | (vFlip ? 0x20 : 0x00) | (paletteIndex << 2) | (bankIndex);
	stroke[1] = brushIndex;
}

void _unpackStroke_PPU(Uint8* stroke, Uint8* brushIndex, Uint8* bankIndex, Uint8* paletteIndex, SDL_bool* hFlip, SDL_bool* vFlip, SDL_bool* mask0) {
	*mask0 = (stroke[0] & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
	*hFlip = (stroke[0] & 0x40) == 0 ? SDL_FALSE : SDL_TRUE;
	*vFlip = (stroke[0] & 0x20) == 0 ? SDL_FALSE : SDL_TRUE;
	*paletteIndex = (stroke[0] & 0x1C) >> 2;
	*bankIndex = (stroke[0] & 0x3);
	*brushIndex = stroke[1];
}

void _packColor_PPU(Uint8* color, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	color[0] = (b & 0xF8) | ((g >> 3) & 0x07);
	color[1] = (r & 0xF8) | (g >> 6) | (t ? 0x04 : 0x00);
}

void _packBackdropControl_PPU(Uint8* control, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost) {
	control[0] = (visible ? 0x80 : 0x00) | (topmost ? 0x40 : 0x00) | (dX << 3) | (dY);
}

SDL_bool _isBackdropVisible_PPU(Uint8* control) {
	return (*control & 0x80) == 0 ? SDL_FALSE : SDL_TRUE;
}

SDL_bool _isBackdropTopmost_PPU(Uint8* control) {
	return (*control & 0x40) == 0 ? SDL_FALSE : SDL_TRUE;
}

void _unpackBackdropOffset_PPU(Uint8* control, Uint8* dX, Uint8* dY) {
	*dX = (*control & 0x28) >> 3;
	*dY = (*control & 0x07);
}

Uint8* _getPalette_PPU(Uint8* palettes, Uint8 paletteIndex) {
	return palettes + 32 * paletteIndex;
}

void _setPaletteColor_PPU(Uint8* palettes, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	_packColor_PPU(palettes + 32 * paletteIndex + 2 * colorIndex, r, g, b, t);
}

void setBackdropPaletteColor_PPU(hPPU ppu, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	_setPaletteColor_PPU(ppu->backdropPalettes, paletteIndex, colorIndex, r, g, b, t);
}

void setActorPaletteColor_PPU(hPPU ppu, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	_setPaletteColor_PPU(ppu->actorPalettes, paletteIndex, colorIndex, r, g, b, t);
}

void setBackdropStroke_PPU(hPPU ppu, Uint8 backdropIndex, Uint8 x, Uint8 y, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	_packStroke_PPU(ppu->backdropStrokes + 2 * (32 * (32 * backdropIndex + y) + x), brushIndex, bankIndex, paletteIndex, hFlip, vFlip, mask0);
}

void setBackdropControl_PPU(hPPU ppu, Uint8 backdropIndex, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost) {
	_packBackdropControl_PPU(ppu->backdropControls + backdropIndex, dX, dY, visible, topmost);
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

void setFullLayerClips_PPU(hPPU ppu, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight,
	    SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom) {
	_packClips_PPU(ppu->layerClips + 4 * layerIndex, leftWidth, rightWidth, topHeight, bottomHeight,
		invertLeft, invertRight, invertTop, invertBottom);
}

void setLayerClips_PPU(hPPU ppu, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight) {
	setFullLayerClips_PPU(ppu, layerIndex, leftWidth, rightWidth, topHeight, bottomHeight, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE);
}

void setActorStroke_PPU(hPPU ppu, Uint8 actorIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	_packStroke_PPU(ppu->actorStrokes + 2 * actorIndex, brushIndex, bankIndex, paletteIndex, hFlip, vFlip, mask0);
}

Uint8 _packActorSize_PPU(Uint8 sizeX, Uint8 sizeY) {
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

Uint8 _unpackActorSize_PPU(Uint8 size, Uint8* sizeX, Uint8* sizeY) {
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

void _packActorControl_PPU(Uint8* control, int x, int y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer) {
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

	Uint8 size = _packActorSize_PPU(sizeX, sizeY);

	control[0] = xByte;
	control[1] = yByte;
	control[2] = (invertX ? 0x80 : 0x00) | (invertY ? 0x40 : 0x00) | (layer << 4) | (visible ? 0x08 : 0x00) | (size);
}

SDL_bool _isActorVisible_PPU(Uint8* control) {
	return (control[2] & 0x08) == 0 ? SDL_FALSE : SDL_TRUE;
}

Uint8 _unpackActorLayerIndex_PPU(Uint8* control) {
	return (control[2] & 0x3) >> 4;
}


void _unpackActorControl_PPU(Uint8* control, int* x, int* y, Uint8* sizeX, Uint8* sizeY) {
	*x = control[0];
	if ((control[2] & 0x80) != 0) {
		*x = -*x;
	}
	*y = control[1];
	if ((control[2] & 0x40) != 0) {
		*y = -*y;
	}
	_unpackActorSize_PPU(control[2] & 0x07, sizeX, sizeY);
}

void setActorControl_PPU(hPPU ppu, Uint8 actorIndex, int x, int y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer) {
	_packActorControl_PPU(ppu->actorControls + 3 * actorIndex, x, y, sizeX, sizeY, visible, layer);
}

void _scanBackdrop(Uint8* control, Uint8* backdrop, hMapper mapper, Uint8* palettes, hSL sl) {
	Uint8 dX, dY;
	_unpackBackdropOffset_PPU(control, &dX, &dY);

	int srcScanLine = getLine_SL(sl) + dY;
	Uint8* stroke = backdrop + 64 * (srcScanLine / 8);
	int glyphBarIndex = srcScanLine % 8;

	int x = -dX;
	for (int i = 0; i < 32; i++) {
		Uint8 brushIndex, bankIndex, paletteIndex;
		SDL_bool hFlip, vFlip, mask0;
		_unpackStroke_PPU(stroke, &brushIndex, &bankIndex, &paletteIndex, &hFlip, &vFlip, &mask0);

		scanBar_SL(sl, getBackgroundBar_Mapper(mapper, bankIndex, brushIndex, vFlip ? 7 - glyphBarIndex : glyphBarIndex),
			_getPalette_PPU(palettes, paletteIndex), x, hFlip, mask0);

		stroke += 2;
		x += 8;
	}
}

void _scanActor(Uint8* control, Uint8* stroke, hMapper mapper, Uint8* palettes, hSL sl) {
	int x, y;
	Uint8 sizeX, sizeY, brushIndex, bankIndex, paletteIndex;
	SDL_bool hFlip, vFlip, mask0;
	_unpackActorControl_PPU(control, &x, &y, &sizeX, &sizeY);
	_unpackStroke_PPU(stroke, &brushIndex, &bankIndex, &paletteIndex, &hFlip, &vFlip, &mask0);

	int localY;
	if (vFlip) {
		localY = (y + 8 * sizeY) - (getLine_SL(sl) + 1);
	}
	else {
		localY = getLine_SL(sl) - y;
	}
	if (localY >= 0 && localY < 8 * sizeY) {
		Uint8 bar = localY % 8;
		brushIndex += 16 * (localY / 8);

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
			scanBar_SL(sl, getSpriteBar_Mapper(mapper, bankIndex, brushIndex, bar),
				_getPalette_PPU(palettes, paletteIndex), destX, hFlip, mask0);

			brushIndex += 1;
			destX += dX;
		}
	}
}

void _scanActors_PPU(hPPU ppu, Uint8 layerIndex, hSL sl) {
	Uint8* stroke = ppu->actorStrokes;
	Uint8* control = ppu->actorControls;
	for (int i = 0; i < 124; i++) {
		if (_unpackActorLayerIndex_PPU(control) == layerIndex && _isActorVisible_PPU(control)) {
			_scanActor(control, stroke, ppu->romMapper, ppu->actorPalettes, sl);
		}
		stroke += 2;
		control += 3;
	}
}

void _scanBackdrop_PPU(hPPU ppu, Uint8 layerIndex, hSL sl, SDL_bool topmost) {
	Uint8* control = ppu->backdropControls + layerIndex;
	if (_isBackdropTopmost_PPU(control) == topmost && _isBackdropVisible_PPU(control)) {
		_scanBackdrop(control, ppu->backdropStrokes + 2048 * layerIndex, ppu->romMapper, ppu->backdropPalettes, sl);
	}
}

void _scan_PPU(hPPU ppu, hSL sl) {
	hBBC vClip = creat_BBC();
	Uint8* clips = ppu->layerClips;
	for (int layer = 0; layer < 4; layer++) {
		setClip_BBC(vClip, clips[2], clips[3]);
		if (!isClipped_BBC(vClip, getLine_SL(sl))) {
			setClip_SL(sl, clips[0], clips[1]);
			_scanBackdrop_PPU(ppu, layer, sl, SDL_FALSE);
			_scanActors_PPU(ppu, layer, sl);
			_scanBackdrop_PPU(ppu, layer, sl, SDL_TRUE);
		}
		clips += 4;
	}
	destr_BBC(vClip);
}

void scan_PPU(hPPU ppu, hBB bb) {
	for (int i = 0; i < 248; i++) {
		hSL sl = creat_SL(bb, i);
		_scan_PPU(ppu, sl);
		destr_SL(sl);
	}
}