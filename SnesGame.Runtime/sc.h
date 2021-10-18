#pragma once
#include "SDL.h"
#include "api/snes_api.h"

#ifndef INTEROP_EXTERN
#define INTEROP_EXTERN
#endif

typedef struct SC* hSC;

INTEROP_EXTERN hSC creat_SC();
INTEROP_EXTERN void destr_SC(hSC sc);

INTEROP_EXTERN void playNote_SC(hSC sc,
	Uint16 length,
	Uint8 volumeLow, Uint8 volumeHigh, ShiftDir volumeShift, Uint8 volumeShiftSpeed, EdgeBehaviour volumeEB,
	Uint16 periodLow, Uint16 periodHigh, ShiftDir periodShift, Uint8 periodShiftSpeed, EdgeBehaviour periodEB);

INTEROP_EXTERN void silence_SC(hSC sc);

INTEROP_EXTERN Sint16 getNextSample_SC(hSC sc);