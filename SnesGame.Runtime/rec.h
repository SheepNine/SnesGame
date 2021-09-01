#pragma once
#include "SDL.h"
#include "gamepad.h"


typedef struct REC* hREC;

hREC creat_REC(hGP gp, char* filename);
void destr_REC(hREC rec);

void record_REC(hREC rec);
