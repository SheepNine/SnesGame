#pragma once
#include "SDL.h"

enum ShiftDir { SD_FALLING = -1, SD_NONE = 0, SD_RISING = 1 };
enum EdgeBehaviour { EB_SILENCE = 0, EB_REVERSE = 1, EB_LOOP = 2 };

typedef struct SC* hSC;

hSC creat_SC();
void destr_SC(hSC sc);

void playNote_SC(hSC sc,
	Uint16 length,
	Uint8 volume, enum ShiftDir volumeShift, Uint8 volumeShiftSpeed, enum EdgeBehaviour volumeEB,
	Uint16 periodLow, Uint16 periodHigh, enum ShiftDir periodShift, Uint8 periodShiftSpeed, enum EdgeBehaviour periodEB);

void silence_SC(hSC sc);

Sint16 getNextSample_SC(hSC sc);