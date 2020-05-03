#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer) {
	resolution[0] = 1920;
	resolution[1] = 1080;
}

void TextureManager::SetRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

SDL_Texture* TextureManager::LoadTexture(const char* texturepath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, texturepath);
	if (texture == NULL) {
		printf("Texture couldn't be loaded! IMG_Error: %s\n", IMG_GetError());
	}
	else {
		printf("Loaded texture %s\n", texturepath);
	}
	return texture;
}


SDL_Surface* TextureManager::LoadSurface(const char* texturepath) {
	SDL_Surface* surface = NULL;
	surface = IMG_Load(texturepath);
	if (surface == NULL) {
		printf("Surface couldn't be loaded! IMG_Error: %s\n", IMG_GetError());
	}
	else {
		printf("Loaded surface %s\n", texturepath);
	}

	return surface;
}

SDL_Texture* TextureManager::LoadTextureWithTransparency(const char* texturepath, Uint8 R, Uint8 G, Uint8 B) {
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	surface = IMG_Load(texturepath); 
	if (surface == NULL) {
		printf("Surface couldn't be loaded! IMG_Error: %s\n", IMG_GetError());
	}
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, R, G, B));
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == NULL) {
		printf("Texture couldn't be loaded! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		printf("Loaded texture %s\n", texturepath);
	}
	return texture;
}

SDL_Texture* TextureManager::LoadMaskTexture(const char* texturepath) {
	SDL_Texture* temptexture = NULL;
	temptexture = IMG_LoadTexture(renderer, texturepath);
	if (temptexture == NULL) {
		printf("TempTexture couldn't be loaded! IMG_Error: %s\n", IMG_GetError());
	}
	else {
		printf("Loaded texture %s\n", texturepath);
	}

	SDL_BlendMode mode = SDL_ComposeCustomBlendMode(
		SDL_BLENDFACTOR_ZERO,
		SDL_BLENDFACTOR_ONE,  // keep color of the target texture
		SDL_BLENDOPERATION_ADD,
		SDL_BLENDFACTOR_ONE, // keep src alpha channel 
		SDL_BLENDFACTOR_ZERO, // set alpha channel of target texture to zero
		SDL_BLENDOPERATION_ADD);

	int w;
	int h;

	GetTextureWH(temptexture, &w, &h);

	SDL_Texture* texture = NULL;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(texture, mode);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, temptexture, NULL, NULL);
	if (texture == NULL) {
		printf("Texture couldn't be loaded! IMG_Error: %s\n", IMG_GetError());
	}
	else {
		printf("Loaded texture %s\n", texturepath);
	}
	SDL_SetRenderTarget(renderer, NULL);
	return texture;
}


SDL_Texture* TextureManager::ComposeMaskTexture(SDL_Surface* mainSurface, SDL_Surface* maskSurface, SDL_Window* window) {
	SDL_Surface* composedSurface = mainSurface;
	SDL_LockSurface(composedSurface);
	Uint32* mainPixels = (Uint32*)composedSurface->pixels;
	int mainCols = composedSurface->pitch;
	SDL_LockSurface(maskSurface);
	Uint32* maskPixels = (Uint32*)maskSurface->pixels;
	int maskCols = maskSurface->w;
	int maskRows = maskSurface->h;

	for (int pixel = 0; pixel < maskCols; pixel++) {
		mainPixels[pixel] = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xAA, 0xAA, 0xAA, 0xAA);
	}

	SDL_UnlockSurface(mainSurface);
	SDL_UnlockSurface(maskSurface);

	SDL_BlitSurface(composedSurface, NULL, SDL_GetWindowSurface(window), NULL);
	SDL_UpdateWindowSurface(window);
	SDL_Delay(10000);

	SDL_Texture* texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, composedSurface);
	return texture;
}

void TextureManager::CopyTextureToTexture(SDL_Texture* srctexture, SDL_Texture* dsttexture, SDL_Rect* srcrect, SDL_Rect* dstrect) {
	SDL_SetRenderTarget(this->renderer, dsttexture);
	SDL_RenderCopy(this->renderer, srctexture, srcrect, dstrect);
}

void TextureManager::ResetRenderTarget() {
	SDL_SetRenderTarget(renderer, NULL);
}

void TextureManager::GetTextureWH(SDL_Texture* texture, int* w, int* h) {
	SDL_QueryTexture(texture, NULL, NULL, w, h);
}