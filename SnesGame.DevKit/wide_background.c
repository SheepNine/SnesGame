#include "api/snes_devkit.h"

struct WB
{
    Uint16 width;
    Uint16 height;
    Uint8* data;
} WB;

hWB creat_WB(Uint16 width, Uint16 height, Uint8* data)
{
    hWB result = (hWB)SDL_malloc(sizeof(WB));
    result->width = width;
    result->height = height;
    result->data = SDL_malloc(width * height);
    SDL_memcpy(result->data, data, width * height);
    return result;
}

void destr_WB(hWB wb)
{
    SDL_free(wb->data);
    SDL_free(wb);
}

Uint16 getWidth_WB(hWB wb)
{
    return wb->width;
}

Uint16 getHeight_WB(hWB wb)
{
    return wb->height;
}


void draw_WB(hRENDER render, Uint8 backdropIndex, SDL_bool topmost, hWB wb, Sint16 x, Sint16 y,
    Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0)
{
    Uint8 dX = (8 - x) & 0x7;
    Uint8 dY = (8 - y) & 0x7;
    showBackdrop(render, backdropIndex, dX, dY, topmost);

    Sint16 srcX = (-x) >> 3;
    Sint16 srcY = (-y) >> 3;

    for (Uint8 backdropY = 0; backdropY < 32; backdropY++)
    {
        for (Uint8 backdropX = 0; backdropX < 32; backdropX++)
        {
            Uint8 brushIndex;
            if (srcX >= 0 && srcX < wb->width && srcY >= 0 && srcY < wb->height)
            {
                brushIndex = wb->data[wb->width * srcY + srcX];
            }
            else
            {
                brushIndex = 0;
            }
            setBackdropStroke(render, backdropIndex, backdropX, backdropY,
                brushIndex, bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, mask0);
            srcX += 1;
        }
        srcX -= 32;
        srcY += 1;
    }
}

void appendToBackdrop_WB(hRENDER render, Uint8 backdropIndex, hWB wb, Sint16 tileX, Sint16 tileY,
    Uint8 bankIndex, Uint8 paletteIndex, Uint8 mask0)
{
    for (Uint8 y = 0; y < 32; y++)
    {
        Sint16 srcY = (Sint16)y - tileY;
        if (srcY < 0) continue;
        if (srcY >= wb->height) break;

        for (Uint8 x = 0; x < 32; x++)
        {
            Sint16 srcX = (Sint16)x - tileX;
            if (srcX < 0) continue;
            if (srcX >= wb->width) break;

            Uint8 brushIndex = wb->data[wb->width * srcY + srcX];
            setBackdropStroke(render, backdropIndex, x, y,
                brushIndex, bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, mask0);
        }
    }
}
