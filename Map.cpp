#include "Map.h"

void Map::Generate(int w, int h)
{
	width = w;
	height = h;

	std::random_device dev;
	std::mt19937 rng(dev());

	surface.SetSeed(rng());

	caves.SetSeed(rng());
	caves.SetFractalType(FastNoiseLite::FractalType_Ridged);
	caves.SetFractalOctaves(1);

	stone.SetSeed(rng());

	for (int y = 0; y < height; y++)
	{
		std::vector<Tile> line;
		for (int x = 0; x < width; x++)
		{
			line.push_back(Get(x, y));
		}
		tiles.push_back(line);
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (tiles[y][x].block == Block::DIRT)
			{
				if (y > 0 && tiles[y - 1][x].block == Block::VOID && tiles[y - 1][x].wall == Wall::NONE)
				{
					tiles[y][x].block = Block::GRASS;
				}
				else if (y < height - 1 && tiles[y + 1][x].block == Block::VOID)
				{
					tiles[y][x].block = Block::COMPACT_DIRT;
				}
			}
			else if (tiles[y][x].block == Block::VOID && tiles[y][x].wall == Wall::DIRT)
			{
				if (y > 0 && tiles[y - 1][x].block == Block::COMPACT_DIRT)
				{
					tiles[y][x].wall = Wall::COMPACT_DIRT;
				}
			}
		}
	}
}

Tile Map::Get(int x, int y)
{
	float noiseX = (float)x;
	float noiseY = (float)y;

	if (y - 250.0f > stone.GetNoise(noiseX, 0.0f) * 20.0f)
	{
		return { Block::STONE, Wall::STONE };
	}
	else if (y - 50.0f > surface.GetNoise(noiseX, 0.0f) * 10.0f)
	{
		if (caves.GetNoise(noiseX, noiseY) > 0.5f)
		{
			if (y - 230.0f > stone.GetNoise(noiseX, 0.0f) * 20.0f)
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

Tile Map::GetTile(int x, int y)
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