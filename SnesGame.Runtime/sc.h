#pragma once
#include "SDL.h"
#include "api/snes_runtime_enums.h"

typedef struct SC* hSC;

hSC creat_SC();
void destr_SC(hSC sc);

void playNote_SC(hSC sc,
	Uint16 length,
	Uint8 volumeLow, Uint8 volumeHigh, ShiftDir volumeShift, Uint8 volumeShiftSpeed, EdgeBehaviour volumeEB,
	Uint16 periodLow, Uint16 periodHigh, ShiftDir periodShift, Uint8 periodShiftSpeed, EdgeBehaviour periodEB);

void playNoise_SC(hSC sc, Uint16 initialRegister, Uint16 tapBit, Uint16 maxLength, Uint8 period);

void silence_SC(hSC sc);

Sint16 getNextSample_SC(hSC sc);