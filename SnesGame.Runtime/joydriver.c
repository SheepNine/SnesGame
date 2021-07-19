#include "joydriver.h"
#include "gamepad.h"

typedef void (*JoyButtonEventHandler)(hGP gp, SDL_JoyButtonEvent* evt);
typedef void (*JoyAxisEventHandler)(hGP gp, SDL_JoyAxisEvent* evt);
typedef void (*JoyHatEventHandler)(hGP gp, SDL_JoyHatEvent* evt);

void NullButton(hGP gp, SDL_JoyButtonEvent* evt) { }
void NullAxis(hGP gp, SDL_JoyAxisEvent* evt) { }
void NullHat(hGP gp, SDL_JoyHatEvent* evt) { }

void RetrolinkButtonDown(hGP gp, SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0: engageButton_GP(gp, GP_BUTTON_BX); break;
	case 1: engageButton_GP(gp, GP_BUTTON_BA); break;
	case 2: engageButton_GP(gp, GP_BUTTON_BB); break;
	case 3: engageButton_GP(gp, GP_BUTTON_BY); break;
	case 4: engageButton_GP(gp, GP_BUTTON_ZL); break;
	case 5: engageButton_GP(gp, GP_BUTTON_ZR); break;
	case 8: engageButton_GP(gp, GP_BUTTON_SL); break;
	case 9: engageButton_GP(gp, GP_BUTTON_ST); break;
	}
}

void RetrolinkButtonUp(hGP gp, SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0: disengageButton_GP(gp, GP_BUTTON_BX); break;
	case 1: disengageButton_GP(gp, GP_BUTTON_BA); break;
	case 2: disengageButton_GP(gp, GP_BUTTON_BB); break;
	case 3: disengageButton_GP(gp, GP_BUTTON_BY); break;
	case 4: disengageButton_GP(gp, GP_BUTTON_ZL); break;
	case 5: disengageButton_GP(gp, GP_BUTTON_ZR); break;
	case 8: disengageButton_GP(gp, GP_BUTTON_SL); break;
	case 9: disengageButton_GP(gp, GP_BUTTON_ST); break;
	}
}

void RetrolinkAxis(hGP gp, SDL_JoyAxisEvent* evt) {
	switch (evt->axis) {
	case 0:
		switch (evt->value) {
		case -32768:
			engageButton_GP(gp, GP_BUTTON_DL);
			disengageButton_GP(gp, GP_BUTTON_DR);
			break;
		case -256:
			disengageButton_GP(gp, GP_BUTTON_DL);
			disengageButton_GP(gp, GP_BUTTON_DR);
			break;
		case 32767:
			disengageButton_GP(gp, GP_BUTTON_DL);
			engageButton_GP(gp, GP_BUTTON_DR);
			break;
		}
		break;
	case 4:
		switch (evt->value) {
		case -32768:
			engageButton_GP(gp, GP_BUTTON_DU);
			disengageButton_GP(gp, GP_BUTTON_DD);
			break;
		case -256:
			disengageButton_GP(gp, GP_BUTTON_DU);
			disengageButton_GP(gp, GP_BUTTON_DD);
			break;
		case 32767:
			disengageButton_GP(gp, GP_BUTTON_DU);
			engageButton_GP(gp, GP_BUTTON_DD);
			break;
		}
		break;
	}
}

void XboxButtonDown(hGP gp, SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0: engageButton_GP(gp, GP_BUTTON_BB); break;
	case 1: engageButton_GP(gp, GP_BUTTON_BA); break;
	case 2: engageButton_GP(gp, GP_BUTTON_BY); break;
	case 3: engageButton_GP(gp, GP_BUTTON_BX); break;
	case 4: engageButton_GP(gp, GP_BUTTON_ZL); break;
	case 5: engageButton_GP(gp, GP_BUTTON_ZR); break;
	case 6: engageButton_GP(gp, GP_BUTTON_SL); break;
	case 7: engageButton_GP(gp, GP_BUTTON_ST); break;
	}
}

void XboxButtonUp(hGP gp, SDL_JoyButtonEvent* evt) {
	switch (evt->button) {
	case 0: disengageButton_GP(gp, GP_BUTTON_BB); break;
	case 1: disengageButton_GP(gp, GP_BUTTON_BA); break;
	case 2: disengageButton_GP(gp, GP_BUTTON_BY); break;
	case 3: disengageButton_GP(gp, GP_BUTTON_BX); break;
	case 4: disengageButton_GP(gp, GP_BUTTON_ZL); break;
	case 5: disengageButton_GP(gp, GP_BUTTON_ZR); break;
	case 6: disengageButton_GP(gp, GP_BUTTON_SL); break;
	case 7: disengageButton_GP(gp, GP_BUTTON_ST); break;
	}
}

void XboxHat(hGP gp, SDL_JoyHatEvent* evt) {
	if (evt->hat != 0) return;

	setButtonEngaged_GP(gp, GP_BUTTON_DU, (evt->value & 0x1) ? SDL_TRUE : SDL_FALSE);
	setButtonEngaged_GP(gp, GP_BUTTON_DR, (evt->value & 0x2) ? SDL_TRUE : SDL_FALSE);
	setButtonEngaged_GP(gp, GP_BUTTON_DD, (evt->value & 0x4) ? SDL_TRUE : SDL_FALSE);
	setButtonEngaged_GP(gp, GP_BUTTON_DL, (evt->value & 0x8) ? SDL_TRUE : SDL_FALSE);
}

void XboxAxis(hGP gp, SDL_JoyAxisEvent* evt) {
	if (evt->axis == 0) // Horizontal, +ve right
	{
		if (evt->value >= 16384) {
			engageButton_GP(gp, GP_BUTTON_DR);
			disengageButton_GP(gp, GP_BUTTON_DL);
		}
		else if (evt->value <= -16384) {
			disengageButton_GP(gp, GP_BUTTON_DR);
			engageButton_GP(gp, GP_BUTTON_DL);
		}
		else {
			disengageButton_GP(gp, GP_BUTTON_DR);
			disengageButton_GP(gp, GP_BUTTON_DL);
		}
	}
	else if (evt->axis == 1) // Vertical, +ve down
	{
		if (evt->value >= 16384) {
			engageButton_GP(gp, GP_BUTTON_DD);
			disengageButton_GP(gp, GP_BUTTON_DU);
		}
		else if (evt->value <= -16384) {
			disengageButton_GP(gp, GP_BUTTON_DD);
			engageButton_GP(gp, GP_BUTTON_DU);
		}
		else {
			disengageButton_GP(gp, GP_BUTTON_DD);
			disengageButton_GP(gp, GP_BUTTON_DU);
		}
	}
}

struct JD {
	SDL_Joystick* joystick;
	JoyButtonEventHandler buttonDown;
	JoyButtonEventHandler buttonUp;
	JoyAxisEventHandler axis;
	JoyHatEventHandler hat;
	hGP gp;
} JD;

hJD creat_JD(Sint32 which, hGP gp) {
	hJD result = (hJD)SDL_malloc(sizeof(JD));
	result->joystick = SDL_JoystickOpen(which);
	// TODO handle joystick NULL
	result->gp = gp;

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
		jd->buttonDown(jd->gp, evt);
	}
}

void handleJoyButtonUp_JD(hJD jd, SDL_JoyButtonEvent* evt) {
	if (evt->which == SDL_JoystickInstanceID(jd->joystick)) {
		jd->buttonUp(jd->gp, evt);
	}
}

void handleJoyAxisMotion_JD(hJD jd, SDL_JoyAxisEvent* evt) {
	if (evt->which == SDL_JoystickInstanceID(jd->joystick)) {
		jd->axis(jd->gp, evt);
	}
}

void handleJoyHatMotion_JD(hJD jd, SDL_JoyHatEvent* evt) {
	if (evt->which == SDL_JoystickInstanceID(jd->joystick)) {
		jd->hat(jd->gp, evt);
	}
}