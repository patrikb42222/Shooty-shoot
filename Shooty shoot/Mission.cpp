#include "Mission.h"

void Mission::mission1(Game* game) {
	bool mission_running = true;

	//Prop tires = Prop(0, 0, 50, 50, 1, "Textures/Tires.png", game->renderer);
	Hull bike = Hull(0, 0, 68/2, 141/2, 3, "Textures/Bike.png", *game->texturemanager);
	Terrain terrain = Terrain(game);
	terrain.GetMaskTextures();
	terrain.AddTexture("Textures/TMGras07a.png");
	terrain.AddTexture("Textures/TMGras08a.png");
	terrain.AddTexture("Textures/TMGras09a.png");
	terrain.CopyTilesToTexture();
	std::vector<Prop*> proplist;

	
	//proplist.push_back(new Prop(20, 0, 0, 1920, 1080, 1, "Textures/River.png", game, &terrain));
	//proplist[0]->AddHeightRect(0, 300, 1920, 550, 0.4);
	proplist.push_back(new Prop(100, 0, 0, 300, 500, 1, "Textures/Ramp.png", game, &terrain));
	proplist[0]->AddHeightRect(0, 0, 300, 100, 1.6);
	proplist[0]->AddHeightRect(0, 100, 300, 100, 1.4);
	proplist[0]->AddHeightRect(0, 200, 300, 100, 1.2);

	Cloud clouds = Cloud(game);


	//terrain.CopyTilesToTexture();
	//SDL_Texture* temp = game->texturemanager->LoadTexture("Textures/Demotrap.png");
	while (mission_running) {
		//game->texturemanager->CopyTextureToTexture(temp, terrain.terrain, NULL,NULL);
		terrain.Draw();
		game->HandleEvents();
		terrain.Move();
		terrain.Buffer();
		game->moveHulls(&bike, 1);
		bike.Fall(0.005);
		if (game->keyboardstate[SDL_SCANCODE_LCTRL]) {
			/*if (terrain.speed < 1000) {
				terrain.speed+=3000/game->fps;
			}*/
			terrain.speed = 500;
		}
		else if (game->keyboardstate[SDL_SCANCODE_SPACE]){
			/*if (terrain.speed > 0) {
				terrain.speed-=1000 / game->fps;
			}*/
			terrain.speed = -500;
		}
		else {
			terrain.speed = 0;
		}
		//tires.Draw();
		for (int prop = 0; prop < proplist.size(); prop++) {
			if (proplist[prop]->OnScreen()) {
				if (proplist[prop]->Behind()) {
					//std::cout << "behind:" << prop << std::endl;
					proplist.erase(proplist.begin()+prop);
					continue;
				}
				proplist[prop]->Draw();
				proplist[prop]->FollowTerrain();
				bike.SetSize(proplist[prop]->GetHeight(bike.dstrect.x-proplist[prop]->dstrect.x, bike.dstrect.y-proplist[prop]->dstrect.y));
			}
		}

		
		bike.Draw();
		clouds.Draw();
		clouds.Move(1, 0);
		clouds.Buffer();
		clouds.FollowTerrain(&terrain);

		game->Draw();

		if (!game->running) {
			mission_running = false;
		}


		game->LimitFPS();
		game->GetFPS();
	}
}