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


typedef struct RenderSingleBrushContext {
	Uint8 layer;
	Uint8 brushIndex;
	Uint8 bankIndex;
	Uint8 paletteIndex;
	Sint8 centerX;
	Sint8 centerY;
	Uint8 sizeX;
	Uint8 sizeY;
	SDL_bool hFlip;
	SDL_bool vFlip;
	SDL_bool mask0;
} RenderSingleBrushContext;

Uint8 renderSingleBrush(hRENDER render, Uint8 actorIndex, Sint32 screenX, Sint32 screenY, void* rawContext) {
	RenderSingleBrushContext* context = rawContext;
	showActor(render, actorIndex, screenX - context->centerX, screenY - context->centerY, context->sizeX, context->sizeY, context->layer);
	setActorStroke(render, actorIndex, context->brushIndex, context->bankIndex, context->paletteIndex, context->hFlip, context->vFlip, context->mask0);
	SDL_free(rawContext);
	return actorIndex + 1;
}

void addSingleBrushSprite_SB(hSB sb, Sint32 worldX, Sint32 worldY, Uint8 layer, Uint8 brushIndex, Uint8 bankIndex,
	Uint8 paletteIndex, Sint8 centerX, Sint8 centerY, Uint8 sizeX, Uint8 sizeY, SDL_bool hFlip, SDL_bool vFlip, SDL_bool mask0) {
	RenderSingleBrushContext* context = (RenderSingleBrushContext*)SDL_malloc(sizeof(RenderSingleBrushContext));
	context->layer = layer;
	context->brushIndex = brushIndex;
	context->bankIndex = bankIndex;
	context->paletteIndex = paletteIndex;
	context->centerX = centerX;
	context->centerY = centerY;
	context->sizeX = sizeX;
	context->sizeY = sizeY;
	context->hFlip = hFlip;
	context->vFlip = vFlip;
	context->mask0 = mask0;
	addSprite_SB(sb, worldX, worldY, renderSingleBrush, context);
}
