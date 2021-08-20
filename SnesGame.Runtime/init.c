#include "init.h"

struct INIT {
	hMapper mapper;
} INIT;

hINIT creat_INIT(hMapper mapper) {
	hINIT result = (hINIT)SDL_malloc(sizeof(INIT));
	result->mapper = mapper;
	return result;
}

void destr_INIT(hINIT __) {
	__;
	// Proxy class; doesn't own any of its references
}

void allocBrushLists(hINIT init, Uint16 count) {
	allocPages_Mapper(init->mapper, count);
}

void initBrushList(hINIT init, Uint16 brushListIndex, Uint8* data) {
	loadPage_Mapper(init->mapper, brushListIndex, data);
}

void loadBrushList(hINIT init, Uint16 brushListIndex, char* filename) {
	Uint8 data[8192];
	SDL_memset(data, 0, 8192);
	SDL_RWops* rw = SDL_RWFromFile(filename, "r");
	if (rw != NULL) {
		SDL_RWread(rw, data, 8192, 1);
		SDL_RWclose(rw);
	}
	initBrushList(init, brushListIndex, data);
}