#include "Map.h"

void Map::Initialize(int w, int h)
{
	width = w;
	height = h;

	tiles = new Tile* [h]();

	for (int i = 0; i < h; i++)
	{
		tiles[i] = new Tile[w]();
	}
}

void Map::Generate()
{
	std::random_device dev;
	std::mt19937 rng(dev());

	surface.SetSeed(rng());

	caves.SetSeed(rng());
	caves.SetFractalType(FastNoiseLite::FractalType_Ridged);
	caves.SetFractalOctaves(1);

	stone.SetSeed(rng());

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tiles[y][x] = GenByCoords(x, y);
		}
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (tiles[y][x].block == Block::DIRT)
			{
				if (y > 0 && IsEmpty(x, y - 1))
				{
					tiles[y][x].block = Block::GRASS;
				}
				else if (y < height - 1 && tiles[y + 1][x].block == Block::VOID)
				{
					tiles[y][x].block = Block::COMPACT_DIRT;
					tiles[y + 1][x].wall = Wall::COMPACT_DIRT;
				}
			}
		}
	}
}

Tile Map::GenByCoords(int x, int y)
{
	if (y - 250.0f > stone.GetNoise((float)x, 0.0f) * 20.0f)
	{
		return { Block::STONE, Wall::STONE };
	}
	else if (y - 50.0f > surface.GetNoise((float)x, 0.0f) * 10.0f)
	{
		if (caves.GetNoise((float)x, (float)y) > 0.5f)
		{
			if (y - 230.0f > stone.GetNoise((float)x, 0.0f) * 20.0f)
			{
				return { Block::VOID, Wall::STONE };
			}
			else
			{
				return { Block::VOID, Wall::DIRT };
			}
		}
		else
		{
			return { Block::DIRT, Wall::DIRT };
		}
	}

	return { Block::VOID, Wall::NONE };
}

bool Map::IsEmpty(int x, int y)
{
	return tiles[y][x].block == Block::VOID && tiles[y][x].wall == Wall::NONE;
}

void Map::Set(int x, int y, Block block, Wall wall)
{
	tiles[y][x].block = block;
	tiles[y][x].wall = wall;
}

Tile Map::Get(int x, int y)
{
	return tiles[y][x];
}

int Map::GetWidth()
{
	return width;
}

int Map::GetHeight()
{
	return height;
}

void Map::Destroy()
{
	for (int i = 0; i < height; i++)
	{
		delete[] tiles[i];
	}

	delete[] tiles;
}