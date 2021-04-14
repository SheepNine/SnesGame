#include "joydriver.h"

typedef void (*JoyButtonEventHandler)(SDL_JoyButtonEvent* evt);
typedef void (*JoyAxisEventHandler)(SDL_JoyAxisEvent* evt);
typedef void (*JoyHatEventHandler)(SDL_JoyHatEvent* evt);

void NullButton(SDL_JoyButtonEvent* evt) { }
void NullAxis(SDL_JoyAxisEvent* evt) { }
void NullHat(SDL_JoyHatEvent* evt) { }

void RetrolinkButtonDown(SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0:
		SDL_Log("+X");
		break;
	case 1:
		SDL_Log("+A");
		break;
	case 2:
		SDL_Log("+B");
		break;
	case 3:
		SDL_Log("+Y");
		break;
	case 4:
		SDL_Log("+ZL");
		break;
	case 5:
		SDL_Log("+ZR");
		break;
	case 8:
		SDL_Log("+SELECT");
		break;
	case 9:
		SDL_Log("+START");
		break;
	}
}

void RetrolinkButtonUp(SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0:
		SDL_Log("-X");
		break;
	case 1:
		SDL_Log("-A");
		break;
	case 2:
		SDL_Log("-B");
		break;
	case 3:
		SDL_Log("-Y");
		break;
	case 4:
		SDL_Log("-ZL");
		break;
	case 5:
		SDL_Log("-ZR");
		break;
	case 8:
		SDL_Log("-SELECT");
		break;
	case 9:
		SDL_Log("-START");
		break;
	}
}

void RetrolinkAxis(SDL_JoyAxisEvent* evt) {
	switch (evt->axis) {
	case 0:
		switch (evt->value) {
		case -32768:
			SDL_Log("+L -R");
			break;
		case -256:
			SDL_Log("-L -R");
			break;
		case 32767:
			SDL_Log("-L +R");
			break;
		}
		break;
	case 4:
		switch (evt->value) {
		case -32768:
			SDL_Log("+U -D");
			break;
		case -256:
			SDL_Log("-U -D");
			break;
		case 32767:
			SDL_Log("-U +D");
			break;
		}
		break;
	}
}

void XboxButtonDown(SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0:
		SDL_Log("+A");
		break;
	case 1:
		SDL_Log("+B");
		break;
	case 2:
		SDL_Log("+X");
		break;
	case 3:
		SDL_Log("+Y");
		break;
	case 4:
		SDL_Log("+ZL");
		break;
	case 5:
		SDL_Log("+ZR");
		break;
	case 6:
		SDL_Log("+SELECT");
		break;
	case 7:
		SDL_Log("+START");
		break;
	default:
		SDL_Log(evt->button);
		break;
	}
}

void XboxButtonUp(SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0:
		SDL_Log("-A");
		break;
	case 1:
		SDL_Log("-B");
		break;
	case 2:
		SDL_Log("-X");
		break;
	case 3:
		SDL_Log("-Y");
		break;
	case 4:
		SDL_Log("-ZL");
		break;
	case 5:
		SDL_Log("-ZR");
		break;
	case 6:
		SDL_Log("-SELECT");
		break;
	case 7:
		SDL_Log("-START");
		break;
	}
}

void XboxHat(SDL_JoyHatEvent* evt) {
	if (evt->hat != 0) return;

	char u = (evt->value & 0x1) ? 'U' : '-';
	char r = (evt->value & 0x2) ? 'R' : '-';
	char d = (evt->value & 0x4) ? 'D' : '-';
	char l = (evt->value & 0x8) ? 'L' : '-';

	SDL_Log("D %c%c%c%c", u, r, d, l);
}

void XboxAxis(SDL_JoyAxisEvent* evt) {
	// TODO: Map controller
}

struct JD {
	SDL_Joystick* joystick;
	JoyButtonEventHandler buttonDown;
	JoyButtonEventHandler buttonUp;
	JoyAxisEventHandler axis;
	JoyHatEventHandler hat;
} JD;

hJD creat_JD(Sint32 which) {
	hJD result = (hJD)SDL_malloc(sizeof(JD));
	result->joystick = SDL_JoystickOpen(which);
	// TODO handle joystick NULL

	char guidBuffer[33];
	SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(result->joystick), guidBuffer, sizeof(guidBuffer));

	if (SDL_strcmp(guidBuffer, "78696e70757401000000000000000000") == 0) {
		SDL_Log("XBox 360/XBox One Controller Connected");
		result->buttonDown = &XboxButtonDown;
		result->buttonUp = &XboxButtonUp;
		result->axis = &XboxAxis;
		result->hat = &XboxHat;
	}
	else if (SDL_strcmp(guidBuffer, "79001100000000000000504944564944") == 0) {
		SDL_Log("Retrolink SNES USB Controller Connected");
		result->buttonDown = &RetrolinkButtonDown;
		result->buttonUp = &RetrolinkButtonUp;
		result->axis = &RetrolinkAxis;
		result->hat = &NullHat;
	}
	else {
		SDL_Log("Unrecognized Controller Connected");
		result->buttonDown = &NullButton;
		result->buttonUp = &NullButton;
		result->axis = &NullAxis;
		result->hat = &NullHat;
	}

	return result;
}

void destr_JD(hJD jd) {
	SDL_JoystickClose(jd->joystick);
	SDL_free(jd);
}

hJD handleDeviceRemoved_JD(hJD jd, SDL_JoyDeviceEvent* evt) {
	if (evt->which != SDL_JoystickInstanceID(jd->joystick)) {
		return jd;
	}

	SDL_Log("Controller Disconnected");
	destr_JD(jd);
	return NULL;
}


void handleJoyButtonDown_JD(hJD jd, SDL_JoyButtonEvent* evt) {
	if (evt->which == SDL_JoystickInstanceID(jd->joystick)) {
		jd->buttonDown(evt);
	}
}

void handleJoyButtonUp_JD(hJD jd, SDL_JoyButtonEvent* evt) {
	if (evt->which != SDL_JoystickInstanceID(jd->joystick)) {
		jd->buttonUp(evt);
	}
}

void handleJoyAxisMotion_JD(hJD jd, SDL_JoyAxisEvent* evt) {
	if (evt->which != SDL_JoystickInstanceID(jd->joystick)) {
		jd->axis(evt);
	}
}

void handleJoyHatMotion_JD(hJD jd, SDL_JoyHatEvent* evt) {
	if (evt->which != SDL_JoystickInstanceID(jd->joystick)) {
		jd->hat(evt);
	}
}