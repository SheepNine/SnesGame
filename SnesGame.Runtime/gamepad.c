#include "gamepad.h"

struct GP {
	Uint16 current;
} GP;

hGP creat_GP() {
	hGP result = (hGP)SDL_malloc(sizeof(GP));
	result->current = 0;
	return result;
}

void destr_GP(hGP gp) {
	SDL_free(gp);
}

void engageButton_GP(hGP gp, GP_BUTTON button) {
	gp->current = gp->current | button;
}

void disengageButton_GP(hGP gp, GP_BUTTON button) {
	gp->current = gp->current & (~button);
}


void setButtonEngaged_GP(hGP gp, GP_BUTTON button, SDL_bool value) {
	if (value == SDL_TRUE) {
		engageButton_GP(gp, button);
	}
	else {
		disengageButton_GP(gp, button);
	}
}

SDL_bool isEngaged_GP(hGP gp, GP_BUTTON button) {
	return (gp->current & button) == 0 ? SDL_FALSE : SDL_TRUE;
}