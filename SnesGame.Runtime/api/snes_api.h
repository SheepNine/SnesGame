#pragma once
#include "SDL.h"

typedef enum {
	GP_BUTTON_DU = 1 << 0,
	GP_BUTTON_DR = 1 << 1,
	GP_BUTTON_DD = 1 << 2,
	GP_BUTTON_DL = 1 << 3,
	GP_BUTTON_BX = 1 << 4,
	GP_BUTTON_BA = 1 << 5,
	GP_BUTTON_BB = 1 << 6,
	GP_BUTTON_BY = 1 << 7,
	GP_BUTTON_SL = 1 << 8,
	GP_BUTTON_ST = 1 << 9,
	GP_BUTTON_ZL = 1 << 10,
	GP_BUTTON_ZR = 1 << 11,
} GP_BUTTON;


typedef enum {
	SD_FALLING = -1,
	SD_NONE = 0,
	SD_RISING = 1
} ShiftDir;

typedef enum {
	EB_SILENCE = 0,
	EB_REVERSE = 1,
	EB_LOOP = 2
} EdgeBehaviour;

typedef struct INIT* hINIT;
typedef void (*pInitCallback)(hINIT);
void allocBrushLists(hINIT init, Uint16 count);
void initBrushList(hINIT init, Uint16 brushListIndex, Uint8* data);
void loadBrushList(hINIT init, Uint16 brushListIndex, char* filename);

typedef struct UPDATE* hUPDATE;
typedef void (*pUpdateCallback)(hUPDATE);
SDL_bool isButtonIn(hUPDATE update, GP_BUTTON button);
SDL_bool isButtonOut(hUPDATE update, GP_BUTTON button);
SDL_bool wasButtonPressed(hUPDATE update, GP_BUTTON button);
SDL_bool wasButtonReleased(hUPDATE update, GP_BUTTON button);

void silenceChannel(hUPDATE update, Uint8 channelIndex);
void playSquareNote(hUPDATE update, Uint8 channelIndex,
	Uint16 length,
	Uint8 volume, enum ShiftDir volumeShift, Uint8 volumeShiftSpeed, enum EdgeBehaviour volumeEB,
	Uint16 periodLow, Uint16 periodHigh, enum ShiftDir periodShift, Uint8 periodShiftSpeed, enum EdgeBehaviour periodEB);


typedef struct RENDER* hRENDER;
typedef void (*pRenderCallback)(hRENDER);
void switchBackdropBrushList(hRENDER render, Uint8 bankIndex, Uint16 brushListIndex);
void setBackdropPaletteColor(hRENDER render, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setBackdropPalette(hRENDER render, Uint8 paletteIndex, Uint8* data);
void setBackdropStroke(hRENDER render, Uint8 backdropIndex, Uint8 x, Uint8 y, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setBackdropControl(hRENDER render, Uint8 backdropIndex, Uint8 dX, Uint8 dY, SDL_bool visible, SDL_bool topmost);
void switchActorBrushList(hRENDER render, Uint8 bankIndex, Uint16 brushListIndex);
void setActorPaletteColor(hRENDER render, Uint8 paletteIndex, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);
void setActorPalette(hRENDER render, Uint8 paletteIndex, Uint8* data);
void setActorStroke(hRENDER render, Uint8 actorIndex, Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);
void setActorControl(hRENDER render, Uint8 actorIndex, Sint16 x, Sint16 y, Uint8 sizeX, Uint8 sizeY, SDL_bool visible, Uint8 layer);
void setFullLayerClips(hRENDER render, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight, SDL_bool invertLeft, SDL_bool invertRight, SDL_bool invertTop, SDL_bool invertBottom);
void setLayerClips(hRENDER render, Uint8 layerIndex, Uint8 leftWidth, Uint8 rightWidth, Uint8 topHeight, Uint8 bottomHeight);

typedef void (*pDeinitCallback)();

extern int libMain(char* title, pInitCallback initFunc, pUpdateCallback updateCallback, pRenderCallback renderCallback, pDeinitCallback deinitCallback);