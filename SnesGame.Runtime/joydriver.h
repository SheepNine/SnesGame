#pragma once
#include "SDL.h"
#include "gamepad.h"

typedef struct JD* hJD;

hJD creat_JD(Sint32 which, hGP gp);
void destr_JD();

hJD handleDeviceRemoved_JD(hJD jd, SDL_JoyDeviceEvent* evt);
void handleJoyButtonDown_JD(hJD jd, SDL_JoyButtonEvent* evt);
void handleJoyButtonUp_JD(hJD jd, SDL_JoyButtonEvent* evt);
void handleJoyAxisMotion_JD(hJD jd, SDL_JoyAxisEvent* evt);
void handleJoyHatMotion_JD(hJD jd, SDL_JoyHatEvent* evt);