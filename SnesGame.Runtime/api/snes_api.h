#pragma once

typedef struct INIT* H_INIT;
typedef void (*pInitCallback)(H_INIT);
extern int libMain(char* title, pInitCallback initFunc);