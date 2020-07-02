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

/*SDL_Texture* TextureManager::LoadMaskTexture(const char* texturepath) {
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
}*/


void TextureManager::ComposeTexture(SDL_Surface* alphaSurface, SDL_Surface* mainSurface, SDL_Texture** output) {
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_Surface* alphaCopy = SDL_ConvertSurface(alphaSurface, format, 0);
	SDL_Surface* mainCopy = SDL_ConvertSurface(mainSurface, format, 0);

	SDL_LockSurface(mainCopy);
	Uint32* alphapixels = (Uint32*)alphaCopy->pixels;
	Uint32* dstpixels = (Uint32*)mainCopy->pixels;
	SDL_SetSurfaceBlendMode(mainCopy, SDL_BLENDMODE_BLEND);
	for (int y = 0; y < mainCopy->h; y++) {
		for (int x = 0; x < mainCopy->w; x++) {
				Uint8 alphaPixel[4];
			Uint8 dstPixel[4];
			SDL_GetRGBA(alphapixels[x + y * mainCopy->w], format, &alphaPixel[0], &alphaPixel[1], &alphaPixel[2], &alphaPixel[3]);
			SDL_GetRGBA(dstpixels[x + y * mainCopy->w], format, &dstPixel[0], &dstPixel[1], &dstPixel[2], &dstPixel[3]);
			dstpixels[x + y * mainCopy->w] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), dstPixel[0], dstPixel[1], dstPixel[2], alphaPixel[3]);
			//dstpixels[x + y * mainCopy->w] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), alphaPixel[0], 0, alphaPixel[2], alphaPixel[3]);
		}
	}
	SDL_UnlockSurface(mainCopy);

	/*SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, mainCopy), NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);*/


	SDL_DestroyTexture(*output);
	SDL_FreeFormat(format);
	*output = SDL_CreateTextureFromSurface(renderer, mainCopy);
	SDL_FreeSurface(alphaCopy);
	SDL_FreeSurface(mainCopy);
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

