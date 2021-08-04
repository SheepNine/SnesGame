#include "update.h"

struct UPDATE {
	hGP gp;
	hSC* soundChannels;
} UPDATE;

hUPDATE creat_UPDATE(hGP gp, hSC* soundChannels) {
	hUPDATE result = (hUPDATE)SDL_malloc(sizeof(UPDATE));
	result->gp = gp;
	result->soundChannels = soundChannels;
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
	return isEngaged_GP(update->gp, button) == SDL_TRUE && wasEngagedLastFrame_GP(update->gp, button) == SDL_FALSE ? SDL_TRUE : SDL_FALSE;
}

SDL_bool wasButtonReleased(hUPDATE update, GP_BUTTON button) {
	return isEngaged_GP(update->gp, button) == SDL_FALSE && wasEngagedLastFrame_GP(update->gp, button) == SDL_TRUE ? SDL_TRUE : SDL_FALSE;
}

void silenceChannel(hUPDATE update, Uint8 channelIndex) {
	silence_SC(update->soundChannels[channelIndex]);
}

void playSquareNote(hUPDATE update, Uint8 channelIndex, SquareWaveParams* params) {
	playNote_SC(update->soundChannels[channelIndex],
		params->length,
		params->volume,
		params->volumeShift.dir, params->volumeShift.speed, params->volumeShift.edgeBehaviour,
		params->periodLow, params->periodHigh,
		params->periodShift.dir, params->periodShift.speed, params->periodShift.edgeBehaviour);
}
