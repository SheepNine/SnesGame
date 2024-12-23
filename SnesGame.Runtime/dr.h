#pragma once
#include "SDL.h"
#include "gamepad.h"


typedef struct DR* hDR;

hDR creat_DR(hGP gp, SDL_RWops* dest);
void destr_DR(hDR rec);

void record_DR(hDR rec);
