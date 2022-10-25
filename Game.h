#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Map.h"
#include "Lights.h"

class Game
{
public:
	bool Initialize();
	bool PollEvents();
	void Update();
	void RenderFrame();
	void Destroy();

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Texture* textures = nullptr;

	int width;
	int height;

	int screenWidth = 1280;
	int screenHeight = 720;

	int mapX = 0;
	int mapY = 0;

	bool mouseLeftDown = false;
	bool mouseRightDown = false;
	bool enableLightsUpdate = true;
	bool fullscreen = false;

	Map map;
	Lights lights;
};