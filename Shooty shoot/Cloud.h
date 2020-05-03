#pragma once
#include "Game.h"
#include "Terrain.h"
#include <vector>
class Cloud
{
public:
	Cloud(Game* game);
	void Draw();
	void Move(double x, double y);
	void Buffer();
	void FollowTerrain(Terrain* terrain);
private:
	Game* game;
	SDL_Texture* texture;
	int width;
	int height;
	int xOffset;
	int yOffset;
	double preciseX;
	double preciseY;

	std::vector<std::vector<SDL_Rect>> dstrects;
	SDL_Rect srcrect;
};

