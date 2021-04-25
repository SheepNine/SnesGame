#include "mapper.h"

#define __MAPPER_PAGE_SIZE 8192

struct Mapper {
	Uint8* pages;
	int numPages;
	Uint8* spriteBanks[4];
	Uint8* backgroundBanks[4];
} Mapper;

hMapper creat_Mapper(int numPages) {
	hMapper result = (hMapper)SDL_malloc(sizeof(Mapper));
	result->numPages = numPages;
	result->pages = SDL_malloc(numPages * __MAPPER_PAGE_SIZE);
	result->spriteBanks[0] = result->pages;
	result->spriteBanks[1] = result->pages;
	result->spriteBanks[2] = result->pages;
	result->spriteBanks[3] = result->pages;

	result->backgroundBanks[0] = result->pages;
	result->backgroundBanks[1] = result->pages;
	result->backgroundBanks[2] = result->pages;
	result->backgroundBanks[3] = result->pages;
	return result;
}

void destr_Mapper(hMapper mapper) {
	SDL_free(mapper->pages);
	SDL_free(mapper);
}

void loadPage_Mapper(hMapper mapper, int page, Uint8* data) {
	SDL_memcpy(mapper->pages + __MAPPER_PAGE_SIZE * page, data, __MAPPER_PAGE_SIZE);
}

void switchSpriteBank_Mapper(hMapper mapper, int bank, int page) {
	mapper->spriteBanks[bank] = mapper->pages + page * __MAPPER_PAGE_SIZE;
}
void switchBackgroundBank_Mapper(hMapper mapper, int bank, int page) {
	mapper->backgroundBanks[bank] = mapper->pages + page * __MAPPER_PAGE_SIZE;
}

Uint8* getSpriteBar_Mapper(hMapper mapper, int bank, int glyph, int bar) {
	return mapper->spriteBanks[bank] + 4 * (8 * glyph + bar);
}
Uint8* getBackgroundBar_Mapper(hMapper mapper, int bank, int glyph, int bar) {
	return mapper->backgroundBanks[bank] + 4 * (8 * glyph + bar);
}
