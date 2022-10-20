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
		SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		return false;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

	map.Generate(525, 150);

	SDL_Surface* surface = IMG_Load("Data/textures.png");
	textures = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

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
			if (sdlEvent.key.keysym.sym == SDLK_n)
			{
				night = !night;
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
				if (mapX > (525 - width) * 16)
				{
					mapX = (525 - width) * 16;
				}

				if (mapY < 0)
				{
					mapY = 0;
				}
				if (mapY > 149 * 16 - screenHeight)
				{
					mapY = 149 * 16 - screenHeight;
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
	if (night)
	{
		SDL_SetRenderDrawColor(renderer, 12, 58, 97, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 97, 143, 182, 255);
	}
	SDL_RenderClear(renderer);

	int beginX = mapX / 16;
	int beginY = mapY / 16;

	lights.Generate(map, beginX, beginY, night);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			SDL_Rect dstRect = { x * 16 - mapX % 16, y * 16 - mapY % 16, 16, 16 };

			if (lights.GetIntensity(x, y) > 0.0f)
			{
				Tile tile = map.GetTile(beginX + x, beginY + y);

				if (tile.block != Type::NONE || tile.wall != Type::NONE)
				{
					int spriteX = 0;
					int spriteY = 0;

					Type Type;

					if (tile.block != Type::NONE)
					{
						Type = tile.block;
						spriteY = 0;
					}
					else if (tile.wall != Type::NONE)
					{
						Type = tile.wall;
						spriteY = 1;
					}

					switch (Type)
					{
					case Type::DIRT:
						spriteX = 1;
						break;
					case Type::GRASS:
						spriteX = 0;
						break;
					case Type::STONE:
						spriteX = 2;
					}

					SDL_Rect srcRect = { spriteX * 4, spriteY * 4, 4, 4 };
					SDL_RenderCopy(renderer, textures, &srcRect, &dstRect);
				}
				
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 * (1.0f - lights.GetIntensity(x, y)));
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
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	SDL_Quit();
}