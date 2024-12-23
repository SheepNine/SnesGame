#pragma once
#include "api/snes_runtime.h"
#include "gamepad.h"
#include "sc.h"

hUPDATE creat_UPDATE(hGP gp, ChannelVoice* voices, Uint64 frameCounter);
void destr_UPDATE(hUPDATE update);
