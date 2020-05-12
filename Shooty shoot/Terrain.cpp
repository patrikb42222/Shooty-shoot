#include "Terrain.h"

Terrain::Terrain(Game* game) {
	this->renderer = game->renderer;
	this->game = game;
	this->textures = textures;

	srand(SDL_GetTicks());



	tiles_coloumns = game->texturemanager->resolution[0] / (width / tiledivider) + 2;
	tiles_rows = game->texturemanager->resolution[1] / smalltile;



	for (int terraindst = 0; terraindst < 2; terraindst++) {
		std::vector<std::vector<int>> temp;
		tiles.push_back(temp);

		SDL_Rect tempdst;
		terraindsts.push_back(tempdst);
		terraindsts[terraindst].w = tiles_coloumns * width / tiledivider;
		terraindsts[terraindst].h = tiles_rows * (smalltile);
		terraindsts[terraindst].x = -width / tiledivider;
		terraindsts[terraindst].y = terraindst * terraindsts[terraindst].h;
		terrain.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, terraindsts[terraindst].w, terraindsts[terraindst].h));
		precisey = terraindsts[terraindst].y;


		for (int coloumn = 0; coloumn < tiles_coloumns; coloumn++) {
			std::vector<int> tempX;
			for (int row = 0; row < tiles_rows; row++) {
				tempX.push_back(0);
			}
			tiles[terraindst].push_back(tempX);
		}



		for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
			std::vector<SDL_Rect> tempVector;

			std::vector<std::vector<double>> tempCoordVector;

			for (int dstrect_y = 0; dstrect_y < tiles_rows; dstrect_y++) {
				SDL_Rect temprect = SDL_Rect();
				tempVector.push_back(temprect);

				std::vector<double> tempCoords;
				tempCoords.push_back(0);
				tempCoords.push_back(0);
				tempCoordVector.push_back(tempCoords);
			}
			dstrects.push_back(tempVector);

			precise_coords.push_back(tempCoordVector);
		}
	}

	for (int dstrect_y = 0; dstrect_y < tiles_rows; dstrect_y++) {
		for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
				
			dstrects[dstrect_x][dstrect_y].x = width * dstrect_x / tiledivider - offset_x;
			dstrects[dstrect_x][dstrect_y].y = height * dstrect_y / tiledivider - offset_y;
			precise_coords[dstrect_x][dstrect_y][0] = dstrects[dstrect_x][dstrect_y].x;
			precise_coords[dstrect_x][dstrect_y][1] = dstrects[dstrect_x][dstrect_y].y;
		
			if (dstrect_x % 3 == 2) {
				dstrects[dstrect_x][dstrect_y].w = width / tiledivider + 1;
			}
			else {
				dstrects[dstrect_x][dstrect_y].w = width / tiledivider;
			}
			if (dstrect_y % 3 == 2) {
				dstrects[dstrect_x][dstrect_y].h = smalltile + 1;
			}
			else {
				dstrects[dstrect_x][dstrect_y].h = smalltile;
			}
		}
	}

	//line = terraindsts[0].h - width / tiledivider;
	line = tiles_rows;
}

void Terrain::GetMaskTextures() {
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_entire.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_top.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_top_left.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_left.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_bottom_left.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_bottom.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_bottom_right.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_right.png"));
	blending_masks.push_back(game->texturemanager->LoadSurface("Textures/Mask_top_right.png"));
}

void Terrain::Draw() {
	for (int terraindst = 0; terraindst < terraindsts.size(); terraindst++) {
		SDL_RenderCopy(renderer, terrain[terraindst], NULL, &terraindsts[terraindst]);
	}
}

void Terrain::Move() {
	double move = speed/game->fps;
	precisey += move;
	if (currentterrain == 0) {
		terraindsts[0].y = precisey;
		terraindsts[1].y = terraindsts[0].y - terraindsts[0].h;
	}
	else {
		terraindsts[1].y = precisey;
		terraindsts[0].y = terraindsts[1].y - terraindsts[0].h;
	}
	distance_travelled += move;
}

void Terrain::Buffer() {
	if (terraindsts[currentterrain].y > game->texturemanager->resolution[1]) {
		precisey -= terraindsts[1].h;
		if (currentterrain == 1) {
			currentterrain = 0;
		}
		else {
			currentterrain = 1;
		}
	}
	if ((last_distance_travelled+1000) / smalltile < (int)(distance_travelled) / smalltile) {
		int diff = (int)distance_travelled / smalltile - (last_distance_travelled+1000) / smalltile;
		int min = line - diff+1;
		for (line; line >= min; line--) {
			if (line == 0) {
				line += tiles_rows;
				min += tiles_rows;
			}
			//std::cout << "Loading line: " << line - 1 << std::endl;
			LoadLine(line-1);
		}
		last_distance_travelled += diff * smalltile;
	}
	game->texturemanager->ResetRenderTarget();
		
}

void Terrain::AddTexture(const char* texturepath) {
	SDL_Surface* mainsurface = game->texturemanager->LoadSurface(texturepath);
	SDL_Texture* maintexture = game->texturemanager->LoadTexture(texturepath);

	std::vector<std::vector<std::vector<SDL_Texture*>>> tempXvec;
	for (int x = 0; x < tiledivider; x++) {
		std::vector<std::vector<SDL_Texture*>> tempYvec;
		for (int y = 0; y < tiledivider; y++) {
			std::vector<SDL_Texture*> tempTextures;

			SDL_Texture* nofadetexture= SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, width / tiledivider, smalltile); //Copy part of tile to another texture
			SDL_SetRenderTarget(renderer, nofadetexture);
			SDL_Rect temprect;
			temprect.x = width / tiledivider * x;
			temprect.y = smalltile * y;
			temprect.w = width / tiledivider;
			temprect.h = smalltile;
			SDL_RenderCopy(renderer, maintexture, &temprect, NULL);
			tempTextures.push_back(nofadetexture);



			
			Uint32 rmask, gmask, bmask, amask; //Copy part of tile to another surface
			/* SDL interprets each pixel as a 32-bit number, so our masks must depend
			   on the endianness (byte order) of the machine */
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
						rmask = 0xff000000;
						gmask = 0x00ff0000;
						bmask = 0x0000ff00;
						amask = 0x000000ff;
			#else
						rmask = 0x000000ff;
						gmask = 0x0000ff00;
						bmask = 0x00ff0000;
						amask = 0xff000000;
			#endif
			SDL_Surface* partsurface = SDL_CreateRGBSurface(0, temprect.w, temprect.h, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(mainsurface, &temprect, partsurface, NULL);



			for (int mask = 0; mask < blending_masks.size(); mask++) {
				SDL_Surface* tempblendingmask = SDL_CreateRGBSurface(0, temprect.w, temprect.h, 32, rmask, gmask, bmask, amask);
				//SDL_Rect tempsrcrect{ 100,100,1000,1000 };
				SDL_BlitScaled(blending_masks[mask], NULL, tempblendingmask, NULL);

				SDL_Texture* temptexture = NULL;
				game->texturemanager->ComposeTexture(tempblendingmask, partsurface, &temptexture);
				SDL_BlitScaled(partsurface, NULL, SDL_GetWindowSurface(game->window), NULL);
				/*SDL_UpdateWindowSurface(game->window);
				SDL_FillRect(SDL_GetWindowSurface(game->window), NULL, 0x000000);*/
				//std::cout << tempblendingmask->h << partsurface->h << std::endl;
				/*SDL_SetRenderTarget(renderer, NULL);
				SDL_RenderCopy(renderer, temptexture, NULL, NULL);
				SDL_RenderPresent(renderer);*/
				//SDL_Delay(1000);
				tempTextures.push_back(temptexture);
				//std::cout << "Composing: " << mask << std::endl;
			}
			tempYvec.push_back(tempTextures);
		}
		tempXvec.push_back(tempYvec);
	}
	textures.push_back(tempXvec);
	SDL_SetRenderTarget(renderer, NULL);
}


void Terrain::RandomizeTerrain() {
	for (int terrainnum = 0; terrainnum < 2; terrainnum++) {
		for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
			for (int dstrect_y = 0; dstrect_y < tiles_rows; dstrect_y++) {
				tiles[terrainnum][dstrect_x][dstrect_y] = rand() % textures.size();
			}
		}
	}
	
}

void Terrain::CopyTilesToTexture() {
	for (int dstrect_y = 0; dstrect_y < tiles_rows; dstrect_y++) {
		int ypart = dstrect_y % tiledivider;
		ypart -= (int)distance_travelled % tiledivider;
		if (ypart < 0) {
			ypart += tiledivider;
		}
		for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
			int xpart = dstrect_x % tiledivider;
			for (int terrainnum = 0; terrainnum < 2; terrainnum++) {
				game->texturemanager->CopyTextureToTexture(textures[tiles[terrainnum][dstrect_x][dstrect_y]][xpart][ypart][rand()%blending_masks.size()], terrain[terrainnum], NULL, &dstrects[dstrect_x][dstrect_y]);
			}
		}
	}

	game->texturemanager->ResetRenderTarget();
}

void Terrain::LoadLine(int dstrect_y) {
	int ypart = dstrect_y % tiledivider;
	for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
		int xpart = dstrect_x % tiledivider;
		tiles[!currentterrain][dstrect_x][dstrect_y] = rand() % textures.size();
		game->texturemanager->CopyTextureToTexture(textures[tiles[!currentterrain][dstrect_x][dstrect_y]][xpart][ypart][!currentterrain], terrain[!currentterrain], NULL, &dstrects[dstrect_x][dstrect_y]);
	}
}