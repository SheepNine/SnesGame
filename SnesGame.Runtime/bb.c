#include "bb.h"

#define __BB_DIM 248

struct BB {
	Uint8 r[__BB_DIM * __BB_DIM];
	Uint8 g[__BB_DIM * __BB_DIM];
	Uint8 b[__BB_DIM * __BB_DIM];
	SDL_bool crtMode;
} BB;


hBB creat_BB() {
	hBB result = (hBB)SDL_malloc(sizeof(BB));
	fill_BB(result, 0, 0, 0);
	result->crtMode = SDL_FALSE;
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

void toggleCrtMode_BB(hBB bb) {
	bb->crtMode = (bb->crtMode == SDL_TRUE) ? SDL_FALSE : SDL_TRUE;
}

void blit_BB(hBB bb, SDL_Surface* surface) {
	int scale = SDL_min(surface->w, surface->h) / __BB_DIM;
	if (scale < 1) {
		return;
	}

	if (scale == 3 && bb->crtMode == SDL_TRUE) {
		int xSkip = (surface->w - scale * __BB_DIM) / 2;
		int ySkip = (surface->h - scale * __BB_DIM) / 2;

		Uint32* dest = (Uint32*)surface->pixels + ySkip * surface->pitch / 4 + xSkip;

		Uint8* readR = bb->r;
		Uint8* readG = bb->g;
		Uint8* readB = bb->b;

		for (int y = 0; y < __BB_DIM; y++) {
			for (int v = 0; v < scale; v++) {
				Uint8 zero = 0;
				Uint8* yRPrev = &zero;
				Uint8* yGPrev = &zero;
				Uint8* yBPrev = &zero;
				Uint8* yR = readR;
				Uint8* yG = readG;
				Uint8* yB = readB;
				Uint32* writPtr = dest;
				for (int x = 0; x < __BB_DIM * scale; x++) {
					if (x == __BB_DIM * scale - 1) {
						yR = &zero;
						yG = &zero;
						yB = &zero;
					}
					Uint8 r, g, b;
					if (x % scale == 0) {
						r = ((*yRPrev) * 1 + (*yR) * 2) / 3;
						g = ((*yGPrev) * 1 + (*yG) * 2) / 3;
						b = ((*yBPrev) * 1 + (*yB) * 2) / 3;
					}
					else if (x % scale == 1) {
						r = *yR;
						g = *yG;
						b = *yB;
						yRPrev = yR;
						yGPrev = yG;
						yBPrev = yB;
						yR++;
						yG++;
						yB++;
					}
					else {
						r = ((*yRPrev) * 2 + (*yR) * 1) / 3;
						g = ((*yGPrev) * 2 + (*yG) * 1) / 3;
						b = ((*yBPrev) * 2 + (*yB) * 1) / 3;
					}
					if (v == 1) {
						*(writPtr++) = (r << 16) | (g << 8) | b;
					}
					else {
						*(writPtr++) = ((r >> 1) << 16) | ((g >> 1) << 8) | (b >> 1);
					}
				}
				dest += surface->pitch / 4;
			}
			readR += __BB_DIM;
			readG += __BB_DIM;
			readB += __BB_DIM;
		}
	}
	else if (scale == 4 && bb->crtMode == SDL_TRUE) {
		int xSkip = (surface->w - scale * __BB_DIM) / 2;
		int ySkip = (surface->h - scale * __BB_DIM) / 2;

		Uint32* dest = (Uint32*)surface->pixels + ySkip * surface->pitch / 4 + xSkip;

		Uint8* readR = bb->r;
		Uint8* readG = bb->g;
		Uint8* readB = bb->b;

		for (int y = 0; y < __BB_DIM; y++) {
			for (int v = 0; v < scale; v++) {
				Uint8 zero = 0;
				Uint8* yRPrev = &zero;
				Uint8* yGPrev = &zero;
				Uint8* yBPrev = &zero;
				Uint8* yR = readR;
				Uint8* yG = readG;
				Uint8* yB = readB;
				Uint32* writPtr = dest;
				for (int x = 0; x < __BB_DIM * scale; x++) {
					if (x == __BB_DIM * scale - 1) {
						yR = &zero;
						yG = &zero;
						yB = &zero;
					}
					Uint8 r, g, b;
					if (x % scale == 0) {
						r = ((*yRPrev) * 1 + (*yR) * 2) / 3;
						g = ((*yGPrev) * 1 + (*yG) * 2) / 3;
						b = ((*yBPrev) * 1 + (*yB) * 2) / 3;
					}
					else if (x % scale == 1) {
						r = *yR;
						g = *yG;
						b = *yB;
					} else if (x % scale == 2) {
						r = *yR;
						g = *yG;
						b = *yB;
						yRPrev = yR;
						yGPrev = yG;
						yBPrev = yB;
						yR++;
						yG++;
						yB++;
					}
					else {
						r = ((*yRPrev) * 2 + (*yR) * 1) / 3;
						g = ((*yGPrev) * 2 + (*yG) * 1) / 3;
						b = ((*yBPrev) * 2 + (*yB) * 1) / 3;
					}
					if (v == 1 || v == 2) {
						*(writPtr++) = (r << 16) | (g << 8) | b;
					}
					else {
						*(writPtr++) = ((r >> 1) << 16) | ((g >> 1) << 8) | (b >> 1);
					}
				}
				dest += surface->pitch / 4;
			}
			readR += __BB_DIM;
			readG += __BB_DIM;
			readB += __BB_DIM;
		}
	}
	else {
		int xSkip = (surface->w - scale * __BB_DIM) / 2;
		int ySkip = (surface->h - scale * __BB_DIM) / 2;

		Uint32* dest = (Uint32*)surface->pixels + ySkip * surface->pitch / 4 + xSkip;

		Uint8* readR = bb->r;
		Uint8* readG = bb->g;
		Uint8* readB = bb->b;

		for (int y = 0; y < __BB_DIM; y++) {
			for (int v = 0; v < scale; v++) {
				Uint8* yR = readR;
				Uint8* yG = readG;
				Uint8* yB = readB;
				Uint32* writPtr = dest;
				for (int x = 0; x < __BB_DIM; x++) {
					Uint32 color = ((*yR) << 16) | ((*yG) << 8) | (*yB);
					for (int u = 0; u < scale; u++) {
						*(writPtr++) = color;
					}
					yR++;
					yG++;
					yB++;
				}
				dest += surface->pitch / 4;
			}
			readR += __BB_DIM;
			readG += __BB_DIM;
			readB += __BB_DIM;
		}
	}
}



struct BBC {
	Uint8 minX;
	Uint8 maxX;
} BBC;


hBBC creat_BBC() {
	hBBC result = (hBBC)SDL_malloc(sizeof(BBC));
	result->minX = 0;
	result->maxX = __BB_DIM - 1;
	return result;
}

void destr_BBC(hBBC bbc) {
	SDL_free(bbc);
}

void setClip_BBC(hBBC bbc, Uint8 minClip, Uint8 maxClip) {
	bbc->minX = 0;
	bbc->maxX = __BB_DIM - 1;

	if ((minClip & 0x80) == 0) {
		bbc->minX = SDL_max(bbc->minX, minClip & 0x7F);
	}
	else {
		bbc->maxX = SDL_min(bbc->maxX, minClip & 0x7F);
	}

	if ((maxClip & 0x80) == 0) {
		bbc->maxX = SDL_min(bbc->maxX, __BB_DIM - 1 - (maxClip & 0x7F));
	}
	else {
		bbc->minX = SDL_max(bbc->minX, __BB_DIM - 1 - (maxClip & 0x7F));
	}
}

SDL_bool isClipped_BBC(hBBC bbc, int value) {
	return value < bbc->minX || bbc->maxX < value;
}



struct SL {
	Uint8* r;
	Uint8* g;
	Uint8* b;
	Uint8 line;
	hBBC bbc;
} SL;


hSL creat_SL(hBB bb, Uint8 line) {
	SDL_assert(line < __BB_DIM);

	hSL result = (hSL)SDL_malloc(sizeof(SL));
	result->r = bb->r + __BB_DIM * line;
	result->g = bb->g + __BB_DIM * line;
	result->b = bb->b + __BB_DIM * line;
	result->line = line;
	result->bbc = creat_BBC();
	return result;
}

void destr_SL(hSL sl) {
	SDL_free(sl->bbc);
	SDL_free(sl);
}

Uint8 getLine_SL(hSL sl) {
	return sl->line;
}

void setClip_SL(hSL sl, Uint8 leftClip, Uint8 rightClip) {
	setClip_BBC(sl->bbc, leftClip, rightClip);
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

	sl->r[x] = (r >> 1) + ((a ? sl->r[x] : r) >> 1);
	sl->g[x] = (g >> 1) + ((a ? sl->g[x] : g) >> 1);
	sl->b[x] = (b >> 1) + ((a ? sl->b[x] : b) >> 1);
}

void scanBar_SL(hSL sl, Uint8* bar, Uint8* swatch, int x, SDL_bool hFlip, SDL_bool mask0) {
	int dX = 1;
	if (hFlip) {
		x += 7;
		dX = -1;
	}

	Uint8 mask = 0x01;
	for (int i = 0; i < 8; i++) {
		if (!isClipped_BBC(sl->bbc, x)) {
			Uint8 colorIndex = 0;
			if ((bar[0] & mask) != 0) colorIndex |= 1;
			if ((bar[1] & mask) != 0) colorIndex |= 2;
			if ((bar[2] & mask) != 0) colorIndex |= 4;
			if ((bar[3] & mask) != 0) colorIndex |= 8;

			if (!mask0 || colorIndex != 0) {
				_setDot_SL(sl, x, colorIndex, swatch);
			}
		}
		mask <<= 1;
		x += dX;
	}
}
