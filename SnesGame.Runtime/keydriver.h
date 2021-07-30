#pragma once
#include "SDL.h"
#include "gamepad.h"

typedef struct KD* hKD;

hKD creat_KD(hGP gp);
void destr_KD(hKD kd);

void handleKeyDown_KD(hKD kd, SDL_KeyboardEvent* evt);
void handleKeyUp_KD(hKD kd, SDL_KeyboardEvent* evt);
