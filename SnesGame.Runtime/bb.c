#include "bb.h"

#define __BB_DIM 248

struct BB {
	Uint8 r[__BB_DIM * __BB_DIM];
	Uint8 g[__BB_DIM * __BB_DIM];
	Uint8 b[__BB_DIM * __BB_DIM];
} BB;


hBB creat_BB() {
	hBB result = (hBB)SDL_malloc(sizeof(BB));
	fill_BB(result, 0, 0, 0);
	return result;
}

void destr_BB(hBB bb) {
	SDL_free(bb);
}

void fill_BB(hBB bb, Uint8 r, Uint8 g, Uint8 b) {
	SDL_memset(bb->r, r, __BB_DIM * __BB_DIM);
	SDL_memset(bb->g, g, __BB_DIM * __BB_DIM);
	SDL_memset(bb->b, b, __BB_DIM * __BB_DIM);
}

Uint32 getDot_BB(hBB bb, int x, int y) {
	int i = (y * 248) + x;
	return (bb->r[i] << 16) | (bb->g[i] << 8) | (bb->b[i]);
}

void setDot_BB(hBB bb, int x, int y, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	int i = (y * __BB_DIM) + x;
	bb->r[i] = (r >> 1) + ((t ? bb->r[i] : r) >> 1);
	bb->g[i] = (g >> 1) + ((t ? bb->g[i] : g) >> 1);
	bb->b[i] = (b >> 1) + ((t ? bb->b[i] : b) >> 1);
}



struct SL {
	Uint8* r;
	Uint8* g;
	Uint8* b;
	int minX;
	int maxX;
} SL;

hSL creat_SL(hBB bb, int y) {
	hSL result = (hSL)SDL_malloc(sizeof(SL));
	result->r = bb->r + __BB_DIM * y;
	result->g = bb->g + __BB_DIM * y;
	result->b = bb->b + __BB_DIM * y;
	result->minX = 0;
	result->maxX = __BB_DIM - 1;
	return result;
}

destr_SL(hSL sl) {
	SDL_free(sl);
}

void setClip_SL(hSL sl, Uint8 leftClip, Uint8 rightClip) {
	sl->minX = 0;
	sl->maxX = __BB_DIM - 1;

	if ((leftClip & 0x80) == 0) {
		sl->minX = SDL_max(sl->minX, leftClip & 0x7F);
	}
	else {
		sl->maxX = SDL_min(sl->maxX, leftClip & 0x7F);
	}

	if ((rightClip & 0x80) == 0) {
		sl->maxX = SDL_min(sl->maxX, __BB_DIM - 1 - (rightClip & 0x7F));
	}
	else {
		sl->minX = SDL_max(sl->minX, __BB_DIM - 1 - (rightClip & 0x7F));
	}
}


void setDot_SL(hSL sl, int x, Uint8 r, Uint8 g, Uint8 b, SDL_bool t) {
	if (x < sl->minX || x > sl->maxX)
		return;

	sl->r[x] = (r >> 1) + ((t ? sl->r[x] : r) >> 1);
	sl->g[x] = (g >> 1) + ((t ? sl->g[x] : g) >> 1);
	sl->b[x] = (b >> 1) + ((t ? sl->b[x] : b) >> 1);
}

void _setDot_SL(hSL sl, int x, Uint8 colorIndex, Uint8* swatch) {
	Uint8 colorLow = swatch[colorIndex * 2];
	Uint8 colorHigh = swatch[colorIndex * 2 + 1];

	SDL_bool a = (colorHigh & 0x4) != 0;
	Uint8 r = colorHigh & 0xF8;
	colorHigh <<= 6;
	Uint8 b = colorLow & 0xF8;
	colorLow <<= 3;
	Uint8 g = (colorHigh & 0xC0) | (colorLow & 0x38);

	setDot_SL(sl, x, r, g, b, a);
}

void scanBar_SL(hSL sl, Uint8* bar, Uint8* swatch, int x, SDL_bool hFlip, SDL_bool mask0) {
	if (x + 7 < sl->minX || x > sl->maxX)
		return;

	int dX = 1;
	if (hFlip) {
		x += 7;
		dX = -1;
	}

	Uint8 mask = 0x01;
	for (int i = 0; i < 8; i++) {
		Uint8 colorIndex = 0;
		if ((bar[0] & mask) != 0) colorIndex |= 1;
		if ((bar[1] & mask) != 0) colorIndex |= 2;
		if ((bar[2] & mask) != 0) colorIndex |= 4;
		if ((bar[3] & mask) != 0) colorIndex |= 8;

		if (!mask0 || colorIndex != 0) {
			_setDot_SL(sl, x, colorIndex, swatch);
		}


		mask <<= 1;
		x += dX;
	}
}