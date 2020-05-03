#include "Cloud.h"

Cloud::Cloud(Game* game) {
	texture = game->texturemanager->LoadTexture("Textures/Cloud.png");
	this->game = game;
	game->texturemanager->GetTextureWH(texture, &width, &height);

	xOffset = (game->texturemanager->resolution[0] % width)/2;
	yOffset = (game->texturemanager->resolution[1] % height)/2;

	preciseX = -xOffset - width;
	preciseY = -yOffset - width;

	for (int cloudx = 0; cloudx < game->texturemanager->resolution[0] / width+2; cloudx++) {
		std::vector<SDL_Rect> tempvector;
		std::cout << "Creating x " << cloudx << std::endl;
		for (int cloudy = 0; cloudy < game->texturemanager->resolution[1] / height+2; cloudy++) {
			std::cout << "Creating y " << cloudy << std::endl;
			SDL_Rect temprect;
			temprect.x = -xOffset + cloudx * width;
			temprect.y = -yOffset + cloudy * height;

			temprect.w = width;
			temprect.h = height;


			tempvector.push_back(temprect);
		}
		dstrects.push_back(tempvector);
		std::cout << std::endl;
	}


	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = width;
	srcrect.h = height;

	std::cout << "Loaded clouds.." << std::endl;
}

void Cloud::Draw() {
	for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
		for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {
			SDL_RenderCopy(game->renderer, texture, &srcrect, &dstrects[cloudx][cloudy]);
		}
	}
}

void Cloud::Move(double x, double y) {
	for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
		for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {
			preciseX += x/game->fps;
			preciseY += y / game->fps;

			dstrects[cloudx][cloudy].x = preciseX+width*cloudx;
			dstrects[cloudx][cloudy].y = preciseY+height*cloudy;
		}
	}
}

void Cloud::Buffer() {
	if (preciseX < -xOffset-width) {
		preciseX += width;
		for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
			for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {
				dstrects[cloudx][cloudy].x = preciseX + width * cloudx;
				dstrects[cloudx][cloudy].y = preciseY + height * cloudy;
			}
		}
	}
	if (preciseX > -xOffset) {
		preciseX -= width;
		for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
			for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {
				dstrects[cloudx][cloudy].x = preciseX + width * cloudx;
				dstrects[cloudx][cloudy].y = preciseY + height * cloudy;
			}
		}
	}
	if (preciseY < -yOffset - height) {
		preciseY += height;
		for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
			for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {
				dstrects[cloudx][cloudy].x = preciseX + width * cloudx;
				dstrects[cloudx][cloudy].y = preciseY + height * cloudy;
			}
		}
	}
	if (preciseY > -yOffset) {
		preciseY -= height;
		for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
			for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {
				dstrects[cloudx][cloudy].x = preciseX + width * cloudx;
				dstrects[cloudx][cloudy].y = preciseY + height * cloudy;
			}
		}
	}
}

void Cloud::FollowTerrain(Terrain* terrain) {
	preciseY += terrain->speed / game->fps;
	for (int cloudx = 0; cloudx < dstrects.size(); cloudx++) {
		for (int cloudy = 0; cloudy < dstrects[cloudy].size(); cloudy++) {

			dstrects[cloudx][cloudy].x = preciseX + width * cloudx;
			dstrects[cloudx][cloudy].y = preciseY + height * cloudy;
		}
	}
}