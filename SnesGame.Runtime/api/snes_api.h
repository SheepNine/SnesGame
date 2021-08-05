#pragma once
#include "SDL.h"

#define PITCH_A0 873
#define PITCH_ASHARP0 824 
#define PITCH_B0 778 
#define PITCH_C1 734
#define PITCH_CSHARP1 693
#define PITCH_D1 654
#define PITCH_DSHARP1 617
#define PITCH_E1 582
#define PITCH_F1 550
#define PITCH_FSHARP1 519
#define PITCH_G1 490
#define PITCH_GSHARP1 462
#define PITCH_A1 436
#define PITCH_ASHARP1 412
#define PITCH_B1 389
#define PITCH_C2 367
#define PITCH_CSHARP2 346
#define PITCH_D2 327
#define PITCH_DSHARP2 309
#define PITCH_E2 291
#define PITCH_F2 275
#define PITCH_FSHARP2 259
#define PITCH_G2 245 
#define PITCH_GSHARP2 231
#define PITCH_A2 218 
#define PITCH_ASHARP2 206 
#define PITCH_B2 194 
#define PITCH_C3 183 
#define PITCH_CSHARP3 173
#define PITCH_D3 163 
#define PITCH_DSHARP3 154 
#define PITCH_E3 146 
#define PITCH_F3 137 
#define PITCH_FSHARP3 130 
#define PITCH_G3 122 
#define PITCH_GSHARP3 116 
#define PITCH_A3 109 
#define PITCH_ASHARP3 103 
#define PITCH_B3 97 
#define PITCH_C4 92 
#define PITCH_CSHARP4 87 
#define PITCH_D4 82 
#define PITCH_DSHARP4 77 
#define PITCH_E4 73 
#define PITCH_F4 69 
#define PITCH_FSHARP4 65
#define PITCH_G4 61 
#define PITCH_GSHARP4 58 
#define PITCH_A4 55 
#define PITCH_ASHARP4 51 
#define PITCH_B4 49 
#define PITCH_C5 46 
#define PITCH_CSHARP5 43
#define PITCH_D5 41 
#define PITCH_DSHARP5 39 
#define PITCH_E5 36
#define PITCH_F5 34
#define PITCH_FSHARP5 32
#define PITCH_G5 31
#define PITCH_GSHARP5 29
#define PITCH_A5 27
#define PITCH_ASHARP5 26
#define PITCH_B5 24
#define PITCH_C6 23
#define PITCH_CSHARP6 22
#define PITCH_D6 20
#define PITCH_DSHARP6 19
#define PITCH_E6 18
#define PITCH_F6 17
#define PITCH_FSHARP6 16

typedef enum {
	GP_BUTTON_DU = 0,
	GP_BUTTON_DR = 1,
	GP_BUTTON_DD = 2,
	GP_BUTTON_DL = 3,
	GP_BUTTON_BX = 4,
	GP_BUTTON_BA = 5,
	GP_BUTTON_BB = 6,
	GP_BUTTON_BY = 7,
	GP_BUTTON_SL = 8,
	GP_BUTTON_ST = 9,
	GP_BUTTON_ZL = 10,
	GP_BUTTON_ZR = 11,
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

typedef struct {
	ShiftDir dir;
	Uint8 speed;
	EdgeBehaviour edgeBehaviour;
} ShiftParams;

typedef struct {
	Uint16 length;
	Uint8 volume;
	Uint16 periodLow;
	Uint16 periodHigh;
	ShiftParams volumeShift;
	ShiftParams periodShift;
} SquareWaveParams;

typedef enum {
	CV_SUSTAIN,
	CV_SILENCE,
	CV_SQUARE
} ChannelVoiceType;

typedef struct {
	ChannelVoiceType type;
	union {
		SquareWaveParams square;
	} waveParams;
} ChannelVoice;

void silenceChannel(hUPDATE update, Uint8 channelIndex);
void playSquareNote(hUPDATE update, Uint8 channelIndex, SquareWaveParams* params);


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