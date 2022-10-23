#pragma once

// Fake header file to compile without needing an SDL dependency

typedef enum
{
    SDL_FALSE = 0,
    SDL_TRUE = 1
} SDL_bool;

typedef   signed __int8  Sint8;
typedef unsigned __int8  Uint8;
typedef   signed __int16 Sint16;
typedef unsigned __int16 Uint16;
typedef   signed __int32 Sint32;
typedef unsigned __int32 Uint32;
typedef   signed __int64 Sint64;
typedef unsigned __int64 Uint64;

#include <malloc.h>

#define SDL_malloc malloc
#define SDL_free free

#include <assert.h>

#define SDL_assert assert