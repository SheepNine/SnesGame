#pragma once
#include "snes_api.h"

typedef struct CS* hCS;
hCS creat_CS();
void destr_CS(hCS cs);
void setWorldLocus_CS(hCS cs, Sint32 x, Sint32 y);
void setScreenLocus_CS(hCS cs, Sint32 x, Sint32 y);
void worldToScreen_CS(hCS cs, Sint32 worldX, Sint32 worldY, Sint32* screenX, Sint32* screenY);
void screenToWorld_CS(hCS cs, Sint32 screenX, Sint32 screenY, Sint32* worldX, Sint32* worldY);

typedef struct WB* hWB;
hWB creat_WB(Uint16 width, Uint16 height, Uint8* data);
void destr_WB(hWB wb);
Uint16 getWidth_WB(hWB wb);
Uint16 getHeight_WB(hWB wb);
void draw_WB(hRENDER render, Uint8 backdropIndex, SDL_bool topmost, hWB wb, Sint16 x, Sint16 y, Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0);
void appendToBackdrop_WB(hRENDER render, Uint8 backdropIndex, hWB wb, Sint16 tileX, Sint16 tileY, Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0);

void drawBackdropString(hRENDER render, Uint8 backdropIndex, char* string, Uint8 x, Uint8 y, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool mask0);
void fillBackdrop(hRENDER render, Uint8 backdropIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);

void setPaletteColor(Uint8* data, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);