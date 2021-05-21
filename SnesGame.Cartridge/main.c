#include <wtypes.h>
#include "snes_api.h"

void initFunc(void* hINIT) {

}

void updateFunc(void* hUPDATE) {

}

void renderFunc(void* hRENDER) {

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	return libMain("Tendies", *initFunc, *updateFunc, *renderFunc);
}