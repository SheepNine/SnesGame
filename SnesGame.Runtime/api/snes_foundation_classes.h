#pragma once
#include "snes_api.h"

typedef struct CS* hCS;
hCS creat_CS();
void destr_CS(hCS cs);
void setWorldLocus_CS(hCS cs, Sint32 x, Sint32 y);
void setScreenLocus_CS(hCS cs, Uint8 x, Uint8 y);
void worldToScreen_CS(hCS cs, Sint32 worldX, Sint32 worldY, Sint32* screenX, Sint32* screenY);

void drawBackdropString(hRENDER render, Uint8 backdropIndex, char* string, Uint8 x, Uint8 y, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool mask0);
void fillBackdrop(hRENDER render, Uint8 backdropIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);