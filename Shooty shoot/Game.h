#pragma once
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <SDL_image.h>
#include "Texturemanager.h"
#include "Hull.h"



class Game
{
	public:
		Game();

		void Initialize(TextureManager* texturemanager);
		void CloseGame();

		bool Running();
		void HandleEvents();
		void moveHulls(Hull* hulls, int hullsnum);
		void Draw();
		void LimitFPS();
		void GetFPS();

		bool PointInRect(SDL_Rect* rect, int x, int y);

		SDL_Renderer* renderer;
		SDL_Window* window;
		const Uint8* keyboardstate = SDL_GetKeyboardState(NULL);

		bool running;
		int max_fps = 200;
		int min_mspf = 1000 / max_fps;
		int last_time = SDL_GetTicks();
		double mspf = 60;
		double fps = max_fps;
		
		TextureManager* texturemanager;
	private:
		double speed = 10;
		int turnspeed = 500;
		SDL_Event e;
		
};

