#pragma once
#include "SDL.h"

typedef struct BB* hBB;

hBB creat_BB();
void destr_BB(hBB bb);

void fill_BB(hBB bb, Uint8 r, Uint8 g, Uint8 b);
void blit_BB(hBB bb, SDL_Surface* surface);


typedef struct SL* hSL;

hSL creat_SL(hBB bb, Uint8 y);
void destr_SL(hSL sl);

Uint8 getLine_SL(hSL sl);

void setClip_SL(hSL sl, Uint8 leftClip, Uint8 rightClip);

void scanBar_SL(hSL sl, Uint8* bar, Uint8* swatch, int x, SDL_bool hFlip, SDL_bool mask0);


typedef struct BBC* hBBC;

hBBC creat_BBC();
void destr_BBC(hBBC bbc);

void setClip_BBC(hBBC bbc, Uint8 minClip, Uint8 maxClip);
SDL_bool isClipped_BBC(hBBC bbc, int value);