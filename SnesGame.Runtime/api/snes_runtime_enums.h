#pragma once

typedef enum {
	GP_BUTTON_DU = 0,
	GP_BUTTON_DR = 1,
	GP_BUTTON_DD = 2,
	GP_BUTTON_DL = 3,
	GP_BUTTON_BX = 4,
	GP_BUTTON_BA = 5,
	GP_BUTTON_BB = 6,
	GP_BUTTON_BY = 7,
	GP_BUTTON_SL = 8,
	GP_BUTTON_ST = 9,
	GP_BUTTON_ZL = 10,
	GP_BUTTON_ZR = 11,
} GP_BUTTON;


typedef enum {
	SD_FALLING = -1,
	SD_NONE = 0,
	SD_RISING = 1
} ShiftDir;

typedef enum {
	EB_SILENCE = 0,
	EB_REVERSE = 1,
	EB_LOOP = 2,
	EB_STEADY = 3,
} EdgeBehaviour;

typedef enum {
	CV_SUSTAIN,
	CV_SILENCE,
	CV_SQUARE,
	CV_NOISE
} ChannelVoiceType;