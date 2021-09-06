#pragma once
#include "SDL.h"
#include "gamepad.h"


typedef struct PB* hPB;

hPB creat_PB(hGP gp, char* filename);
void destr_PB(hPB pb);

void playback_PB(hPB pb);
