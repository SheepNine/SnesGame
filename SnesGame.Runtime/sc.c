#include "sc.h"
#include "sw.h"

struct SC {
	Uint8 volumeLow;
	Uint8 volumeHigh;
	enum ShiftDir volumeShift;
	enum EdgeBehaviour volumeEB;
	Uint8 volumeShiftSpeed;
	Uint16 periodLow;
	Uint16 periodHigh;
	enum ShiftDir periodShift;
	Uint8 periodShiftSpeed;
	enum EdgeBehaviour periodEB;

	Uint16 lengthCounter;
	Uint16 periodCounter;
	Uint16 volumeShiftCounter;
	Uint16 periodShiftCounter;
	Uint16 periodCurr;
	Uint8 volumeCurr;
	SDL_bool upperWave;
	hSW sw;
} SC;


hSC creat_SC() {
	hSC result = (hSC)SDL_malloc(sizeof(SC));
	result->lengthCounter = 65535;
	result->volumeCurr = 0;
	result->periodCounter = 0;
	result->sw = creat_SW();
	return result;
}

void destr_SC(hSC sc) {
	destr_SW(sc->sw);
	SDL_free(sc);
}

void playNote_SC(hSC sc,
		Uint16 length,
		Uint8 volumeLow, Uint8 volumeHigh, ShiftDir volumeShift, Uint8 volumeShiftSpeed, EdgeBehaviour volumeEB,
		Uint16 periodLow, Uint16 periodHigh, ShiftDir periodShift, Uint8 periodShiftSpeed, EdgeBehaviour periodEB) {
	SDL_assert(0 < volumeLow && volumeHigh <= 8); // use silence_SC for zero volume
	SDL_assert(0 < volumeHigh && volumeHigh <= 8);
	SDL_assert(volumeLow <= volumeHigh);
	SDL_assert(volumeLow == volumeHigh || volumeShift != SD_NONE);
	SDL_assert(volumeLow != volumeHigh || volumeShift == SD_NONE);
	SDL_assert(16 <= periodLow && periodLow <= 24000);
	SDL_assert(16 <= periodHigh && periodHigh <= 24000);
	SDL_assert(periodLow <= periodHigh);
	SDL_assert(periodLow == periodHigh || periodShift != SD_NONE);
	SDL_assert(periodLow != periodHigh || periodShift == SD_NONE);


	sc->volumeLow = volumeLow;
	sc->volumeHigh = volumeHigh;
	sc->volumeShift = volumeShift;
	sc->volumeShiftSpeed = volumeShiftSpeed;
	sc->volumeEB = volumeEB;

	sc->periodLow = periodLow;
	sc->periodHigh = periodHigh;
	sc->periodShift = periodShift;
	sc->periodShiftSpeed = periodShiftSpeed;
	sc->periodEB = periodEB;

	sc->lengthCounter = length;
	sc->volumeShiftCounter = 0;
	sc->periodShiftCounter = 0;
	sc->periodCurr = periodShift == SD_FALLING ? periodHigh : periodLow;
	sc->volumeCurr = volumeShift == SD_FALLING ? volumeHigh : volumeLow;
}

void silence_SC(hSC sc) {
	sc->lengthCounter = 65535;
	sc->volumeCurr = 0;
}

void setupNextVolume(hSC sc) {
	if (sc->volumeShift == SD_NONE) return;
	sc->volumeShiftCounter += 1;

	if (sc->volumeShiftCounter <= sc->volumeShiftSpeed) return;
	sc->volumeShiftCounter = 0;

	if (sc->volumeShift == SD_RISING) {
		if (sc->volumeCurr < sc->volumeHigh) {
			sc->volumeCurr += 1;
		}
		else {
			if (sc->volumeEB == EB_LOOP) {
				sc->volumeCurr = sc->volumeLow;
			}
			else if (sc->volumeEB == EB_REVERSE) {
				sc->volumeShift = SD_FALLING;
				sc->volumeCurr -= 1;
			}
			else {
				sc->volumeCurr = 0;
				sc->volumeShift = SD_NONE;
			}
		}
	}
	else {
		if (sc->volumeCurr > sc->volumeLow) {
			sc->volumeCurr -= 1;
		}
		else {
			if (sc->volumeEB == EB_LOOP) {
				sc->volumeCurr = sc->volumeHigh;
			}
			else if (sc->volumeEB == EB_REVERSE) {
				sc->volumeShift = SD_RISING;
				sc->volumeCurr += 1;
			}
			else {
				sc->volumeCurr = 0;
				sc->volumeShift = 0;
			}
		}
	}
}

void setupNextPeriod(hSC sc) {
	if (sc->periodShift == SD_NONE) return;
	sc->periodShiftCounter += 1;

	if (sc->periodShiftCounter < sc->periodShiftSpeed) return;
	sc->periodShiftCounter = 0;

	if (sc->periodShift == SD_RISING) {
		if (sc->periodCurr < sc->periodHigh) {
			sc->periodCurr += 1;
		}
		else {
			if (sc->periodEB == EB_LOOP) {
				sc->periodCurr = sc->periodLow;
			}
			else if (sc->periodEB == EB_REVERSE) {
				sc->periodShift = SD_FALLING;
				sc->periodCurr -= 1;
			}
			else {
				sc->volumeCurr = 0;
				sc->periodShift = SD_NONE;
			}
		}
	}
	else {
		if (sc->periodCurr > sc->periodLow) {
			sc->periodCurr -= 1;
		}
		else {
			if (sc->periodEB == EB_LOOP) {
				sc->periodCurr = sc->periodHigh;

			}
			else if (sc->periodEB == EB_REVERSE) {
				sc->periodShift = SD_RISING;
				sc->periodCurr += 1;
			}
			else {
				sc->volumeCurr = 0;
				sc->periodShift = SD_NONE;
			}
		}
	}
}

Sint16 getNextSample_SC(hSC sc) {
	if (sc->lengthCounter == 0) {
		sc->volumeCurr = 0;
	}
	else if (sc->lengthCounter != 65535) {
		sc->lengthCounter -= 1;
	}

	if (sc->periodCounter > 0) {
		sc->periodCounter -= 1;
	}
	else {
		if (sc->volumeCurr == 0) {
			if (!isSilent_SW(sc->sw)) {
				setTargetVolume_SW(sc->sw, 0);
				sc->periodCounter = 16;
			}
		}
		else {
			setTargetVolume_SW(sc->sw, sc->upperWave ? sc->volumeCurr : -sc->volumeCurr);
			sc->periodCounter = sc->periodCurr;
			sc->upperWave = sc->upperWave ? SDL_FALSE : SDL_TRUE;

			setupNextVolume(sc);
			setupNextPeriod(sc);
		}
	}

	return getNextSample_SW(sc->sw);
}

void SampleSquareWave(
	Uint16 length,
	Uint8 volumeLow, Uint8 volumeHigh, ShiftDir volumeShift, Uint8 volumeShiftSpeed, EdgeBehaviour volumeEB,
	Uint16 periodLow, Uint16 periodHigh, ShiftDir periodShift, Uint8 periodShiftSpeed, EdgeBehaviour periodEB,
	Sint16* buffer, Sint32 bufferSize) {

	hSC sc = creat_SC();

	playNote_SC(sc, length,
		volumeLow, volumeHigh, volumeShift, volumeShiftSpeed, volumeEB,
		periodLow, periodHigh, periodShift, periodShiftSpeed, periodEB);

	for (Sint32 i = 0; i < bufferSize; i++) {
		buffer[i] = getNextSample_SC(sc);
	}

	destr_SC(sc);
}