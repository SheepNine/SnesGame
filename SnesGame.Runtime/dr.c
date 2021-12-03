#include "dr.h"

struct DR {
	hGP gp;
	Uint16 currentValue;
	Uint16 currentCount;
	SDL_RWops* RW;
} DR;

hDR creat_DR(hGP gp, SDL_RWops* dest) {
	hDR result = (hDR)SDL_malloc(sizeof(DR));
	result->gp = gp;
	result->currentValue = 0;
	result->currentCount = 0;
	result->RW = dest;
	return result;
}

void emitRun_DR(hDR rec) {
	if (rec->RW == NULL || rec->currentCount == 0) {
		return;
	}

	//SDL_Log("Value %X, count %i", (unsigned int)rec->currentValue, (int)rec->currentCount);

	Uint8 data[3];
	data[0] = (Uint8)(rec->currentValue >> 8);
	data[1] = rec->currentValue & 0xFF;

	if (rec->currentCount < 0x8) {
		data[0] |= rec->currentCount << 4;
		SDL_RWwrite(rec->RW, data, 1, 2);
		// TODO: error handling
	}
	else {
		data[0] |= 0x80;
		data[0] |= 0x70 & (rec->currentCount << 4);
		data[2] = (Uint8)(rec->currentCount >> 3);
		SDL_RWwrite(rec->RW, data, 1, 3);
		// TODO: error handling
	}

	rec->currentCount = 0;
}

void destr_DR(hDR rec) {
	emitRun_DR(rec);
	if (rec->RW != NULL) {
		SDL_RWclose(rec->RW);
	}
	SDL_free(rec);
}

void record_DR(hDR rec) {
	Uint16 nextValue = getCurrent_GP(rec->gp);
	if (rec->currentCount == 0x7FF || rec->currentValue != nextValue) {
		emitRun_DR(rec);
	}
	rec->currentValue = nextValue;
	rec->currentCount += 1;
}
