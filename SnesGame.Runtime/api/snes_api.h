#pragma once
#include "SDL.h"

typedef struct INIT* hINIT;
typedef void (*pInitCallback)(hINIT);
void allocBrushLists(hINIT init, Uint16 count);
void initBrushList(hINIT init, Uint16 brushListIndex, Uint8* data);
void loadBrushList(hINIT init, Uint16 brushListIndex, char* filename);

typedef struct UPDATE* H_UPDATE;
typedef void (*pUpdateCallback)(H_UPDATE);

typedef struct RENDER* hRENDER;
typedef void (*pRenderCallback)(hRENDER);
void switchBackdropBrushList(hRENDER render, Uint8 bankIndex, Uint16 brushListIndex);
void setBackdropPaletteColor(hRENDER render, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setBackdropStroke(hRENDER render, Uint8 backdropIndex, Uint8 x, Uint8 y, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setBackdropControl(hRENDER render, Uint8 backdropIndex, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost);
void switchActorBrushList(hRENDER render, Uint8 bankIndex, Uint16 brushListIndex);
void setActorPaletteColor(hRENDER render, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setActorStroke(hRENDER render, Uint8 actorIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setActorControl(hRENDER render, Uint8 actorIndex, Sint16 x, Sint16 y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer);
void setFullLayerClips(hRENDER render, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight, SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom);
void setLayerClips(hRENDER render, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight);

extern int libMain(char* title, pInitCallback initFunc, pUpdateCallback updateCallback, pRenderCallback renderCallback);