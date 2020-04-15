#pragma once
#include "SDL.h"

typedef struct Mapper* hMapper;

hMapper creat_Mapper(int numPages);
void destr_Mapper(hMapper mapper);

void loadPage_Mapper(hMapper mapper, int page, Uint8* data);

void switchBank_Mapper(hMapper mapper, int bank, int page);

Uint8* getBar_Mapper(hMapper mapper, int bank, int glyph, int bar);
