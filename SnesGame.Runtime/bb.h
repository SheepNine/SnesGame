#pragma once
#include "SDL.h"

typedef struct BB* hBB;

hBB creat_BB();
void destr_BB(hBB bb);

void fill_BB(hBB bb, Uint8 r, Uint8 g, Uint8 b);

Uint32 getDot_BB(hBB bb, int x, int y);
void setDot_BB(hBB bb, int x, int y, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);


typedef struct SL* hSL;

hSL creat_SL(hBB bb, int y);
destr_SL(hSL sl);

void setClip_SL(hSL sl, Uint8 leftClip, Uint8 rightClip);

void setDot_SL(hSL sl, int x, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void scanBar_SL(hSL sl, Uint8* bar, Uint8* swatch, int x, SDL_bool hFlip);