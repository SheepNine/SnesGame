#include "keydriver.h"

#define NUM_BUTTONS 12

struct KD {
	hGP gp;
	SDL_Keycode mapping[NUM_BUTTONS];
} KD;

hKD creat_KD(hGP gp) {
	hKD result = (hKD)SDL_malloc(sizeof(KD));
	result->gp = gp;
	result->mapping[GP_BUTTON_DU] = SDLK_UP;
	result->mapping[GP_BUTTON_DR] = SDLK_RIGHT;
	result->mapping[GP_BUTTON_DD] = SDLK_DOWN;
	result->mapping[GP_BUTTON_DL] = SDLK_LEFT;
	result->mapping[GP_BUTTON_BX] = SDLK_w;
	result->mapping[GP_BUTTON_BA] = SDLK_d;
	result->mapping[GP_BUTTON_BB] = SDLK_s;
	result->mapping[GP_BUTTON_BY] = SDLK_a;
	result->mapping[GP_BUTTON_SL] = SDLK_RSHIFT;
	result->mapping[GP_BUTTON_ST] = SDLK_RETURN;
	result->mapping[GP_BUTTON_ZL] = SDLK_q;
	result->mapping[GP_BUTTON_ZR] = SDLK_e;
	return result;
}

void destr_KD(hKD kd) {
	SDL_free(kd);
}

void handleKeyDown_KD(hKD kd, SDL_KeyboardEvent* evt) {
	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (kd->mapping[i] == evt->keysym.sym)
			engageButton_GP(kd->gp, i);
	}
}

void handleKeyUp_KD(hKD kd, SDL_KeyboardEvent* evt) {
	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (kd->mapping[i] == evt->keysym.sym)
			disengageButton_GP(kd->gp, i);
	}
}
