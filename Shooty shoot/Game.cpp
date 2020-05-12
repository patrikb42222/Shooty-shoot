#include "Game.h"

Game::Game() {
}

void Game::Initialize(TextureManager* texturemanager) {
	printf("Initializing...");


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else if(IMG_Init(IMG_INIT_PNG) < 0) {
		printf("SDL image could not initialize! SDL_image_Error: %s\n", IMG_GetError());
	}
	
	else {
		printf("Initialized!\n");
	}
	//window = SDL_CreateWindow("Shooty shoot", 500, 500, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
	window = SDL_CreateWindow("Shooty shoot", 200, 200, 600, 600, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL)
	{
	printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	this->texturemanager = texturemanager;
	texturemanager->SetRenderer(renderer);
	running = true;
	
}

void Game::CloseGame() {
	running = false;
	printf("exiting...");
}


bool Game::Running() {
	return running;
} 

void Game::HandleEvents() {
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			CloseGame();
		}
	}
}


void Game::moveHulls(Hull* hulls, int hullsnum) {
	double movex = 0;
	double movey = 0;

	if (keyboardstate[SDL_SCANCODE_A]) {
		movex = -turnspeed/fps;
	}
	if (keyboardstate[SDL_SCANCODE_D]) {
		movex = turnspeed / fps;
	}
	if (keyboardstate[SDL_SCANCODE_W]) {
		movey = turnspeed / fps;
	}
	if (keyboardstate[SDL_SCANCODE_S]) {
		movey = -turnspeed / fps;
	}
	for (int hull = 0; hull < hullsnum; hull++) {
		hulls->Move(movex, movey);
	}
}


void Game::Draw() {
	SDL_RenderPresent(renderer);
}

void Game::LimitFPS() {
	int current_time = SDL_GetTicks();
	mspf = current_time - last_time;
	last_time = current_time;
	if (mspf < min_mspf) {
		SDL_Delay(min_mspf - mspf);
	}
}

void Game::GetFPS() {
	if (min_mspf - mspf != 0) {
		fps = 1000 / mspf;
	}
	else {
		fps = max_fps;
	}

	if (fps < 0) {
		std::cout << "1000/(min_mspf-mspf): " << "1000/(" << min_mspf<<"-"<<mspf<< ")="<<fps << std::endl;
	}
}

bool Game::PointInRect(SDL_Rect* rect, int x, int y) {

	if (x > rect->x) {
	}
	else {
		//std::cout << "x NOT > rect->x: " << x << " NOT > " << rect->x << std::endl;
		return false;
	}
	if (x < rect->x + rect->w) {
	}
	else {
		//std::cout << "x NOT < rect->x+rect->w: " << x << " NOT < " << rect->x+rect->w << std::endl;
		return false;
	}
	if (y > rect->y ) {
	}
	else {
		//std::cout << "y NOT > rect->y: " << y << " NOT > " << rect->y << std::endl;
		return false;
	}
	if (y < rect->y + rect->h) {
	}
	else {
		//std::cout << "x NOT > rect->x + rect->h: " << x << " NOT > " << rect->y+rect->h << std::endl;
		return false;
	}
	return true;
}