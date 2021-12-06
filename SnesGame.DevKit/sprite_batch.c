#include "api/snes_devkit.h"

typedef struct Sprite {
	Sint32 worldX;
	Sint32 worldY;
	hSBRC renderCallback;
	void* renderContext;
} Sprite;

struct SB {
	hCC cc;
	Sprite sprites[128];
	Uint8 numSprites;
} SB;

hSB creat_SB(hCC cc) {
	hSB result = (hSB)SDL_malloc(sizeof(SB));
	result->cc = clone_CC(cc);
	result->numSprites = 0;
	return result;
}

void destr_SB(hSB sb) {
	destr_CC(sb->cc);
	SDL_free(sb);
}

void addSprite_SB(hSB sb, Sint32 worldX, Sint32 worldY, hSBRC renderCallback, void* renderContext) {
	if (sb->numSprites == 128) return;

	Sprite* sprite = &sb->sprites[sb->numSprites];
	sprite->worldX = worldX;
	sprite->worldY = worldY;
	sprite->renderCallback = renderCallback;
	sprite->renderContext = renderContext;

	// TODO: sort the sprite according to its screen position

	sb->numSprites += 1;
}

Uint8 render_SB(hSB sb, hRENDER render, Uint8 actorIndex) {
	Sint32 screenX, screenY;
	for (int i = 0; i < sb->numSprites; i++) {
		Sprite* sprite = &sb->sprites[i];
		worldToScreen_CC(sb->cc, sprite->worldX, sprite->worldY, &screenX, &screenY);
		screenX >>= 16;
		screenY >>= 16;
		actorIndex = sprite->renderCallback(render, actorIndex, screenX, screenY, sprite->renderContext);
		if (actorIndex == 128)
			break;
	}
	return actorIndex;
}