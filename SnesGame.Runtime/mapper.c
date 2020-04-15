#include "mapper.h"

#define __MAPPER_PAGE_SIZE 8192

struct Mapper {
	Uint8* pages;
	int numPages;
	Uint8* banks[4];
} Mapper;

hMapper creat_Mapper(int numPages) {
	hMapper result = (hMapper)SDL_malloc(sizeof(Mapper));
	result->numPages = numPages;
	result->pages = SDL_malloc(numPages * __MAPPER_PAGE_SIZE);
	result->banks[0] = result->pages;
	result->banks[1] = result->pages;
	result->banks[2] = result->pages;
	result->banks[3] = result->pages;
	return result;
}

void destr_Mapper(hMapper mapper) {
	SDL_free(mapper->pages);
	SDL_free(mapper);
}

void loadPage_Mapper(hMapper mapper, int page, Uint8* data) {
	SDL_memcpy(mapper->pages + __MAPPER_PAGE_SIZE * page, data, __MAPPER_PAGE_SIZE);
}

void switchBank_Mapper(hMapper mapper, int bank, int page) {
	mapper->banks[bank] = mapper->pages + page * __MAPPER_PAGE_SIZE;
}

Uint8* getBar_Mapper(hMapper mapper, int bank, int glyph, int bar) {
	return mapper->banks[bank] + 32 * glyph + 4 * bar;
}
