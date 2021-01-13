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
	void AddTexture(const char* texturepath, SDL_Color color);
	void AddTerrain(const char* texturepath);
	void LoadTerrainCache(TextureManager texturemanager);
	void RandomizeTerrain();
	void CopyTilesToTexture();
	void CopyTerrainFromCache(int terrain, TextureManager texturemanager);
	void LoadLine(int dstrect_y);
	std::vector<std::vector<SDL_Rect>> dstrects;

	int tiledivider = 4;

	int width = 256; //resizes the full textures to be devisible by tiledivider, and not be larger
	int height = 256;

	double speed = 1;
	int tiles_coloumns;
	int tiles_rows;
	std::vector<std::vector<std::vector<int>>> tiles;
	double distance_travelled = 0;
	int last_distance_travelled = distance_travelled;
	std::vector<SDL_Texture*> terrain_BigTextures; //2 images for terrain
	double precisey;
	double smalltile_width = width / tiledivider;
	double smalltile_height = height / tiledivider;
	std::vector<const char*> terrainPaths;
	std::vector<SDL_Color> terrainColors;
	std::vector<SDL_Texture*> terrainCache;
private:
	std::vector<std::vector<std::vector<double>>> precise_coords;

	SDL_Renderer* renderer;
	int parts;


	std::vector<SDL_Surface*> blending_masks;

	std::vector<SDL_Rect> terraindsts;
	short currentterrain = 0;
	int line= 1;

	/////////////XXXXXXXXXXXYYYYYYYYYYYTTTTTTTTTTTT////////////////////////////
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

