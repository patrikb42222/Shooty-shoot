#pragma once
#include <SDL.h>
#include "TextureManager.h"
#include <stdlib.h>

class Hull
{
public:
	Hull(int x, int y, int width, int height, int INframes, const char* texturepath, TextureManager texturemanager);
	~Hull();
	void Draw();
	void Move(double x, double y);
	void SetSize(double size);
	void Fall(double gravity);
	SDL_Rect dstrect;
	double size;
private:
	SDL_Renderer* renderer;
	SDL_Rect* srcrects;
	int frames;
	SDL_Texture* texture;
	int frame;
	double x;
	double y;
	int original_width;
	int original_height;
};

