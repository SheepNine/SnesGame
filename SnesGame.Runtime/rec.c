#include "rec.h"

struct REC {
	hGP gp;
	Uint16 currentValue;
	Uint8 currentCount;
} REC;

hREC creat_REC(hGP gp, char* filename) {
	filename; // TEMPORARY BUILD FIX
	hREC result = (hREC)SDL_malloc(sizeof(REC));
	result->gp = gp;
	result->currentValue = 0;
	result->currentCount = 0;
	return result;
}

void emitRun_REC(hREC rec) {
	if (rec->currentCount == 0) {
		return;
	}

	// Write it out

	rec->currentCount = 0;
}

void destr_REC(hREC rec) {
	emitRun_REC(rec);
	SDL_free(rec);

}

void record_REC(hREC rec) {
	Uint16 nextValue = getCurrent_GP(rec->gp);
	if (rec->currentCount == 127 || rec->currentValue != nextValue) {
		emitRun_REC(rec);
	}
	rec->currentValue = nextValue;
	rec->currentCount += 1;
}
