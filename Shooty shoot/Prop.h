#pragma once
#include <SDL_image.h>
#include <SDL.h>
#include <iostream>
#include "TextureManager.h"
#include "Terrain.h"

class Prop
{
	public:
		Prop(int dist, int x, int y, int width, int height, int INframes, const char* texturepath, Game* game, Terrain* terrain);
		Prop(int dist, int x, int y, int INframes, const char* texturepath, Game* game, Terrain* terrain);
		~Prop();
		bool OnScreen();
		bool Behind();
		void FollowTerrain();
		void Draw();
		void Move(Terrain terrain);
		void Move(int x, int y);
		void AddHeightRect(int x, int y, int w, int h, double height);
		double GetHeight(int x, int y);
		void SetSize(int w, int h);
		SDL_Rect dstrect;
		int dist;
		Terrain* terrain;
	private:
		SDL_Renderer* renderer;
		SDL_Rect* srcrects;
		std::vector<SDL_Rect> heightrects;
		std::vector<double> heights;
		int frames;
		SDL_Texture* texture;

		std::vector<double> precise_coords;
		double y;
		Game* game;
};

