#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include <string>
#include "Game.h"
#include <stdio.h>
#include <time.h>

class Terrain
{
public:
	Terrain(Game* game);
	void GetMaskTextures();
	void Draw();
	void Move();
	void Buffer();
	void AddTexture(const char* texturepath);
	void RandomizeTerrain();
	void CopyTilesToTexture();
	void LoadLine(int dstrect_y);
	std::vector<std::vector<SDL_Rect>> dstrects;

	double tilemultiplier=1;
	int tiledivider = 12;

	int width = 256/tilemultiplier;
	int height = 256/tilemultiplier;

	double speed = 1;
	int tiles_coloumns;
	int tiles_rows;
	std::vector<std::vector<std::vector<int>>> tiles;
	double distance_travelled = 0;
	int last_distance_travelled = 0;
	std::vector<SDL_Texture*> terrain; //2 images for terrain
	double precisey;
	double smalltile = height / tiledivider;
private:
	std::vector<std::vector<std::vector<double>>> precise_coords;

	int offset_x = 100/tilemultiplier*0;
	int offset_y = 64/tilemultiplier*0;
	SDL_Renderer* renderer;
	int parts;


	std::vector<SDL_Surface*> blending_masks;

	std::vector<SDL_Rect> terraindsts;
	short currentterrain = 1;
	int line= 0;

	std::vector<std::vector<std::vector<std::vector<SDL_Texture*>>>> textures; 
	/*A list containing lists for each grass texture. 
	each list contains a list for the x dimension
	that contains a list for the y dimension.
	the list inside those contain:
	-the texture
	-the faded top part
	-the faded top left corner
	-the faded left part
	-the faded bottom left corner
	-the faded bottom part
	-the faded bottom right corner
	-the faded right part
	-the faded top right corner
	*/


	Game* game;
};

