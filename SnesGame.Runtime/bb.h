#pragma once
#include "SDL.h"

typedef struct BB* hBB;

hBB creat_BB();
void destr_BB(hBB bb);

void fill_BB(hBB bb, Uint8 r, Uint8 g, Uint8 b);

Uint32 getDot_BB(hBB bb, int x, int y);
void setDot_BB(hBB bb, int x, int y, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);