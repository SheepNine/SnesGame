#pragma once
#include "SDL.h"

typedef struct SW* hSW;

hSW creat_SW();
void destr_SW(hSW sw);

void setTargetVolume_SW(hSW sw, Sint8 volume);
Sint16 getNextSample_SW(hSW sw);
SDL_bool isSilent_SW(hSW sw);