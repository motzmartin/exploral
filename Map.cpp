#include "Map.h"

void Map::Generate(int width, int height)
{
	surfaceLayerVariations.SetSeed(0);

	surfaceHoles.SetSeed(1);
	surfaceHoles.SetFrequency(0.02f);

	surfaceHoles.SetSeed(2);
	surfaceWallHoles.SetFrequency(0.05f);

	caves.SetSeed(3);
	caves.SetFrequency(0.02f);

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
			if (tiles[y][x].block == Type::DIRT)
			{
				if ((x > 0 && tiles[y][x - 1].block == Type::NONE && tiles[y][x - 1].wall == Type::NONE) ||
					(y > 0 && tiles[y - 1][x].block == Type::NONE && tiles[y - 1][x].wall == Type::NONE) ||
					(x < width - 1 && tiles[y][x + 1].block == Type::NONE && tiles[y][x + 1].wall == Type::NONE) ||
					(y < height - 1 && tiles[y + 1][x].block == Type::NONE && tiles[y + 1][x].wall == Type::NONE))
				{
					tiles[y][x].block = Type::GRASS;
				}
			}
		}
	}
}

Tile Map::Get(int x, int y)
{
	if (y > 50 + surfaceLayerVariations.GetNoise((float)x, 0.0f) * 10)
	{
		if (caves.GetNoise((float)x, (float)y) + (y / (float)400) * 2 - 1.0f > 0.0f)
		{
			return { Type::STONE, Type::STONE };
		}
		else if (surfaceHoles.GetNoise((float)x, (float)y) > 0.5f)
		{
			if (surfaceWallHoles.GetNoise((float)x, (float)y) < 0.8f)
			{
				return { Type::NONE, Type::DIRT };
			}
		}
		else
		{
			return { Type::DIRT, Type::DIRT };
		}
	}

	return { Type::NONE, Type::NONE };
}

Tile Map::GetTile(int x, int y)
{
	return tiles[y][x];
}