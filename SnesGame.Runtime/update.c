#include "update.h"

struct UPDATE {
	hGP gp;
	ChannelVoice* voices;
} UPDATE;

hUPDATE creat_UPDATE(hGP gp, ChannelVoice* voices) {
	hUPDATE result = (hUPDATE)SDL_malloc(sizeof(UPDATE));
	result->gp = gp;
	result->voices = voices;
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
	update->voices[channelIndex].type = CV_SILENCE;
}

void playSquareNote(hUPDATE update, Uint8 channelIndex, SquareWaveParams* params) {
	update->voices[channelIndex].type = CV_SQUARE;
	update->voices[channelIndex].waveParams.square = *params;
}
