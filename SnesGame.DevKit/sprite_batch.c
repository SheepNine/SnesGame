#include "api/snes_devkit.h"

typedef struct Sprite
{
    Sint32 worldX;
    Sint32 worldY;
    Sint32 screenX;
    Sint32 screenY;
    hSBRC renderCallback;
    void* renderContext;
} Sprite;

struct SB
{
    hCC cc;
    Sprite sprites[128];
    Uint8 numSprites;
} SB;

hSB creat_SB(hCC cc)
{
    hSB result = (hSB)SDL_malloc(sizeof(SB));
    result->cc = clone_CC(cc);
    result->numSprites = 0;
    return result;
}

void destr_SB(hSB sb)
{
    destr_CC(sb->cc);
    SDL_free(sb);
}

void addSprite_SB(hSB sb, Sint32 worldX, Sint32 worldY, hSBRC renderCallback, void* renderContext)
{
    if (sb->numSprites == 128) return;

    Sprite* sprite = &sb->sprites[sb->numSprites];
    sprite->worldX = worldX;
    sprite->worldY = worldY;
    worldToScreen_CC(sb->cc, sprite->worldX, sprite->worldY, &sprite->screenX, &sprite->screenY);
    sprite->renderCallback = renderCallback;
    sprite->renderContext = renderContext;

    sb->numSprites += 1;
}

#define parent(i) ((i-1)/2)
#define lChild(i) ((i << 1) + 1)
#define rChild(i) ((i << 1) + 2)

SDL_bool outOfOrder_SB(hSB sb, int a, int b)
{
    Sprite* spriteA = sb->sprites + a;
    Sprite* spriteB = sb->sprites + b;

    if (spriteA->screenY == spriteB->screenY)
    {
        return spriteA->screenX < spriteB->screenX;
    }
    else
    {
        return spriteA->screenY < spriteB->screenY;
    }
}

void swapSprite_SB(hSB sb, int a, int b)
{
    Sprite temp = sb->sprites[a];
    sb->sprites[a] = sb->sprites[b];
    sb->sprites[b] = temp;
}

void siftDown_SB(hSB sb, int index, int lastIndex)
{
    while (lChild(index) <= lastIndex)
    {
        int child = lChild(index);
        int swap = index;

        if (outOfOrder_SB(sb, swap, child))
            swap = child;
        if (child + 1 <= lastIndex && outOfOrder_SB(sb, swap, child + 1))
            swap = child + 1;

        if (swap == index)
            return;

        swapSprite_SB(sb, index, swap);
        index = swap;
    }
}

void prerenderSort_SB(hSB sb)
{
    // heapify
    for (int start = parent(sb->numSprites - 1); start >= 0; start--)
        siftDown_SB(sb, start, sb->numSprites - 1);

    // sort
    for (int end = sb->numSprites - 1; end > 0; end--)
    {
        swapSprite_SB(sb, 0, end);
        siftDown_SB(sb, 0, end - 1);
    }
}

Uint8 render_SB(hSB sb, hRENDER render, Uint8 actorIndex)
{
    prerenderSort_SB(sb);

    Sint32 screenX, screenY;
    for (int i = 0; i < sb->numSprites; i++)
    {
        Sprite* sprite = &sb->sprites[i];
        worldToScreen_CC(sb->cc, sprite->worldX, sprite->worldY, &screenX, &screenY);
        screenX >>= 16;
        screenY >>= 16;
        actorIndex = sprite->renderCallback(render, actorIndex,
            screenX, screenY, sprite->renderContext);
        if (actorIndex == 128)
            break;
    }
    return actorIndex;
}


typedef struct RenderSingleBrushContext
{
    Uint8 layer;
    Uint8 brushIndex;
    Uint8 bankIndex;
    Uint8 paletteIndex;
    Sint8 centerX;
    Sint8 centerY;
    Uint8 sizeX;
    Uint8 sizeY;
    SDL_bool hFlip;
    SDL_bool vFlip;
    SDL_bool mask0;
} RenderSingleBrushContext;

Uint8 renderSingleBrush(hRENDER render, Uint8 actorIndex, Sint32 screenX, Sint32 screenY,
    void* rawContext)
{
    RenderSingleBrushContext* context = rawContext;
    if (screenX - context->centerX < -7 || screenY - context->centerY < -7 ||
        screenX - context->centerX > 248 || screenY - context->centerY > 248)
    {
        return actorIndex;
    }
    showActor(render, actorIndex, screenX - context->centerX, screenY - context->centerY,
        context->sizeX, context->sizeY, context->layer);
    setActorStroke(render, actorIndex, context->brushIndex, context->bankIndex,
        context->paletteIndex, context->hFlip, context->vFlip, context->mask0);
    SDL_free(rawContext);
    return actorIndex + 1;
}

void addSingleBrushSprite_SB(hSB sb, Sint32 worldX, Sint32 worldY, Uint8 layer,
    Uint8 brushIndex, Uint8 bankIndex, Uint8 paletteIndex, Sint8 centerX, Sint8 centerY,
    Uint8 sizeX, Uint8 sizeY, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0)
{
    RenderSingleBrushContext* context
        = (RenderSingleBrushContext*)SDL_malloc(sizeof(RenderSingleBrushContext));
    context->layer = layer;
    context->brushIndex = brushIndex;
    context->bankIndex = bankIndex;
    context->paletteIndex = paletteIndex;
    context->centerX = centerX;
    context->centerY = centerY;
    context->sizeX = sizeX;
    context->sizeY = sizeY;
    context->hFlip = hFlip;
    context->vFlip = vFlip;
    context->mask0 = mask0;
    addSprite_SB(sb, worldX, worldY, renderSingleBrush, context);
}

// ====== TEST for sprite batch sorting ======

typedef struct
{
    int x;
    int y;
    char c;
    char* writeBuffer;
    int* writeIndex;
} TestContext;

Uint8 testRender(hRENDER render, Uint8 actorIndex, Sint32 screenX, Sint32 screenY,
    void* renderContext)
{
    render; screenX, screenY;
    TestContext* context = (TestContext*)renderContext;
    context->writeBuffer[*context->writeIndex] = context->c;
    *context->writeIndex = (*context->writeIndex) + 1;
    return actorIndex;
}

void testCase_SB(char* expected, TestContext** order, char* actual, int* writeIndex)
{
    hCC cc = creat_CC();
    setWorldLocus_CC(cc, 0, 0);
    setScreenLocus_CC(cc, 124, 124);

    actual[SDL_strlen(expected)] = 0;
    hSB sb = creat_SB(cc);
    for (Uint32 i = 0; i < SDL_strlen(expected); i++)
    {
        addSprite_SB(sb, order[i]->x, order[i]->y, testRender, order[i]);
    }
    *writeIndex = 0;
    render_SB(sb, NULL, 0);

    SDL_assert(SDL_strcmp(actual, expected) == 0);
    destr_SB(sb);
}

void test_SB()
{
    char actual[6];
    actual[5] = 0;
    int writeIndex = 0;

    TestContext a = { 0,  2, 'A', actual, &writeIndex };
    TestContext b = { 2,  0, 'B', actual, &writeIndex };
    TestContext c = { 0,  0, 'C', actual, &writeIndex };
    TestContext d = { -2,  0, 'D', actual, &writeIndex };
    TestContext e = { 0, -2, 'E', actual, &writeIndex };

    { TestContext* order[2] = { &b, &a }; testCase_SB("AB", order, actual, &writeIndex); }
    { TestContext* order[2] = { &a, &b }; testCase_SB("AB", order, actual, &writeIndex); }

    { TestContext* order[5] = { &e, &d, &c, &a, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &d, &c, &b, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &d, &a, &c, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &d, &b, &c, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &d, &a, &b, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &d, &b, &a, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &c, &d, &a, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &c, &d, &b, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &a, &d, &c, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &b, &d, &c, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &a, &d, &b, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &b, &d, &a, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &c, &a, &d, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &c, &b, &d, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &a, &c, &d, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &b, &c, &d, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &a, &b, &d, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &b, &a, &d, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &c, &a, &b, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &c, &b, &a, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &a, &c, &b, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &b, &c, &a, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &a, &b, &c, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &e, &b, &a, &c, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &e, &c, &a, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &e, &c, &b, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &e, &a, &c, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &e, &b, &c, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &e, &a, &b, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &e, &b, &a, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &e, &d, &a, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &e, &d, &b, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &e, &d, &c, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &e, &d, &c, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &e, &d, &b, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &e, &d, &a, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &e, &a, &d, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &e, &b, &d, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &e, &c, &d, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &e, &c, &d, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &e, &b, &d, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &e, &a, &d, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &e, &a, &b, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &e, &b, &a, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &e, &c, &b, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &e, &c, &a, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &e, &b, &c, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &e, &a, &c, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &c, &e, &a, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &c, &e, &b, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &a, &e, &c, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &b, &e, &c, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &a, &e, &b, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &b, &e, &a, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &d, &e, &a, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &d, &e, &b, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &d, &e, &c, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &d, &e, &c, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &d, &e, &b, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &d, &e, &a, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &a, &e, &d, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &b, &e, &d, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &c, &e, &d, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &c, &e, &d, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &b, &e, &d, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &a, &e, &d, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &a, &e, &b, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &b, &e, &a, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &c, &e, &b, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &c, &e, &a, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &b, &e, &c, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &a, &e, &c, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &c, &a, &e, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &c, &b, &e, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &a, &c, &e, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &b, &c, &e, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &a, &b, &e, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &b, &a, &e, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &d, &a, &e, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &d, &b, &e, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &d, &c, &e, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &d, &c, &e, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &d, &b, &e, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &d, &a, &e, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &a, &d, &e, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &b, &d, &e, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &c, &d, &e, &b };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &c, &d, &e, &a };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &b, &d, &e, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &a, &d, &e, &c };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &a, &b, &e, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &b, &a, &e, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &c, &b, &e, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &c, &a, &e, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &b, &c, &e, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &a, &c, &e, &d };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &c, &a, &b, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &c, &b, &a, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &a, &c, &b, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &b, &c, &a, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &a, &b, &c, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &d, &b, &a, &c, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &d, &a, &b, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &d, &b, &a, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &d, &c, &b, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &d, &c, &a, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &d, &b, &c, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &d, &a, &c, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &a, &d, &b, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &b, &d, &a, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &c, &d, &b, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &c, &d, &a, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &b, &d, &c, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &a, &d, &c, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &a, &b, &d, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &c, &b, &a, &d, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &c, &b, &d, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &c, &a, &d, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &a, &b, &c, &d, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
    { TestContext* order[5] = { &b, &a, &c, &d, &e };
    testCase_SB("ABCDE", order, actual, &writeIndex); }
}