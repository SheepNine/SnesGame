#include "mapper.h"

#define __MAPPER_PAGE_SIZE 8192

struct Mapper {
	Uint8* pages;
	Uint16 numPages;
	Uint8* spriteBanks[4];
	Uint8* backgroundBanks[4];
} Mapper;

hMapper creat_Mapper(Uint16 numPages) {
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


void allocPages_Mapper(hMapper mapper, Uint16 brushListCount) {
	if (brushListCount == mapper->numPages) return;

	Uint8* newPages = SDL_malloc(brushListCount * __MAPPER_PAGE_SIZE);
	SDL_memset(newPages, 0, brushListCount * __MAPPER_PAGE_SIZE);
	SDL_memcpy(newPages, mapper->pages, SDL_min(mapper->numPages, brushListCount) * __MAPPER_PAGE_SIZE);
	SDL_free(mapper->pages);
	mapper->pages = newPages;
	mapper->numPages = brushListCount;
	mapper->spriteBanks[0] = newPages;
	mapper->spriteBanks[1] = newPages;
	mapper->spriteBanks[2] = newPages;
	mapper->spriteBanks[3] = newPages;
	mapper->backgroundBanks[0] = newPages;
	mapper->backgroundBanks[1] = newPages;
	mapper->backgroundBanks[2] = newPages;
	mapper->backgroundBanks[3] = newPages;
}

void loadPage_Mapper(hMapper mapper, Uint16 page, Uint8* data) {
	SDL_memcpy(mapper->pages + __MAPPER_PAGE_SIZE * page, data, __MAPPER_PAGE_SIZE);
}

void switchSpriteBank_Mapper(hMapper mapper, Uint8 bank, Uint16 page) {
	mapper->spriteBanks[bank] = mapper->pages + page * __MAPPER_PAGE_SIZE;
}
void switchBackgroundBank_Mapper(hMapper mapper, Uint8 bank, Uint16 page) {
	mapper->backgroundBanks[bank] = mapper->pages + page * __MAPPER_PAGE_SIZE;
}

Uint8* getSpriteBar_Mapper(hMapper mapper, Uint8 bank, int glyph, int bar) {
	return mapper->spriteBanks[bank] + 4 * (8 * glyph + bar);
}
Uint8* getBackgroundBar_Mapper(hMapper mapper, Uint8 bank, int glyph, int bar) {
	return mapper->backgroundBanks[bank] + 4 * (8 * glyph + bar);
}
