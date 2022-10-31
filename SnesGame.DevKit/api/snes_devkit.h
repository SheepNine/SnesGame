#pragma once
#include "snes_runtime.h"

typedef struct CC* hCC;
hCC creat_CC();
hCC clone_CC(hCC other);
void destr_CC(hCC cs);
void setWorldLocus_CC(hCC cs, Sint32 x, Sint32 y);
void setScreenLocus_CC(hCC cs, Sint32 x, Sint32 y);
void worldToScreen_CC(hCC cs, Sint32 worldX, Sint32 worldY, Sint32* screenX, Sint32* screenY,
    SDL_bool dejitter);
void screenToWorld_CC(hCC cs, Sint32 screenX, Sint32 screenY, Sint32* worldX, Sint32* worldY);

typedef struct WB* hWB;
hWB creat_WB(Uint16 width, Uint16 height, Uint8* data);
void destr_WB(hWB wb);
Uint16 getWidth_WB(hWB wb);
Uint16 getHeight_WB(hWB wb);
void draw_WB(hRENDER render, Uint8 backdropIndex, SDL_bool topmost, hWB wb, Sint16 x, Sint16 y,
    Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0);
void appendToBackdrop_WB(hRENDER render, Uint8 backdropIndex, hWB wb, Sint16 tileX, Sint16 tileY,
    Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0);

typedef struct Dialog* hDialog;
hDialog creatBare_Dialog(char* text, Uint8 height, SDL_bool tallText);
hDialog creatPortrait_Dialog(char* text, hWB wb, SDL_bool portraitOnRight);
void destr_Dialog(hDialog dialog);
SDL_bool isFinished_Dialog(hDialog dialog);
SDL_bool pageDone_Dialog(hDialog dialog);
void poke_Dialog(hDialog dialog);
void update_Dialog(hDialog dialog);
void renderText_Dialog(hDialog dialog, hRENDER render, Uint8 layerIndex,
    Uint8 bankIndex, Uint8 paletteIndex);
void renderBorder_Dialog(hDialog dialog, hRENDER render, Uint8 layerIndex,
    Uint8 bankIndex, Uint8 paletteIndex);

typedef struct Record* hRecord;
hRecord creat_Record(Uint8 trackCount, Uint8* opcodeStream, Uint16* trackOffsets,
    Uint16* trackLengths);
void destr_Record(hRecord rec);

typedef struct Jukebox* hJukebox;
hJukebox creat_Jukebox();
void update_Jukebox(hJukebox jukebox, hUPDATE update);
void silence_Jukebox(hJukebox jukebox, hUPDATE update);
void destr_Jukebox(hJukebox jukebox);
void playSfx_Jukebox(hJukebox jukebox, hRecord recording);
void playBgm_Jukebox(hJukebox jukebox, hRecord recording);
void stopBgm_Jukebox(hJukebox jukebox);
void startBgm_Jukebox(hJukebox jukebox);

void drawBackdropString(hRENDER render, Uint8 backdropIndex, char* string, Uint8 x, Uint8 y,
    Uint8 bankIndex, Uint8 paletteIndex, SDL_bool mask0);
void fillBackdrop(hRENDER render, Uint8 backdropIndex, Uint8 brushIndex,
    Uint8 bankIndex, Uint8 paletteIndex, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);

void setPaletteColor(Uint8* data, Uint8 colorIndex, Uint8 r, Uint8 g, Uint8 b, SDL_bool t);

typedef Uint8(*hSBRC)(hRENDER render, Uint8 actorIndex, Sint32 screenX, Sint32 screenY,
    void* renderContext);

typedef struct SB* hSB;
hSB creat_SB(hCC cc);
void destr_SB(hSB sb);

void addSprite_SB(hSB sb, Sint32 worldX, Sint32 worldY, hSBRC renderCallback,
    void* renderContext);
Uint8 render_SB(hSB sb, hRENDER render, Uint8 actorIndex);
void addSingleBrushSprite_SB(hSB sb, Sint32 worldX, Sint32 worldY, Uint8 layer,
    Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, Sint8 centerX, Sint8 centerY,
    Uint8 sizeX, Uint8 sizeY, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0);