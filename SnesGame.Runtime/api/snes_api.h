#pragma once

typedef struct INIT* H_INIT;
typedef void (*pInitCallback)(H_INIT);

typedef struct UPDATE* H_UPDATE;
typedef void (*pUpdateCallback)(H_UPDATE);

typedef struct RENDER* H_RENDER;
typedef void (*pRenderCallback)(H_RENDER);

extern int libMain(char* title, pInitCallback initFunc, pUpdateCallback updateCallback, pRenderCallback renderCallback);