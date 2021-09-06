#pragma once
#include "SDL.h"
#include "gamepad.h"


typedef struct PB* hPB;

hPB creat_PB(hGP gp, SDL_RWops* source);
void destr_PB(hPB pb);

void playback_PB(hPB pb);
