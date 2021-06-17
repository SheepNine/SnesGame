#pragma once
#include "api/snes_api.h"
#include "gamepad.h"
#include "sc.h"

hUPDATE creat_UPDATE(hGP gp, hSC* soundChannels);
void destr_UPDATE(hUPDATE update);
