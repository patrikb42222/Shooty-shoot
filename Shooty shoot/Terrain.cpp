#include "Terrain.h"

Terrain::Terrain(Game* game) {
	this->renderer = game->renderer;
	this->game = game;
	this->textures = textures;

	srand(SDL_GetTicks());


	tiles_coloumns = 32;



	for (int terraindst = 0; terraindst < 2; terraindst++) {
		std::vector<std::vector<int>> temp;
		tiles.push_back(temp);

		SDL_Rect tempdst = {0,0,1920,0};
		terraindsts.push_back(tempdst);
		terrain_BigTextures.push_back(NULL);
		/*terrain_BigTextures.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, terraindsts[terraindst].w, terraindsts[terraindst].h));
		precisey = terraindsts[terraindst].y;


		for (int coloumn = 0; coloumn < tiles_coloumns; coloumn++) {
			std::vector<int> tempX;
			for (int row = 0; row < tiles_rows; row++) {
				tempX.push_back(0);
			}
			tiles[terraindst].push_back(tempX);
		}*/


		/*for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
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
		}*/
	}

	for (int dstrect_y = 0; dstrect_y < tiles_rows; dstrect_y++) {
		for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
				
			dstrects[dstrect_x][dstrect_y].x = width * dstrect_x / tiledivider;
			dstrects[dstrect_x][dstrect_y].y = height * dstrect_y / tiledivider;
			precise_coords[dstrect_x][dstrect_y][0] = dstrects[dstrect_x][dstrect_y].x;
			precise_coords[dstrect_x][dstrect_y][1] = dstrects[dstrect_x][dstrect_y].y;
		
			dstrects[dstrect_x][dstrect_y].w = smalltile_width;
			dstrects[dstrect_x][dstrect_y].h = smalltile_height;
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
		SDL_RenderCopy(renderer, terrain_BigTextures[terraindst], NULL, &terraindsts[terraindst]);
		//SDL_RenderCopy(renderer, terrainCache[0], NULL, &terraindsts[terraindst]);
		std::cout << "XYWH = " << terraindsts[terraindst].x << " " << terraindsts[terraindst].y << " " << terraindsts[terraindst].w << " " << terraindsts[terraindst].h << std::endl;
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
		precisey -= terraindsts[currentterrain].h;
		if (currentterrain == 1) {
			currentterrain = 0;
		}
		else {
			currentterrain = 1;
		}
	}
	/*if (((last_distance_travelled) / smalltile_height) < (int)(distance_travelled / smalltile_height)) {
		int diff = (int)(distance_travelled / smalltile_height) - ((last_distance_travelled) / smalltile_height);
		int min = line - diff+1;
		for (line; line >= min; line--) {
			if (line == 0) {
				line += tiles_rows;
				min += tiles_rows;
				//currentterrain = !currentterrain; 
			}
			//std::cout << "Loading line: " << line - 1 << std::endl;
			LoadLine(line-1);
			last_distance_travelled += (diff)*smalltile_height;
		}
	}*/
	game->texturemanager->ResetRenderTarget();
		
}

void Terrain::AddTexture(const char* texturepath, SDL_Color color) {
	SDL_Surface* mainsurface = game->texturemanager->LoadSurface(texturepath);
	SDL_Texture* maintexture = game->texturemanager->LoadTexture(texturepath);

	std::vector<std::vector<std::vector<SDL_Texture*>>> tempXvec;
	for (int x = 0; x < tiledivider; x++) {
		std::vector<std::vector<SDL_Texture*>> tempYvec;
		for (int y = 0; y < tiledivider; y++) {
			std::vector<SDL_Texture*> tempTextures;

			SDL_Texture* nofadetexture= SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, width / tiledivider, smalltile_height); //Copy part of tile to another texture
			SDL_SetRenderTarget(renderer, nofadetexture);
			SDL_Rect temprect;
			temprect.x = smalltile_width * x;
			temprect.y = smalltile_height * y;
			temprect.w = smalltile_width;
			temprect.h = smalltile_height;
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
				SDL_BlitScaled(blending_masks[mask], NULL, tempblendingmask, NULL);

				SDL_Texture* temptexture = NULL;
				game->texturemanager->ComposeTexture(tempblendingmask, partsurface, &temptexture);
				tempTextures.push_back(temptexture);

				game->HandleEvents();
			}
			tempYvec.push_back(tempTextures);
		}
		tempXvec.push_back(tempYvec);
	}
	textures.push_back(tempXvec);
	SDL_SetRenderTarget(renderer, NULL);
	terrainColors.push_back(color);
}

void Terrain::AddTerrain(const char* texturepath) {
	terrainPaths.push_back(texturepath);
}
Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;

	case 2:
		return *(Uint16*)p;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;

	case 4:
		return *(Uint32*)p;

	default:
		std::cout << "ERROR, GETPIXEL DEFAULTED" << std::endl;
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}
void Terrain::LoadTerrainCache(TextureManager texturemanager) {
	for (int terrain = 0; terrain < terrainPaths.size(); terrain++) {
		//SDL_Surface* pixelSurface = texturemanager.LoadSurface(terrainPaths[terrain]);
		SDL_Surface* pixelSurface = SDL_LoadBMP(terrainPaths[terrain]);
		//SDL_BlitSurface(pixelSurface, NULL, SDL_GetWindowSurface(game->window), NULL);
		//std::cout << SDL_GetError() << std::endl;
		//SDL_UpdateWindowSurface(game->window);


		terrainCache.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, 1920, 1080));
		//std::cout << "ERROR: " << SDL_GetError() << "W: " << pixelSurface->w << " H: " << pixelSurface->h << std::endl;
		for (int y = 0; y < pixelSurface->h; y++) {
			for (int x = 0; x < pixelSurface->w; x++) {


				int textureNum = -1;
				/*pixelSurface = SDL_LoadBMP("Textures/map1.bmp");
				SDL_LockSurface(pixelSurface);
				Uint8* pixels = (Uint8*)pixelSurface->pixels;
				Uint8* pixel = pixels + y * pixelSurface->pitch + x;
				SDL_GetRGB(pixels[1], pixelSurface->format, &pixel[0], &pixel[1], &pixel[2]);
				std::cout << "RGB = " << (unsigned int)pixel[0] << (unsigned int)pixel[1] << (unsigned int)pixel[2] << std::endl;
				SDL_UnlockSurface(pixelSurface);*/



				Uint32 pixel = getpixel(pixelSurface, x, y);
				Uint8 pixel_8bit[4];
				SDL_GetRGB(pixel, pixelSurface->format, &pixel_8bit[0], &pixel_8bit[1], &pixel_8bit[2]);
				std::cout << "RGB = " << (unsigned int)pixel_8bit[0] << (unsigned int)pixel_8bit[1] << (unsigned int)pixel_8bit[2] << std::endl;




				for (int texture = 0; texture < textures.size(); texture++) {
					if (pixel_8bit[0] == terrainColors[texture].r && pixel_8bit[1] == terrainColors[texture].g && pixel_8bit[2] == terrainColors[texture].b) {
						textureNum = texture;
					}

						//std::cout << "R" << (unsigned int)pixel_8bit[0] << "==" << (unsigned int)terrainColors[texture].r << "=" << (pixel_8bit[0] == terrainColors[texture].r) << std::endl;
						//std::cout << "g" << (unsigned int)pixel_8bit[1] << "==" << (unsigned int)terrainColors[texture].g << "=" << (pixel_8bit[1] == terrainColors[texture].g) << std::endl;
						//std::cout << "b" << (unsigned int)pixel_8bit[2] << "==" << (unsigned int)terrainColors[texture].b << "=" << (pixel_8bit[2] == terrainColors[texture].b) << std::endl;
				}

				if (textureNum == -1) {
					//std::cout << "ERROR: texture not found with that pixel color!" << "RGB = " << (unsigned int)pixel_8bit[0] << " " << (unsigned int)pixel_8bit[1] << " " << (unsigned int)pixel_8bit[2] << std::endl;
				}
				else {
					//std::cout << "RGB = " << (unsigned int)pixel_8bit[0] << " " << (unsigned int)pixel_8bit[1] << " " << (unsigned int)pixel_8bit[2] << std::endl;
				}
				//std::cout << "XY = " << x << " " << y << std::endl;

				SDL_Rect dst = SDL_Rect();
				int scale = 1920 / 32;
				dst.x = x * scale;
				dst.y = y * scale;
				dst.w = scale;
				dst.h = scale;
				std::cout << textureNum << std::endl;
				texturemanager.CopyTextureToTexture(textures[textureNum][0][0][0], terrainCache[terrain], NULL, &dst);
				//texturemanager.CopyTextureToTexture(terrainCache[terrain], terrainCache[terrain], NULL, &dst);
			}
		}
	}
	texturemanager.ResetRenderTarget();
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
				game->texturemanager->CopyTextureToTexture(textures[tiles[terrainnum][dstrect_x][dstrect_y]][xpart][ypart][rand()%blending_masks.size()], terrain_BigTextures[terrainnum], NULL, &dstrects[dstrect_x][dstrect_y]);
			}
		}
	}

	game->texturemanager->ResetRenderTarget();
}

void Terrain::CopyTerrainFromCache(int terrain, TextureManager texturemanager) {
	//SDL_SetRenderTarget(renderer,terrain_BigTextures[0]);
	//SDL_SetRenderTarget(renderer,terrain_BigTextures[1]);
	//terrain_BigTextures[0] = terrainCache[0];
	//terrain_BigTextures[1] = terrainCache[0];
	int w, h;
	SDL_QueryTexture(terrainCache[0], NULL, NULL, &w, &h);
	terrain_BigTextures[terrain] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, w, h);
	texturemanager.CopyTextureToTexture(terrainCache[0], terrain_BigTextures[terrain], NULL, NULL);
	terraindsts[terrain].h = h;
	//texturemanager.CopyTextureToTexture(terrainCache[0], terrain_BigTextures[0], NULL, NULL);
	//texturemanager.CopyTextureToTexture(terrainCache[0], terrain_BigTextures[1], NULL, NULL);
	//SDL_RenderCopy(renderer, terrainCache[0], NULL, NULL);
}

void Terrain::LoadLine(int dstrect_y) {
	int ypart = dstrect_y % tiledivider;
	for (int dstrect_x = 0; dstrect_x < tiles_coloumns; dstrect_x++) {
		int xpart = dstrect_x % tiledivider;
		//tiles[!currentterrain][dstrect_x][dstrect_y] = rand() % (textures.size());
		tiles[!currentterrain][dstrect_x][dstrect_y] = dstrect_y%height<height/2;
		game->texturemanager->CopyTextureToTexture(textures[tiles[!currentterrain][dstrect_x][dstrect_y]][xpart][ypart][!currentterrain], terrain_BigTextures[!currentterrain], NULL, &dstrects[dstrect_x][dstrect_y]);
	}
}