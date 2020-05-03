#include "Hull.h"

#include <iostream>

Hull::Hull(int x, int y, int width, int height, int frames, const char* texturepath, TextureManager texturemanager)
{
	srcrects = new SDL_Rect[frames];
	dstrect = SDL_Rect();

	this->frames = frames;


	texture = texturemanager.LoadTextureWithTransparency(texturepath, 0xFF, 0x00, 0xFF);

	for (int frame = 0; frame < frames; frame++) {
		texturemanager.GetTextureWH(texture, &srcrects[frame].w, &srcrects[frame].h);
		srcrects[frame].w /= frames;
		srcrects[frame].x = frame * srcrects[frame].w;
		srcrects[frame].y = 0;

		dstrect.x = texturemanager.resolution[0] / 2 + x - width / 2;
		dstrect.y = texturemanager.resolution[1] / 4 * 3 + y - height / 2;
		//std::cout << dstrect.x << std::endl;
		this->x = dstrect.x;
		this->y = dstrect.y;

		dstrect.w = width;
		dstrect.h = height;
		original_width = width;
		original_height = height;
	}

	this->renderer = texturemanager.renderer;
}

Hull::~Hull() {
	SDL_DestroyTexture(texture);
	printf("Deleting hull...");

}

void Hull::Draw() {
	SDL_RenderCopy(renderer, texture, &srcrects[frame], &dstrect);
}

void Hull::Move(double x, double y) {
	if (x < 0) {
		frame = 0;
	}
	if (x > 0) {
		frame = 2;
	}
	if (x == 0) {
		frame = 1;
	}
	this->x += x;
	this->y -= y;

	dstrect.x = this->x;
	dstrect.y = this->y;
}

void Hull::SetSize(double size) {
	if (size != 0) {
		dstrect.w = original_width * size;
		dstrect.h = original_height * size;
		//std::cout << original_width << " * " << size << " = " << dstrect.w << std::endl;
		this->size = size;
	}
}

void Hull::Fall(double gravity) {
	if (size > 1 + gravity) {
		SetSize(size - gravity);
	}
	else {
		SetSize(1);
	}
}