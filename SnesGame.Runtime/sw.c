#include "sw.h"

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
	result->output = 0;
	result->currVolume = 0;
	result->prevVolume = 0;
	result->waveletTimer = 16;
	return result;
}

void destr_SW(hSW sw) {
	SDL_free(sw);
}

void setTargetVolume_SW(hSW sw, int volume) {
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