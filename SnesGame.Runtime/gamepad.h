#pragma once
#include "api/snes_api.h"

typedef struct GP* hGP;

hGP creat_GP();
void destr_GP(hGP gp);

void engageButton_GP(hGP gp, GP_BUTTON button);
void disengageButton_GP(hGP gp, GP_BUTTON button);
void setButtonEngaged_GP(hGP gp, GP_BUTTON button, SDL_bool value);
SDL_bool isEngaged_GP(hGP gp, GP_BUTTON button);
