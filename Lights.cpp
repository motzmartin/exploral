#include "Lights.h"

void Lights::Initialize(int w, int h)
{
	width = w;
	height = h;

	std::vector<float> line;

	for (int i = 0; i < width; i++)
	{
		line.push_back(0.0f);
	}

	for (int i = 0; i < height; i++)
	{
		lights.push_back(line);
	}
}

void Lights::Generate(Map& map, int beginX, int beginY, int mouseX, int mouseY)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Tile tile = map.GetTile(beginX + x, beginY + y);

			if ((x == mouseX && y == mouseY) || tile.block == Block::VOID && tile.wall == Wall::NONE)
			{
				lights[y][x] = 1.0f;
			}
			else
			{
				lights[y][x] = 0.0f;
			}
		}
	}

	bool stop = false;
	bool firstIteration = true;

	while (!stop)
	{
		stop = true;

		std::vector<std::vector<float>> temp = lights;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Tile tile = map.GetTile(beginX + x, beginY + y);

				if (tile.block != Block::VOID || tile.wall != Wall::NONE)
				{
					float max = 0.0f;

					if (x > 0 && temp[y][x - 1] > max)
					{
						max = temp[y][x - 1];
					}
					if (y > 0 && temp[y - 1][x] > max)
					{
						max = temp[y - 1][x];
					}
					if (x < width - 1 && temp[y][x + 1] > max)
					{
						max = temp[y][x + 1];
					}
					if (y < height - 1 && temp[y + 1][x] > max)
					{
						max = temp[y + 1][x];
					}

					if (max > 0.0f)
					{
						float finalIntensity;

						if (firstIteration)
						{
							finalIntensity = max;
						}
						else
						{
							if (tile.block != Block::VOID)
							{
								finalIntensity = max - 0.15f;
							}
							else
							{
								finalIntensity = max - 0.05f;
							}
						}

						if (finalIntensity < 0.0f)
						{
							finalIntensity = 0.0f;
						}

						if (finalIntensity > lights[y][x])
						{
							lights[y][x] = finalIntensity;

							stop = false;
						}
					}
				}
			}
		}

		if (firstIteration)
		{
			firstIteration = false;
		}
	}
}

float Lights::GetIntensity(int x, int y)
{
	return lights[y][x];
}