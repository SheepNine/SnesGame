#pragma once
#include "SDL.h"

typedef struct Mapper* hMapper;

hMapper creat_Mapper(int numPages);
void destr_Mapper(hMapper mapper);

void loadPage_Mapper(hMapper mapper, int page, Uint8* data);

void switchSpriteBank_Mapper(hMapper mapper, int bank, int page);
void switchBackgroundBank_Mapper(hMapper mapper, int bank, int page);

Uint8* getSpriteBar_Mapper(hMapper mapper, int bank, int glyph, int bar);
Uint8* getBackgroundBar_Mapper(hMapper mapper, int bank, int glyph, int bar);