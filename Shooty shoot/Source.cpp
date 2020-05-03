#include "Prop.h"
#include "Mission.h"

int main(int argc, char* args[])
{
	static Game* game = new Game();
	TextureManager texturemanager = TextureManager(game->renderer);
	game->Initialize(&texturemanager);
	
	Mission missions = Mission();
	int missionnum = 1;
	while (game->running) {
		switch (missionnum)
		{
		case 1:
			missions.mission1(game);
		default:
			break;
		}
	}
	

	SDL_Quit();
	return 0;
}