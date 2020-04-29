#include "perf.h"

struct Perf {
	Uint64 freqNs;
	Uint64 initTime;
	Uint64 lastTime;
} Perf;

hPerf creat_Perf() {
	hPerf result = (hPerf)SDL_malloc(sizeof(Perf));
	result->freqNs = SDL_GetPerformanceFrequency() / (1000 * 1000);
	result->lastTime = SDL_GetPerformanceCounter();
	result->initTime = result->lastTime;
	return result;
}

void destr_Perf(hPerf perf) {
	SDL_Log("\r\n");
	SDL_free(perf);
}

void logInterval_Perf(hPerf perf, char* label) {
	Uint64 now = SDL_GetPerformanceCounter();
	SDL_Log("%s: %llu ns (%llu ns elapsed)\r\n",
		    label,
		    (now - perf->lastTime) / perf->freqNs,
		    (now - perf->initTime) / perf->freqNs);
	perf->lastTime = now;
}
