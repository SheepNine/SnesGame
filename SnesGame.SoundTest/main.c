#include <wtypes.h>
#include "snes_api.h"
#include "snes_foundation_classes.h"

SDL_bool controllerState[12];

#define BL_ASCII 0


ShiftDir shiftDirOptions[3] = { SD_NONE, SD_RISING, SD_FALLING };
char* shiftDirLabels[3] = { "NONE", "RISE", "FALL" };
Uint8 numShiftDirOptions = 3;

EdgeBehaviour edgeBehaviourOptions[3] = { EB_SILENCE, EB_LOOP, EB_REVERSE };
char* edgeBehaviourLabels[3] = { "STOP", "LOOP", "REVERSE", };
Uint8 numEdgeBehaviourOptions = 3;

Uint16 periodOptions[70] = {
										873,824,778,
	734,693,654,617,582,550,519,490,462,436,412,389,
	367,346,327,309,291,275,259,245,231,218,206,194,
	183,173,163,154,146,137,130,122,116,109,103, 97,
	 92, 87, 82, 77, 73, 69, 65, 61, 58, 55, 51, 49,
	 46, 43, 41, 39, 36, 34, 32, 31, 29, 27, 26, 24,
	 23, 22, 20, 19, 18, 17, 16
};

char* periodLabels[70] = {
													 "A0","A#0","B0",
	"C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
	"C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
	"C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
	"C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
	"C4","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
	"C5","C#6","D6","D#6","E6","F6","F#6"
};
Uint8 numPeriodOptions = 70;

Uint8 shiftSpeedOptions[10] = { 1, 2, 3, 4, 5, 6, 8, 10, 15, 20 };
Uint8 numShiftSpeedOptions = 10;

Uint8 volumeOptions[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
Uint8 numVolumeChoices = 8;

Uint16 lengthOptions[7] = { 2400, 4800, 6000, 12000, 24000, 48000, 65535 };
Uint8 numLengthChoices = 7;

Uint8 optionIndex = 0;
Uint8 numOptions = 10;

Uint8 lengthChoiceIndex = 3;
Uint8 volumeChoiceIndex = 7;
Uint8 volumeShiftDirChoiceIndex = 0;
Uint8 volumeShiftSpeedChoiceIndex = 0;
Uint8 volumeEdgeBehaviourChoiceIndex = 0;

Uint8 periodChoice1Index = 36;
Uint8 periodChoice2Index = 36;
Uint8 periodShiftDirChoiceIndex = 0;
Uint8 periodShiftSpeedChoiceIndex = 0;
Uint8 periodEdgeBehaviourChoiceIndex = 0;

char scratch[10];

SDL_bool isVolumeShifting() {
	return volumeShiftDirChoiceIndex != 0;
}

SDL_bool isPeriodShifting() {
	return periodChoice1Index != periodChoice2Index && periodShiftDirChoiceIndex != 0;
}

char* toString(Uint16 number) {
	SDL_snprintf(scratch, 10, "%05u", number);
	return scratch;
}

Uint8 increment(Uint8 value, Uint8 count) {
	return (value == count - 1) ? 0 : value + 1;
}

Uint8 decrement(Uint8 value, Uint8 count) {
	return (value == 0) ? count - 1 : value - 1;
}

void initFunc(hINIT init) {
	allocBrushLists(init, 1);
	loadBrushList(init, BL_ASCII, "..\\resources\\ascii.glyphset");
}

void updateFunc(hUPDATE update) {
	if (wasButtonPressed(update, GP_BUTTON_SL)) {
		silenceChannel(update, 0);
	}
	if (wasButtonPressed(update, GP_BUTTON_ST)) {
		SDL_bool canPitchShift = periodChoice1Index != periodChoice2Index && periodShiftDirChoiceIndex != 0;
		playSquareNote(update, 0,
			lengthOptions[lengthChoiceIndex], 
			volumeOptions[volumeChoiceIndex],
			shiftDirOptions[volumeShiftDirChoiceIndex],
			shiftSpeedOptions[volumeShiftSpeedChoiceIndex],
			edgeBehaviourOptions[volumeEdgeBehaviourChoiceIndex],
			periodOptions[canPitchShift ? SDL_max(periodChoice1Index, periodChoice2Index) : periodChoice1Index],
			periodOptions[canPitchShift ? SDL_min(periodChoice1Index, periodChoice2Index) : periodChoice1Index],
			canPitchShift ? shiftDirOptions[periodShiftDirChoiceIndex] : SD_NONE,
			shiftSpeedOptions[periodShiftSpeedChoiceIndex],
			edgeBehaviourOptions[periodEdgeBehaviourChoiceIndex]);
	}

	if (wasButtonPressed(update, GP_BUTTON_DU)) {
		optionIndex = decrement(optionIndex, numOptions);
	}
	if (wasButtonPressed(update, GP_BUTTON_DD)) {
		optionIndex = increment(optionIndex, numOptions);
	}

	if (wasButtonPressed(update, GP_BUTTON_DR)) {
		switch (optionIndex) {
		case 0:
			lengthChoiceIndex = increment(lengthChoiceIndex, numLengthChoices);
			break;
		case 1:
			volumeChoiceIndex = increment(volumeChoiceIndex, numVolumeChoices);
			break;
		case 2:
			periodChoice1Index = increment(periodChoice1Index, numPeriodOptions);
			break;
		case 3:
			volumeShiftDirChoiceIndex = increment(volumeShiftDirChoiceIndex, numShiftDirOptions);
			break;
		case 4:
			volumeShiftSpeedChoiceIndex = increment(volumeShiftSpeedChoiceIndex, numShiftSpeedOptions);
			break;
		case 5:
			volumeEdgeBehaviourChoiceIndex = increment(volumeEdgeBehaviourChoiceIndex, numEdgeBehaviourOptions);
			break;
		case 6:
			periodChoice2Index = increment(periodChoice2Index, numPeriodOptions);
			break;
		case 7:
			periodShiftDirChoiceIndex = increment(periodShiftDirChoiceIndex, numShiftDirOptions);
			break;
		case 8:
			periodShiftSpeedChoiceIndex = increment(periodShiftSpeedChoiceIndex, numShiftSpeedOptions);
			break;
		case 9:
			periodEdgeBehaviourChoiceIndex = increment(periodEdgeBehaviourChoiceIndex, numEdgeBehaviourOptions);
			break;
		}
	}
	if (wasButtonPressed(update, GP_BUTTON_DL)) {
		switch (optionIndex) {
		case 0:
			lengthChoiceIndex = decrement(lengthChoiceIndex, numLengthChoices);
			break;
		case 1:
			volumeChoiceIndex = decrement(volumeChoiceIndex, numVolumeChoices);
			break;
		case 2:
			periodChoice1Index = decrement(periodChoice1Index, numPeriodOptions);
			break;
		case 3:
			volumeShiftDirChoiceIndex = decrement(volumeShiftDirChoiceIndex, numShiftDirOptions);
			break;
		case 4:
			volumeShiftSpeedChoiceIndex = decrement(volumeShiftSpeedChoiceIndex, numShiftSpeedOptions);
			break;
		case 5:
			volumeEdgeBehaviourChoiceIndex = decrement(volumeEdgeBehaviourChoiceIndex, numEdgeBehaviourOptions);
			break;
		case 6:
			periodChoice2Index = decrement(periodChoice2Index, numPeriodOptions);
			break;
		case 7:
			periodShiftDirChoiceIndex = decrement(periodShiftDirChoiceIndex, numShiftDirOptions);
			break;
		case 8:
			periodShiftSpeedChoiceIndex = decrement(periodShiftSpeedChoiceIndex, numShiftSpeedOptions);
			break;
		case 9:
			periodEdgeBehaviourChoiceIndex = decrement(periodEdgeBehaviourChoiceIndex, numEdgeBehaviourOptions);
			break;
		}
	}
}

Uint8 cursorY[10] = { 2, 4, 6, 9, 11, 13, 16, 18, 20, 22 };

void renderFunc(hRENDER render) {
	switchBackdropBrushList(render, 0, BL_ASCII);
	setBackdropPaletteColor(render, 0, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setBackdropPaletteColor(render, 0, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setBackdropPaletteColor(render, 0, 0x2, 0x80, 0x55, 0x55, SDL_FALSE);

	setBackdropPaletteColor(render, 1, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setBackdropPaletteColor(render, 1, 0x1, 0x55, 0x55, 0x55, SDL_FALSE);
	setBackdropPaletteColor(render, 1, 0x2, 0x80, 0x55, 0x55, SDL_FALSE);

	setBackdropPaletteColor(render, 2, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setBackdropPaletteColor(render, 2, 0x1, 0xFF, 0xFF, 0x55, SDL_FALSE);
	setBackdropPaletteColor(render, 2, 0x2, 0x80, 0x55, 0x55, SDL_FALSE);

	setLayerClips(render, 0, 0, 0, 0, 0);
	setBackdropControl(render, 0, 0, 0, SDL_TRUE, SDL_FALSE);
	fillBackdrop(render, 0, 0, 0, 0, SDL_FALSE, SDL_FALSE, SDL_FALSE);

	Uint8 fancyVolumePalette = isVolumeShifting() ? 0 : 1;
	Uint8 fancyPeriodPalette = isPeriodShifting() ? 0 : 1;

	drawBackdropString(render, 0, "LENGTH: ", 3, 2, 0, 0, SDL_FALSE);
	drawBackdropString(render, 0, toString(lengthOptions[lengthChoiceIndex]), 12, 2, 0, 0, SDL_FALSE);
	drawBackdropString(render, 0, "VOLUME: ", 3, 4, 0, 0, SDL_FALSE);
	drawBackdropString(render, 0, toString(volumeOptions[volumeChoiceIndex]), 12, 4, 0, 0, SDL_FALSE);
	drawBackdropString(render, 0, "PERIOD: ", 3, 6, 0, 0, SDL_FALSE);
	drawBackdropString(render, 0, periodLabels[periodChoice1Index], 12, 6, 0, 0, SDL_FALSE);

	drawBackdropString(render, 0, "VSHIFT: ", 3, 9, 0, fancyVolumePalette, SDL_FALSE);
	drawBackdropString(render, 0, shiftDirLabels[volumeShiftDirChoiceIndex], 12, 9, 0, fancyVolumePalette, SDL_FALSE);
	drawBackdropString(render, 0, "V SSPD: ", 3, 11, 0, fancyVolumePalette, SDL_FALSE);
	drawBackdropString(render, 0, toString(shiftSpeedOptions[volumeShiftSpeedChoiceIndex]), 12, 11, 0, fancyVolumePalette, SDL_FALSE);
	drawBackdropString(render, 0, "V EB  : ", 3, 13, 0, fancyVolumePalette, SDL_FALSE);
	drawBackdropString(render, 0, edgeBehaviourLabels[volumeEdgeBehaviourChoiceIndex], 12, 13, 0, fancyVolumePalette, SDL_FALSE);

	drawBackdropString(render, 0, "PRIOD2: ", 3, 16, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, periodLabels[periodChoice2Index], 12, 16, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, "PSHIFT: ", 3, 18, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, shiftDirLabels[periodShiftDirChoiceIndex], 12, 18, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, "P SSPD: ", 3, 20, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, toString(shiftSpeedOptions[periodShiftSpeedChoiceIndex]), 12, 20, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, "P EB  : ", 3, 22, 0, fancyPeriodPalette, SDL_FALSE);
	drawBackdropString(render, 0, edgeBehaviourLabels[periodEdgeBehaviourChoiceIndex], 12, 22, 0, fancyPeriodPalette, SDL_FALSE);

	setBackdropStroke(render, 0, 1, cursorY[optionIndex], (Uint8)'>', 0, 2, 0, 0, SDL_FALSE);

	switchActorBrushList(render, 0, BL_ASCII);
	setActorPaletteColor(render, 0, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setActorPaletteColor(render, 0, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setActorPaletteColor(render, 0, 0x2, 0xFF, 0x00, 0x00, SDL_FALSE);

	setActorPaletteColor(render, 1, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setActorPaletteColor(render, 1, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setActorPaletteColor(render, 1, 0x2, 0x00, 0xFF, 0x00, SDL_FALSE);

	setActorPaletteColor(render, 2, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setActorPaletteColor(render, 2, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setActorPaletteColor(render, 2, 0x2, 0x00, 0x00, 0xFF, SDL_FALSE);

	setActorPaletteColor(render, 3, 0x0, 0x00, 0x00, 0x00, SDL_FALSE);
	setActorPaletteColor(render, 3, 0x1, 0xFF, 0xFF, 0xFF, SDL_FALSE);
	setActorPaletteColor(render, 3, 0x2, 0xFF, 0xFF, 0x00, SDL_FALSE);

	Sint32 x, y;
	Uint8 actor = 0;

	hCS cs = creat_CS();
	setScreenLocus_CS(cs, 231 << 16, 239 << 16);
	setWorldLocus_CS(cs, 32768, 32768);

	worldToScreen_CS(cs, 0, 0, &x, &y);
	setActorStroke(render, actor, 1, 0, 0, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorControl(render, actor, x >> 16, y >> 16, 1, 1, SDL_TRUE, 0);
	actor++;

	worldToScreen_CS(cs, 65536, 0, &x, &y);
	setActorStroke(render, actor, 1, 0, 1, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorControl(render, actor, x >> 16, y >> 16, 1, 1, SDL_TRUE, 0);
	actor++;

	worldToScreen_CS(cs, 0, 65536, &x, &y);
	setActorStroke(render, actor, 1, 0, 2, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorControl(render, actor, x >> 16, y >> 16, 1, 1, SDL_TRUE, 0);
	actor++;

	worldToScreen_CS(cs, 32768, 32768, &x, &y);
	setActorStroke(render, actor, 1, 0, 3, SDL_FALSE, SDL_FALSE, SDL_TRUE);
	setActorControl(render, actor, x >> 16, y >> 16, 1, 1, SDL_TRUE, 0);
	actor++;

	destr_CS(cs);
}

void deinitFunc() {
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	return libMain("Tendies", *initFunc, *updateFunc, *renderFunc, *deinitFunc);
}