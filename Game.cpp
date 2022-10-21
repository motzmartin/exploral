#include "Game.h"

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	int flags = IMG_INIT_PNG;

	int initted = IMG_Init(flags);

	if ((initted & flags) != flags)
	{
		return false;
	}

	window = SDL_CreateWindow("Exploral",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		0);

	if (window == nullptr)
	{
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == nullptr)
	{
		return false;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Surface* surface = IMG_Load("./Data/Textures.png");
	if (surface == nullptr)
	{
		return false;
	}
	textures = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (screenWidth % 16 == 0)
	{
		width = screenWidth / 16 + 1;
	}
	else
	{
		width = screenWidth / 16 + 2;
	}

	if (screenHeight % 16 == 0)
	{
		height = screenHeight / 16 + 1;
	}
	else
	{
		height = screenHeight / 16 + 2;
	}

	map.Generate(3000, 300);
	mapX = (map.GetWidth() / 2 - width / 2) * 16;
	mapY = 256;

	lights.Initialize(width, height);

	return true;
}

bool Game::PollEvents()
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_l)
			{
				enableLightsUpdate = !enableLightsUpdate;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
			{
				mouseDown = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
			{
				mouseDown = false;
			}
			break;
		case SDL_MOUSEMOTION:
			if (mouseDown)
			{
				mapX -= sdlEvent.motion.xrel;
				mapY -= sdlEvent.motion.yrel;

				if (mapX < 0)
				{
					mapX = 0;
				}
				if (mapX > (map.GetWidth() - width) * 16)
				{
					mapX = (map.GetWidth() - width) * 16;
				}

				if (mapY < 0)
				{
					mapY = 0;
				}
				if (mapY > (map.GetHeight() - 1) * 16 - screenHeight)
				{
					mapY = (map.GetHeight() - 1) * 16 - screenHeight;
				}
			}
		}
	}

	return true;
}

void Game::Update()
{
}

void Game::RenderFrame()
{
	SDL_SetRenderDrawColor(renderer, 97, 143, 182, 255);
	SDL_RenderClear(renderer);

	int beginX = mapX / 16;
	int beginY = mapY / 16;

	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouseX = (mouseX + mapX % 16) / 16;
	mouseY = (mouseY + mapY % 16) / 16;

	if (enableLightsUpdate)
	{
		lights.Generate(map, beginX, beginY, mouseX, mouseY);
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			SDL_Rect dstRect = { x * 16 - mapX % 16, y * 16 - mapY % 16, 16, 16 };
			float lightIntensity = lights.GetIntensity(x, y);

			if (lightIntensity > 0.0f)
			{
				Tile tile = map.GetTile(beginX + x, beginY + y);

				if (tile.block != Block::VOID || tile.wall != Wall::NONE)
				{
					int spriteX = 0;
					int spriteY = 0;

					if (tile.block != Block::VOID)
					{
						spriteY = 0;
						switch (tile.block)
						{
						case Block::GRASS:
							spriteX = 0;
							break;
						case Block::DIRT:
							spriteX = 1;
							break;
						case Block::COMPACT_DIRT:
							spriteX = 2;
							break;
						case Block::STONE:
							spriteX = 3;
						}
					}
					else if (tile.wall != Wall::NONE)
					{
						spriteY = 1;
						switch (tile.wall)
						{
						case Wall::GRASS:
							spriteX = 0;
							break;
						case Wall::DIRT:
							spriteX = 1;
							break;
						case Wall::COMPACT_DIRT:
							spriteX = 2;
							break;
						case Wall::STONE:
							spriteX = 3;
						}
					}

					SDL_Rect srcRect = { spriteX * 4, spriteY * 4, 4, 4 };
					SDL_RenderCopy(renderer, textures, &srcRect, &dstRect);
				}

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)(255 * (1.0f - lightIntensity)));
				SDL_RenderFillRect(renderer, &dstRect);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &dstRect);
			}
		}
	}

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}