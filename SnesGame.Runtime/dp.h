#pragma once
#include "SDL.h"
#include "gamepad.h"


typedef struct DP* hDP;

hDP creat_DP(hGP gp, SDL_RWops* source);
void destr_DP(hDP pb);

void playback_DP(hDP pb);
