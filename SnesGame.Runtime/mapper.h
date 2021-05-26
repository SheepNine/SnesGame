#pragma once
#include "SDL.h"

typedef struct Mapper* hMapper;

hMapper creat_Mapper(Uint16 numPages);
void destr_Mapper(hMapper mapper);

void allocPages_Mapper(hMapper mapper, Uint16 brushListCount);
void loadPage_Mapper(hMapper mapper, Uint16 brushListIndex, Uint8* data);

void switchSpriteBank_Mapper(hMapper mapper, Uint8 bankIndex, Uint16 brushListIndex);
void switchBackgroundBank_Mapper(hMapper mapper, Uint8 bankIndex, Uint16 brushListIndex);

Uint8* getSpriteBar_Mapper(hMapper mapper, Uint8 bank, int glyph, int bar);
Uint8* getBackgroundBar_Mapper(hMapper mapper, Uint8 bank, int glyph, int bar);