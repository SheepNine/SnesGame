#include "sc.h"

// -------------------- SW.h

typedef struct SW* hSW;

hSW creat_SW();
void destr_SW(hSW sw);

void halt_SW(hSW sw);

void setTargetVolume_SW(hSW sw, Sint8 volume);
Sint16 getNextSample_SW(hSW sw);
SDL_bool isSilent_SW(hSW sw);

// -------------------- SW.c


/*
Half-periods
	1	2	3	4	5	6
C		367	183	92	46	23
C#		346	173	87	43	22
D		327	163	82	41	20
D#		309	154	77	39	19
E		291	146	73	36	18
F		275	137	69	34	17
F#		259	130	65	32	16
G	490	245	122	61	31
G#	462	231	116	58	29
A	436	218	109	55	27
A#	412	206	103	51	26
B	389	194	97	49	24
*/

Sint16 __STEP_DELTAS[17][16] = {
   {     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,      0,      0,      0,      0 },
   {     6,   -13,    16,   -20,    26,   -36,    60,  -256,  -256,    60,   -36,    26,    -20,     16,    -13,      6 },
   {    13,   -27,    32,   -39,    50,   -71,   120,  -512,  -512,   120,   -71,    50,    -39,     32,    -27,     13 },
   {    19,   -40,    48,   -59,    76,  -107,   180,  -768,  -768,   180,  -107,    76,    -59,     48,    -40,     19 },
   {    26,   -55,    64,   -78,   101,  -142,   240, -1024, -1024,   240,  -142,   101,    -78,     64,    -55,     26 },
   {    32,   -68,    80,   -98,   127,  -178,   300, -1280, -1280,   300,  -178,   127,    -98,     80,    -68,     32 },
   {    38,   -81,    96,  -117,   151,  -213,   360, -1536, -1536,   360,  -213,   151,   -117,     96,    -81,     38 },
   {    45,   -95,   112,  -137,   177,  -249,   420, -1792, -1792,   420,  -249,   177,   -137,    112,    -95,     45 },
   {    51,  -108,   128,  -157,   202,  -284,   480, -2048, -2048,   480,  -284,   202,   -157,    128,   -108,     51 },
   {    58,  -122,   144,  -177,   228,  -320,   540, -2304, -2304,   540,  -320,   228,   -177,    144,   -122,     58 },
   {    64,  -136,   161,  -196,   252,  -355,   600, -2560, -2560,   600,  -355,   252,   -196,    161,   -136,     64 },
   {    70,  -149,   177,  -216,   278,  -391,   660, -2816, -2816,   660,  -391,   278,   -216,    177,   -149,     70 },
   {    77,  -163,   192,  -235,   303,  -426,   720, -3072, -3072,   720,  -426,   303,   -235,    192,   -163,     77 },
   {    83,  -176,   208,  -255,   329,  -462,   780, -3328, -3328,   780,  -462,   329,   -255,    208,   -176,     83 },
   {    90,  -190,   224,  -274,   353,  -496,   839, -3584, -3584,   839,  -496,   353,   -274,    224,   -190,     90 },
   {    96,  -203,   240,  -294,   379,  -532,   899, -3840, -3840,   899,  -532,   379,   -294,    240,   -203,     96 },
   {   102,  -217,   257,  -314,   404,  -567,   958, -4095, -4095,   958,  -567,   404,   -314,    257,   -217,    102 }
};

struct SW {
	Sint16 output;
	Sint8 currVolume;
	Sint8 prevVolume;
	Uint8 waveletTimer;
} SW;


hSW creat_SW() {
	hSW result = (hSW)SDL_malloc(sizeof(SW));
	halt_SW(result);
	return result;
}

void destr_SW(hSW sw) {
	SDL_free(sw);
}

void halt_SW(hSW sw) {
	sw->output = 0;
	sw->currVolume = 0;
	sw->prevVolume = 0;
	sw->waveletTimer = 16;
}

void setTargetVolume_SW(hSW sw, Sint8 volume) {
	SDL_assert(sw->waveletTimer == 16);
	sw->prevVolume = sw->currVolume;
	sw->currVolume = volume;
	sw->waveletTimer = sw->currVolume == sw->prevVolume ? 16 : 0;
}

Sint16 getNextSample_SW(hSW sw) {
	if (sw->waveletTimer < 16) {
		if (sw->prevVolume > sw->currVolume) {
			sw->output += __STEP_DELTAS[sw->prevVolume - sw->currVolume][sw->waveletTimer++];
		}
		else {
			sw->output -= __STEP_DELTAS[sw->currVolume - sw->prevVolume][sw->waveletTimer++];
		}
	}
	return sw->output;
}

SDL_bool isSilent_SW(hSW sw) {
	return sw->output == 0;
}

// -------------------- SC.c

#define UNLIMITED_LENGTH 0xFFFFFFFF

struct SC {
	Uint8 volumeLow;
	Uint8 volumeHigh;
	enum ShiftDir volumeShift;
	enum EdgeBehaviour volumeEB;
	Uint8 volumeShiftSpeed;
	Uint16 periodLow;
	Uint16 periodHigh;
	enum ShiftDir periodShift;
	Uint8 periodShiftSpeed;
	enum EdgeBehaviour periodEB;

	Uint32 lengthCounter;
	Uint16 periodCounter;
	Uint16 volumeShiftCounter;
	Uint16 periodShiftCounter;
	Uint16 periodCurr;
	Uint8 volumeCurr;
	SDL_bool upperWave;
	hSW sw;

	Uint16 noiseShiftRegister;
	Uint16 noiseShiftTap;
	Uint8 noisePeriod;
} SC;


hSC creat_SC() {
	hSC result = (hSC)SDL_malloc(sizeof(SC));
	result->lengthCounter = UNLIMITED_LENGTH;
	result->volumeCurr = 0;
	result->periodCounter = 0;
	result->sw = creat_SW();
	return result;
}

void destr_SC(hSC sc) {
	destr_SW(sc->sw);
	SDL_free(sc);
}

void playNote_SC(hSC sc,
		Uint16 length,
		Uint8 volumeLow, Uint8 volumeHigh, ShiftDir volumeShift, Uint8 volumeShiftSpeed, EdgeBehaviour volumeEB,
		Uint16 periodLow, Uint16 periodHigh, ShiftDir periodShift, Uint8 periodShiftSpeed, EdgeBehaviour periodEB) {
	SDL_assert(0 < volumeLow && volumeHigh <= 8); // use silence_SC for zero volume
	SDL_assert(0 < volumeHigh && volumeHigh <= 8);
	SDL_assert(volumeLow <= volumeHigh);
	SDL_assert(volumeLow == volumeHigh || volumeShift != SD_NONE);
	SDL_assert(volumeLow != volumeHigh || volumeShift == SD_NONE);
	SDL_assert(16 <= periodLow && periodLow <= 24000);
	SDL_assert(16 <= periodHigh && periodHigh <= 24000);
	SDL_assert(periodLow <= periodHigh);
	SDL_assert(periodLow == periodHigh || periodShift != SD_NONE);
	SDL_assert(periodLow != periodHigh || periodShift == SD_NONE);

	sc->volumeLow = volumeLow;
	sc->volumeHigh = volumeHigh;
	sc->volumeShift = volumeShift;
	sc->volumeShiftSpeed = volumeShiftSpeed;
	sc->volumeEB = volumeEB;

	sc->periodLow = periodLow;
	sc->periodHigh = periodHigh;
	sc->periodShift = periodShift;
	sc->periodShiftSpeed = periodShiftSpeed;
	sc->periodEB = periodEB;

	if (length == 0)
		sc->lengthCounter = UNLIMITED_LENGTH;
	else
		sc->lengthCounter = (Uint32)length * 100;

	sc->volumeShiftCounter = 0;
	sc->periodShiftCounter = 0;
	sc->periodCurr = periodShift == SD_FALLING ? periodHigh : periodLow;
	sc->volumeCurr = volumeShift == SD_FALLING ? volumeHigh : volumeLow;

	sc->noiseShiftRegister = 0;
	sc->noiseShiftTap = 0;
}

void playNoise_SC(hSC sc, Uint16 initialRegister, Uint16 tapBit, Uint16 maxLength, Uint8 period) {
	SDL_assert(initialRegister != 0 && (initialRegister & 0x8000) == 0);
	SDL_assert(tapBit == 0x2 || tapBit == 0x40);
	sc->noiseShiftRegister = initialRegister;
	sc->noiseShiftTap = tapBit;
	if (maxLength == 0)
		sc->lengthCounter = UNLIMITED_LENGTH;
	else
		sc->lengthCounter = maxLength;
	sc->noisePeriod = period;
	sc->periodCounter = period;
	halt_SW(sc->sw);
}

void silence_SC(hSC sc) {
	sc->lengthCounter = UNLIMITED_LENGTH;
	sc->volumeCurr = 0;

	sc->noiseShiftRegister = 0;
	sc->noiseShiftTap = 0;
}

void setupNextVolume(hSC sc) {
	if (sc->volumeShift == SD_NONE) return;
	sc->volumeShiftCounter += 1;

	if (sc->volumeShiftCounter <= sc->volumeShiftSpeed) return;
	sc->volumeShiftCounter = 0;

	if (sc->volumeShift == SD_RISING) {
		if (sc->volumeCurr < sc->volumeHigh) {
			sc->volumeCurr += 1;
		}
		else {
			if (sc->volumeEB == EB_LOOP) {
				sc->volumeCurr = sc->volumeLow;
			}
			else if (sc->volumeEB == EB_REVERSE) {
				sc->volumeShift = SD_FALLING;
				sc->volumeCurr -= 1;
			}
			else if (sc->volumeEB == EB_SILENCE) {
				sc->volumeCurr = 0;
				sc->volumeShift = SD_NONE;
			}
			else if (sc->volumeEB == EB_STEADY) {
				sc->volumeLow = sc->volumeCurr;
				sc->volumeHigh = sc->volumeCurr;
				sc->volumeShift = SD_NONE;
			}
		}
	}
	else {
		if (sc->volumeCurr > sc->volumeLow) {
			sc->volumeCurr -= 1;
		}
		else {
			if (sc->volumeEB == EB_LOOP) {
				sc->volumeCurr = sc->volumeHigh;
			}
			else if (sc->volumeEB == EB_REVERSE) {
				sc->volumeShift = SD_RISING;
				sc->volumeCurr += 1;
			}
			else if (sc->volumeEB == EB_SILENCE) {
				sc->volumeCurr = 0;
				sc->volumeShift = 0;
			}
			else if (sc->volumeEB == EB_STEADY) {
				sc->volumeLow = sc->volumeCurr;
				sc->volumeHigh = sc->volumeCurr;
				sc->volumeShift = SD_NONE;
			}
		}
	}
}

void setupNextPeriod(hSC sc) {
	if (sc->periodShift == SD_NONE) return;
	sc->periodShiftCounter += 1;

	if (sc->periodShiftCounter < sc->periodShiftSpeed) return;
	sc->periodShiftCounter = 0;

	if (sc->periodShift == SD_RISING) {
		if (sc->periodCurr < sc->periodHigh) {
			sc->periodCurr += 1;
		}
		else {
			if (sc->periodEB == EB_LOOP) {
				sc->periodCurr = sc->periodLow;
			}
			else if (sc->periodEB == EB_REVERSE) {
				sc->periodShift = SD_FALLING;
				sc->periodCurr -= 1;
			}
			else if (sc->periodEB == EB_SILENCE) {
				sc->volumeCurr = 0;
				sc->periodShift = SD_NONE;
			}
			else if (sc->periodEB == EB_STEADY) {
				sc->periodLow = sc->periodCurr;
				sc->periodHigh = sc->periodCurr;
				sc->periodShift = SD_NONE;
			}
		}
	}
	else {
		if (sc->periodCurr > sc->periodLow) {
			sc->periodCurr -= 1;
		}
		else {
			if (sc->periodEB == EB_LOOP) {
				sc->periodCurr = sc->periodHigh;

			}
			else if (sc->periodEB == EB_REVERSE) {
				sc->periodShift = SD_RISING;
				sc->periodCurr += 1;
			}
			else if(sc->periodEB == EB_SILENCE) {
				sc->volumeCurr = 0;
				sc->periodShift = SD_NONE;
			}
			else if (sc->periodEB == EB_STEADY) {
				sc->periodLow = sc->periodCurr;
				sc->periodHigh = sc->periodCurr;
				sc->periodShift = SD_NONE;
			}
		}
	}
}

Sint16 getNextSample_SC(hSC sc) {
	if (sc->noiseShiftRegister == 0) {
		if (sc->lengthCounter == 0) {
			sc->volumeCurr = 0;
		}
		else if (sc->lengthCounter != UNLIMITED_LENGTH) {
			sc->lengthCounter -= 1;
		}

		if (sc->periodCounter > 0) {
			sc->periodCounter -= 1;
		}
		else {
			if (sc->volumeCurr == 0) {
				if (!isSilent_SW(sc->sw)) {
					setTargetVolume_SW(sc->sw, 0);
					sc->periodCounter = 16;
				}
			}
			else {
				setTargetVolume_SW(sc->sw, sc->upperWave ? sc->volumeCurr : -sc->volumeCurr);
				sc->periodCounter = sc->periodCurr;
				sc->upperWave = sc->upperWave ? SDL_FALSE : SDL_TRUE;

				setupNextVolume(sc);
				setupNextPeriod(sc);
			}
		}

		return getNextSample_SW(sc->sw);
	}
	else {
		if (sc->lengthCounter == 0) {
			sc->noiseShiftRegister = 0;
			sc->noiseShiftTap = 0;
			return 0;
		}
		else if (sc->lengthCounter != UNLIMITED_LENGTH) {
			sc->lengthCounter -= 1;
		}

		if (sc->periodCounter != 0) {
			sc->periodCounter -= 1;
		}
		else {
			sc->periodCounter = sc->noisePeriod;
			Uint16 tap1 = sc->noiseShiftRegister & sc->noiseShiftTap;
			Uint16 tap2 = sc->noiseShiftRegister & 0x0001;
			if ((tap1 == 0) != (tap2 == 0)) {
				sc->noiseShiftRegister |= 0x8000;
			}
			sc->noiseShiftRegister >>= 1;
		}
		return (sc->noiseShiftRegister - 0x4000) >> 3;
	}
}