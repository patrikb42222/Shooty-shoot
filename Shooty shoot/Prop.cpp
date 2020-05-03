#include "Prop.h"
#include <iostream>

Prop::Prop(int dist, int x, int y, int width, int height, int INframes, const char* texturepath, Game* game, Terrain* terrain)
{
	srcrects = new SDL_Rect[INframes];
	dstrect = SDL_Rect();
	this->game = game;

	frames = INframes;


	texture = NULL;

	texture = game->texturemanager->LoadTextureWithTransparency(texturepath, 0xFF, 0x00, 0xFF);


	for (int frame = 0; frame < frames; frame++) {
		game->texturemanager->GetTextureWH(texture, &srcrects[frame].w, &srcrects[frame].h);
		srcrects[frame].x = frame * srcrects->w;
		srcrects[frame].y = 0;
	}

	dstrect.w = width;
	dstrect.h = height;
	dstrect.x = x;
	dstrect.y = y-dstrect.h;
	this->y = y;
	precise_coords.push_back(dstrect.x);
	precise_coords.push_back(dstrect.y);


	this->terrain = terrain;
	this->renderer = game->texturemanager->renderer;
	this->dist = dist*(terrain->height/terrain->tiledivider);
}

Prop::~Prop() {
	SDL_DestroyTexture(texture);
	printf("Deleting prop...");

}

bool Prop::OnScreen() {
	//std::cout << terrain->tiles_rows * (terrain->height / terrain->tiledivider) << std::endl;
	//std::cout << dist << std::endl;
	//std::cout << terrain->distance_travelled + y + dstrect.h << std::endl;
	//std::cout << dist << std::endl;
	if (terrain->distance_travelled+y+dstrect.h >= dist) {
		//std::cout << "onscreen" << std::endl;
		return true;
	}
	else {
		return false;
	}
}

bool Prop::Behind() {
	if (terrain->distance_travelled / terrain->tiledivider - terrain->tiles_rows*terrain->tiledivider >= dist) {
		return false;
	}
	else {
		return false;
	}
}

void Prop::FollowTerrain() {
	if (terrain->speed != 0 && game->fps != 0) {
		precise_coords[1] += terrain->speed / game->fps;
	}
	dstrect.y = precise_coords[1];
}

void Prop::Draw() {
	SDL_RenderCopy(renderer, texture, &srcrects[0], &dstrect);
}



void Prop::Move(int x, int y) {
	dstrect.x += x;
	dstrect.y -= y;
}

void Prop::AddHeightRect(int x, int y, int w, int h, double height) {
	SDL_Rect heightrect;
	heightrect.x = x;
	heightrect.y = y;
	heightrect.w = w;
	heightrect.h = h;
	heightrects.push_back(heightrect);

	heights.push_back(height);
}

double Prop::GetHeight(int x, int y) {
	for (int height = 0; height < heights.size(); height++) {
		if (game->PointInRect(&heightrects[height], x, y)) {
			std::cout << "Height:" << heights[height] << std::endl;
			return heights[height];
		}
	}
	return 0;
}