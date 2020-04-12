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