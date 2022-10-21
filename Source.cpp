#include "Game.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")

int SDL_main(int argc, char* argv[])
{
	Game game;

	if (!game.Initialize())
	{
		return 1;
	}

	while (game.PollEvents())
	{
		game.Update();
		game.RenderFrame();
	}

	game.Destroy();

	return 0;
}