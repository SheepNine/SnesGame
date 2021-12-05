#include "api/snes_devkit.h"

#define OPC_PLAY 0
#define OPC_SILENCE 1

#define OPC_DEFAULT_LENGTH 2
#define OPC_DEFAULT_VSHIFT_DIR 3
#define OPC_DEFAULT_VSHIFT_EB 4
#define OPC_DEFAULT_VSHIFT_SPEED 5
#define OPC_DEFAULT_VOLUME_BOTH 6
#define OPC_DEFAULT_VOLUME_RANGE 7
#define OPC_DEFAULT_PSHIFT_DIR 8
#define OPC_DEFAULT_PSHIFT_EB 9
#define OPC_DEFAULT_PSHIFT_SPEED 10
#define OPC_DEFAULT_PERIOD_BOTH 11
#define OPC_DEFAULT_PERIOD_RANGE 12

#define OPC_SINGLE_LENGTH 13
#define OPC_SINGLE_VSHIFT_DIR 14
#define OPC_SINGLE_VSHIFT_EB 15
#define OPC_SINGLE_VSHIFT_SPEED 16
#define OPC_SINGLE_VOLUME_BOTH 17
#define OPC_SINGLE_VOLUME_RANGE 18
#define OPC_SINGLE_PSHIFT_DIR 19
#define OPC_SINGLE_PSHIFT_EB 20
#define OPC_SINGLE_PSHIFT_SPEED 21
#define OPC_SINGLE_PERIOD_BOTH 22
#define OPC_SINGLE_PERIOD_RANGE 23

#define MAX_SFX_CHANNELS 64
#define NO_CHANNEL_ASSIGNED 255

typedef struct Record {
	Uint8 audioTrackCount;
	Uint8* opcodeStream;
	Uint16* opcodeStreamOffsets;
	Uint16* opcodeStreamLengths;
} Record;

hRecord creat_Record(Uint8 trackCount, Uint8* opcodeStream, Uint16* trackOffsets, Uint16* trackLengths) {
	hRecord result = (hRecord)SDL_malloc(sizeof(Record));
	result->audioTrackCount = trackCount;
	result->opcodeStream = opcodeStream;
	result->opcodeStreamOffsets = trackOffsets;
	result->opcodeStreamLengths = trackLengths;
	return result;
}

void destr_Record(hRecord rec) {
	SDL_free(rec);
}

typedef struct PlayingTrack* hPlayingTrack;
typedef struct PlayingTrack {
	Uint8* opcodeStream;
	Uint16 opcodeStreamLength;
	Uint16 opcodeStreamPos;
	Uint8 framesUntilNextNote;
	Uint8 assignedChannel;
	SDL_bool isBgmChannel;
	SquareWaveParams singleNoteParams;
	SquareWaveParams defaultParams;
} PlayingTrack;

typedef struct Jukebox {
	SDL_bool sfxAcquired[8];
	PlayingTrack* playingSfxTracks[MAX_SFX_CHANNELS];
	Uint8 numBgmChannelsPlaying;
	hRecord playingBgm;
} Jukebox;

hJukebox creat_Jukebox() {
	hJukebox result = (hJukebox)SDL_malloc(sizeof(Jukebox));
	for (int i = 0; i < 8; i++) result->sfxAcquired[i] = SDL_FALSE;
	for (int i = 0; i < MAX_SFX_CHANNELS; i++) result->playingSfxTracks[i] = NULL;
	result->numBgmChannelsPlaying = 0;
	result->playingBgm = NULL;
	return result;
}

void destr_Jukebox(hJukebox jukebox) {
	SDL_free(jukebox);
}

void update_Jukebox(hJukebox jukebox, hUPDATE update) {
	for (int i = 0; i < MAX_SFX_CHANNELS; i++) {
		if (jukebox->playingSfxTracks[i] == NULL) break;
		PlayingTrack* track = jukebox->playingSfxTracks[i];
		// Assign a hardware channel if none is assigned yet
		if (track->assignedChannel == NO_CHANNEL_ASSIGNED) {
			for (Uint8 j = 0; j < 8; j++) {
				if (jukebox->sfxAcquired[j] == SDL_FALSE) {
					jukebox->sfxAcquired[j] = SDL_TRUE;
					track->assignedChannel = j;
					break;
				}
			}
		}
		if (track->framesUntilNextNote > 0) {
			// let the current note continue to play
			track->framesUntilNextNote -= 1;
		}
		else {
			// process some opcodes
			while (track->opcodeStreamPos < track->opcodeStreamLength) {
				Uint8 opcode = track->opcodeStream[track->opcodeStreamPos++];
				switch (opcode) {
				case OPC_PLAY:
					if (track->assignedChannel != NO_CHANNEL_ASSIGNED && (track->isBgmChannel == SDL_FALSE || jukebox->sfxAcquired[track->assignedChannel] == SDL_FALSE)) {
						playSquareNote(update, track->assignedChannel, &track->singleNoteParams);
					}
					track->singleNoteParams = track->defaultParams;
					track->framesUntilNextNote = track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_SILENCE:
					if (track->assignedChannel != NO_CHANNEL_ASSIGNED && (track->isBgmChannel == SDL_FALSE || jukebox->sfxAcquired[track->assignedChannel] == SDL_FALSE)) {
						silenceChannel(update, track->assignedChannel);
					}
					track->framesUntilNextNote = track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_DEFAULT_PERIOD_BOTH:
					track->singleNoteParams.periodLow = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.periodLow += track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.periodHigh = track->singleNoteParams.periodLow;
					track->defaultParams.periodLow = track->singleNoteParams.periodLow;
					track->defaultParams.periodHigh = track->singleNoteParams.periodHigh;
					break;
				case OPC_DEFAULT_PERIOD_RANGE:
					track->singleNoteParams.periodLow = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.periodLow += track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.periodHigh = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.periodHigh += track->opcodeStream[track->opcodeStreamPos++];
					track->defaultParams.periodLow = track->singleNoteParams.periodLow;
					track->defaultParams.periodHigh = track->singleNoteParams.periodHigh;
					break;
				case OPC_DEFAULT_VOLUME_BOTH:
					track->singleNoteParams.volumeLow = track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.volumeHigh = track->singleNoteParams.volumeLow;
					track->defaultParams.volumeLow = track->singleNoteParams.volumeLow;
					track->defaultParams.volumeHigh = track->singleNoteParams.volumeHigh;
					break;
				case OPC_DEFAULT_VOLUME_RANGE:
					track->singleNoteParams.volumeLow = track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.volumeHigh = track->opcodeStream[track->opcodeStreamPos++];
					track->defaultParams.volumeLow = track->singleNoteParams.volumeLow;
					track->defaultParams.volumeHigh = track->singleNoteParams.volumeHigh;
					break;
				case OPC_DEFAULT_LENGTH:
					track->singleNoteParams.length = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.length += track->opcodeStream[track->opcodeStreamPos++];
					track->defaultParams.length = track->singleNoteParams.length;
					break;
				case OPC_DEFAULT_VSHIFT_SPEED:
					track->singleNoteParams.volumeShift.speed = track->opcodeStream[track->opcodeStreamPos++];
					track->defaultParams.volumeShift.speed = track->singleNoteParams.volumeShift.speed;
					break;
				case OPC_DEFAULT_PSHIFT_SPEED:
					track->singleNoteParams.periodShift.speed = track->opcodeStream[track->opcodeStreamPos++];
					track->defaultParams.periodShift.speed = track->singleNoteParams.periodShift.speed;
					break;
				case OPC_DEFAULT_VSHIFT_EB:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.volumeShift.edgeBehaviour = EB_SILENCE; break;
					case 1: track->singleNoteParams.volumeShift.edgeBehaviour = EB_LOOP; break;
					case 2: track->singleNoteParams.volumeShift.edgeBehaviour = EB_REVERSE; break;
					case 3: track->singleNoteParams.volumeShift.edgeBehaviour = EB_STEADY; break;
					}
					track->defaultParams.volumeShift.edgeBehaviour = track->singleNoteParams.volumeShift.edgeBehaviour;
					break;
				case OPC_DEFAULT_PSHIFT_EB:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.periodShift.edgeBehaviour = EB_SILENCE; break;
					case 1: track->singleNoteParams.periodShift.edgeBehaviour = EB_LOOP; break;
					case 2: track->singleNoteParams.periodShift.edgeBehaviour = EB_REVERSE; break;
					case 3: track->singleNoteParams.periodShift.edgeBehaviour = EB_STEADY; break;
					}
					track->defaultParams.periodShift.edgeBehaviour = track->singleNoteParams.periodShift.edgeBehaviour;
					break;
				case OPC_DEFAULT_VSHIFT_DIR:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.volumeShift.dir = SD_NONE; break;
					case 1: track->singleNoteParams.volumeShift.dir = SD_RISING; break;
					case 2: track->singleNoteParams.volumeShift.dir = SD_FALLING; break;
					}
					track->defaultParams.volumeShift.dir = track->singleNoteParams.volumeShift.dir;
					break;
				case OPC_DEFAULT_PSHIFT_DIR:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.periodShift.dir = SD_NONE; break;
					case 1: track->singleNoteParams.periodShift.dir = SD_RISING; break;
					case 2: track->singleNoteParams.periodShift.dir = SD_FALLING; break;
					}
					track->defaultParams.periodShift.dir = track->singleNoteParams.periodShift.dir;
					break;
				case OPC_SINGLE_PERIOD_BOTH:
					track->singleNoteParams.periodLow = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.periodLow += track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.periodHigh = track->singleNoteParams.periodLow;
					break;
				case OPC_SINGLE_PERIOD_RANGE:
					track->singleNoteParams.periodLow = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.periodLow += track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.periodHigh = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.periodHigh += track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_SINGLE_VOLUME_BOTH:
					track->singleNoteParams.volumeLow = track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.volumeHigh = track->singleNoteParams.volumeLow;
					break;
				case OPC_SINGLE_VOLUME_RANGE:
					track->singleNoteParams.volumeLow = track->opcodeStream[track->opcodeStreamPos++];
					track->singleNoteParams.volumeHigh = track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_SINGLE_LENGTH:
					track->singleNoteParams.length = track->opcodeStream[track->opcodeStreamPos++] << 8;
					track->singleNoteParams.length += track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_SINGLE_VSHIFT_SPEED:
					track->singleNoteParams.volumeShift.speed = track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_SINGLE_PSHIFT_SPEED:
					track->singleNoteParams.periodShift.speed = track->opcodeStream[track->opcodeStreamPos++];
					break;
				case OPC_SINGLE_VSHIFT_EB:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.volumeShift.edgeBehaviour = EB_SILENCE; break;
					case 1: track->singleNoteParams.volumeShift.edgeBehaviour = EB_LOOP; break;
					case 2: track->singleNoteParams.volumeShift.edgeBehaviour = EB_REVERSE; break;
					case 3: track->singleNoteParams.volumeShift.edgeBehaviour = EB_STEADY; break;
					}
					break;
				case OPC_SINGLE_PSHIFT_EB:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.periodShift.edgeBehaviour = EB_SILENCE; break;
					case 1: track->singleNoteParams.periodShift.edgeBehaviour = EB_LOOP; break;
					case 2: track->singleNoteParams.periodShift.edgeBehaviour = EB_REVERSE; break;
					case 3: track->singleNoteParams.periodShift.edgeBehaviour = EB_STEADY; break;
					}
					break;
				case OPC_SINGLE_VSHIFT_DIR:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.volumeShift.dir = SD_NONE; break;
					case 1: track->singleNoteParams.volumeShift.dir = SD_RISING; break;
					case 2: track->singleNoteParams.volumeShift.dir = SD_FALLING; break;
					}
					break;
				case OPC_SINGLE_PSHIFT_DIR:
					switch (track->opcodeStream[track->opcodeStreamPos++]) {
					case 0: track->singleNoteParams.periodShift.dir = SD_NONE; break;
					case 1: track->singleNoteParams.periodShift.dir = SD_RISING; break;
					case 2: track->singleNoteParams.periodShift.dir = SD_FALLING; break;
					}
					break;
				}
				if (track->framesUntilNextNote > 0) break;
			}
		}

		if (track->framesUntilNextNote == 0 && track->opcodeStreamPos == track->opcodeStreamLength) {
			// stream is done! clean up track
			if (track->assignedChannel != NO_CHANNEL_ASSIGNED) {
				silenceChannel(update, track->assignedChannel);
				if (track->isBgmChannel == SDL_FALSE) {
					jukebox->sfxAcquired[track->assignedChannel] = SDL_FALSE;
				}
			}
			if (track->isBgmChannel) {
				jukebox->numBgmChannelsPlaying -= 1;
			}
			SDL_free(track->opcodeStream);
			SDL_free(track);
			jukebox->playingSfxTracks[i] = NULL;
			// Shift other tracks up one
			for (int j = i + 1; j < MAX_SFX_CHANNELS; j++) {
				jukebox->playingSfxTracks[j - 1] = jukebox->playingSfxTracks[j];
			}
			jukebox->playingSfxTracks[MAX_SFX_CHANNELS - 1] = NULL;
			// Regress the loop pointer so that it iterates back on the right place
			i -= 1;
		}
	}
	if (jukebox->playingBgm != NULL && jukebox->numBgmChannelsPlaying == 0) {
		startBgm_Jukebox(jukebox);
	}
}

void playSfx_JukeboxInternal(hJukebox jukebox, hRecord clip, SDL_bool isBgm) {
	Uint8 clipChannel = 0;
	Uint8 bgmChannelAssigment = 7;
	for (int i = 0; i < MAX_SFX_CHANNELS; i++) {
		if (clipChannel == clip->audioTrackCount) return;
		if (jukebox->playingSfxTracks[i] != NULL) continue;

		PlayingTrack* newPlayingTrack = (PlayingTrack*)SDL_malloc(sizeof(PlayingTrack));
		newPlayingTrack->opcodeStreamLength = clip->opcodeStreamLengths[clipChannel];
		newPlayingTrack->opcodeStream = SDL_malloc(newPlayingTrack->opcodeStreamLength);
		SDL_memcpy(
			newPlayingTrack->opcodeStream,
			clip->opcodeStream + clip->opcodeStreamOffsets[clipChannel],
			newPlayingTrack->opcodeStreamLength);
		newPlayingTrack->framesUntilNextNote = 0;
		newPlayingTrack->opcodeStreamPos = 0;
		newPlayingTrack->isBgmChannel = isBgm;
		if (isBgm) {
			newPlayingTrack->assignedChannel = bgmChannelAssigment--;
			jukebox->numBgmChannelsPlaying += 1;
		}
		else {
			newPlayingTrack->assignedChannel = NO_CHANNEL_ASSIGNED;
		}

		newPlayingTrack->defaultParams.length = 3000;
		newPlayingTrack->defaultParams.periodHigh = 173;
		newPlayingTrack->defaultParams.periodLow = 173;
		newPlayingTrack->defaultParams.periodShift.edgeBehaviour = EB_SILENCE;
		newPlayingTrack->defaultParams.periodShift.dir = SD_NONE;
		newPlayingTrack->defaultParams.periodShift.speed = 0;
		newPlayingTrack->defaultParams.volumeLow = 8;
		newPlayingTrack->defaultParams.volumeHigh = 8;
		newPlayingTrack->defaultParams.volumeShift.edgeBehaviour = EB_SILENCE;
		newPlayingTrack->defaultParams.volumeShift.dir = SD_NONE;
		newPlayingTrack->defaultParams.volumeShift.speed = 0;

		newPlayingTrack->singleNoteParams = newPlayingTrack->defaultParams;

		jukebox->playingSfxTracks[i] = newPlayingTrack;
		clipChannel += 1;
	}
}

void playSfx_Jukebox(hJukebox jukebox, hRecord clip) {
	playSfx_JukeboxInternal(jukebox, clip, SDL_FALSE);
}

void startBgm_Jukebox(hJukebox jukebox) {
	playSfx_JukeboxInternal(jukebox, jukebox->playingBgm, SDL_TRUE);
}

void playBgm_Jukebox(hJukebox jukebox, hRecord clip) {
	if (jukebox->playingBgm != NULL) return; // Disallow playback if play in progress
	jukebox->playingBgm = clip;
	startBgm_Jukebox(jukebox);
}

void stopBgm_Jukebox(hJukebox jukebox) {
	if (jukebox->playingBgm == NULL) return;
	jukebox->playingBgm = NULL;
	for (int i = 0; i < MAX_SFX_CHANNELS; i++) {
		hPlayingTrack track = jukebox->playingSfxTracks[i];
		if (track == NULL) break;
		if (track->isBgmChannel == SDL_TRUE) {
			track->framesUntilNextNote = 0;
			track->opcodeStreamPos = track->opcodeStreamLength;
		}
	}
}

void silence_Jukebox(hJukebox jukebox, hUPDATE update) {
	for (int i = 0; i < 8; i++) {
		jukebox->sfxAcquired[i] = SDL_FALSE;
	}
	for (int i = 0; i < MAX_SFX_CHANNELS; i++) {
		if (jukebox->playingSfxTracks[i] == NULL) break;
		if (jukebox->playingSfxTracks[i]->assignedChannel != NO_CHANNEL_ASSIGNED) {
			silenceChannel(update, jukebox->playingSfxTracks[i]->assignedChannel);
		}
		SDL_free(jukebox->playingSfxTracks[i]->opcodeStream);
		SDL_free(jukebox->playingSfxTracks[i]);
		jukebox->playingSfxTracks[i] = NULL;
		jukebox->playingBgm = NULL;
	}
}
