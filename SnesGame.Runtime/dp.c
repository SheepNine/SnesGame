#include "dp.h"

struct DP {
	hGP gp;
	Uint16 currentValue;
	Uint16 currentCount;
	SDL_RWops* RW;
} DP;

hDP creat_DP(hGP gp, SDL_RWops* source) {
	hDP result = (hDP)SDL_malloc(sizeof(DP));
	result->currentCount = 0;
	result->currentValue = 0;
	result->gp = gp;
	result->RW = source;
	// TODO: read/parse header
	return result;
}

void destr_DP(hDP pb) {
	if (pb->RW != NULL) {
		SDL_RWclose(pb->RW);
	}
	SDL_free(pb);
}

void consume_PB(hDP pb) {
	Uint8 data[2];

	size_t count = SDL_RWread(pb->RW, data, 1, 2);
	if (count < 2) {
		pb->currentCount = 1;
		pb->currentValue = 0;
		SDL_RWclose(pb->RW);
		pb->RW = NULL;
		return;
	}

	pb->currentCount = (data[0] & 0x70) >> 4;
	pb->currentValue = ((data[0] & 0x0F) << 8) | data[1];

	if (data[0] & 0x80) {
		count = SDL_RWread(pb->RW, data, 1, 1);
		if (count == 0) {
			// NOT EXPECTED; CORRUPT STREAM
			pb->currentCount = 1;
			pb->currentValue = 0;
			SDL_RWclose(pb->RW);
			pb->RW = NULL;
			return;
		}

		pb->currentCount |= (data[0] << 3);
	}

	//SDL_Log("Value %X, count %i", (unsigned int)pb->currentValue, (int)pb->currentCount);
}

void playback_DP(hDP pb) {
	if (pb->RW == NULL) {
		return;
	}
	if (pb->currentCount == 0) {
		consume_PB(pb);
	}
	setCurrent_GP(pb->gp, pb->currentValue);
	pb->currentCount -= 1;
}
