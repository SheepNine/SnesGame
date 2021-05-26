#include "update.h"

struct UPDATE {
	hGP gp;
} UPDATE;

hUPDATE creat_UPDATE(hGP gp) {
	hUPDATE result = (hUPDATE)SDL_malloc(sizeof(UPDATE));
	result->gp = gp;
	return result;
}

void destr_UPDATE(hUPDATE render) {
	// Proxy class; doesn't own any of its references
}

SDL_bool isButtonIn(hUPDATE update, GP_BUTTON button) {
	return isEngaged_GP(update->gp, button);
}

SDL_bool isButtonOut(hUPDATE update, GP_BUTTON button) {
	return isEngaged_GP(update->gp, button) == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
}

SDL_bool wasButtonPressed(hUPDATE update, GP_BUTTON button) {
	return SDL_FALSE; // Pending implementation
}

SDL_bool wasButtonReleased(hUPDATE update, GP_BUTTON button) {
	return SDL_FALSE; // Pending implementation
}