#pragma once
#include "SDL.h"

typedef enum {
	GP_BUTTON_DU =  1 << 0,
	GP_BUTTON_DR =  1 << 1,
	GP_BUTTON_DD =  1 << 2,
	GP_BUTTON_DL =  1 << 3,
	GP_BUTTON_BX =  1 << 4,
	GP_BUTTON_BA =  1 << 5,
	GP_BUTTON_BB =  1 << 6,
	GP_BUTTON_BY =  1 << 7,
	GP_BUTTON_SL = 1 << 8,
	GP_BUTTON_ST =  1 << 9,
	GP_BUTTON_ZL =  1 << 10,
	GP_BUTTON_ZR =  1 << 11,
} GP_BUTTON;

typedef struct GP* hGP;

hGP creat_GP();
void destr_GP(hGP gp);

void engageButton_GP(hGP gp, GP_BUTTON button);
void disengageButton_GP(hGP gp, GP_BUTTON button);
void setButtonEngaged_GP(hGP gp, GP_BUTTON button, SDL_bool value);
SDL_bool isEngaged_GP(hGP gp, GP_BUTTON button);
