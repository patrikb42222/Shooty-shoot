#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

class TextureManager
{	
public:
	TextureManager(SDL_Renderer* renderer);
	void SetRenderer(SDL_Renderer* renderer);
	SDL_Texture* LoadTexture(const char* texturepath);
	SDL_Surface* LoadSurface(const char* texturepath);
	SDL_Texture* LoadTextureWithTransparency(const char* texturepath, Uint8 R, Uint8 G, Uint8 B);
	SDL_Texture* LoadMaskTexture(const char* texturepath);
	void ComposeTexture(SDL_Surface* alphaSurface, SDL_Surface* mainSurface, SDL_Texture** output);
	void CopyTextureToTexture(SDL_Texture* srctexture, SDL_Texture* dsttexture, SDL_Rect* srcrect, SDL_Rect* dstrect);
	void ResetRenderTarget();
	void GetTextureWH(SDL_Texture* texture, int* w, int* h);


	SDL_Renderer* renderer;
	int resolution[2];
private:
};

