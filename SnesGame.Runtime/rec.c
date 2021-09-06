#include "rec.h"

struct REC {
	hGP gp;
	Uint16 currentValue;
	Uint16 currentCount;
	SDL_RWops* RW;
} REC;

hREC creat_REC(hGP gp, char* filename) {
	hREC result = (hREC)SDL_malloc(sizeof(REC));
	result->gp = gp;
	result->currentValue = 0;
	result->currentCount = 0;
	if (filename != NULL) {
		result->RW = SDL_RWFromFile(filename, "w");
		// TODO: add some sort of header
	}
	return result;
}

void emitRun_REC(hREC rec) {
	if (rec->RW == NULL || rec->currentCount == 0) {
		return;
	}

	Uint8 data[3];
	data[0] = (Uint8)(rec->currentValue >> 4);
	data[1] = rec->currentValue & 0xFF;

	if (rec->currentCount < 0x8) {
		data[0] |= rec->currentCount << 4;
		SDL_RWwrite(rec->RW, data, 1, 2);
	}
	else {
		data[0] |= 0x80;
		data[0] |= 0x70 & (rec->currentCount << 4);
		data[2] = (Uint8)(rec->currentCount >> 3);
		SDL_RWwrite(rec->RW, data, 1, 3);
	}

	rec->currentCount = 0;
}

void destr_REC(hREC rec) {
	emitRun_REC(rec);
	if (rec->RW != NULL) {
		SDL_RWclose(rec->RW);
	}
	SDL_free(rec);
}

void record_REC(hREC rec) {
	Uint16 nextValue = getCurrent_GP(rec->gp);
	if (rec->currentCount == 0x7FF || rec->currentValue != nextValue) {
		emitRun_REC(rec);
	}
	rec->currentValue = nextValue;
	rec->currentCount += 1;
}
