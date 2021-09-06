#include "pb.h"

struct PB {
	hGP gp;
	Uint16 currentValue;
	Uint16 currentCount;
	SDL_RWops* RW;
} PB;

hPB creat_PB(hGP gp, char* filename) {
	hPB result = (hPB)SDL_malloc(sizeof(PB));
	result->currentCount = 0;
	result->currentValue = 0;
	result->gp = gp;
	if (filename != NULL) {
		result->RW = SDL_RWFromFile(filename, "r");
	}
	return result;
}

void destr_PB(hPB pb) {
	if (pb->RW != NULL) {
		SDL_RWclose(pb->RW);
	}
	SDL_free(pb);
}

void consume_PB(hPB pb) {
	Uint8 data[2];

	size_t count = SDL_RWread(pb->RW, data, 1, 2);
	if (count == 0) {
		pb->currentCount = 1;
		pb->currentValue = 0;
		SDL_RWclose(pb->RW);
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
			return;
		}

		pb->currentValue |= (data[0] << 3);
	}
}

void playback_PB(hPB pb) {
	if (pb->RW == NULL) {
		return;
	}
	if (pb->currentCount == 0) {
		consume_PB(pb);
	}
	setCurrent_GP(pb->gp, pb->currentValue);
	pb->currentCount -= 1;
}
