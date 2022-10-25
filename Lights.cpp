#include "Lights.h"

void Lights::Initialize(int w, int h)
{
	width = w;
	height = h;

	lights = new float* [h]();

	for (int i = 0; i < h; i++)
	{
		lights[i] = new float[w]();
	}
}

void Lights::Generate(Map& map, int beginX, int beginY, int mouseX, int mouseY)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int mapX = beginX + x;
			int mapY = beginY + y;

			if ((x == mouseX && y == mouseY || map.IsEmpty(mapX, mapY)) ||
				((x > 0 && map.IsEmpty(mapX - 1, mapY)) ||
				(y > 0 && map.IsEmpty(mapX, mapY - 1)) ||
				(x < map.GetWidth() - 1 && map.IsEmpty(mapX + 1, mapY)) ||
				(y < map.GetHeight() - 1 && map.IsEmpty(mapX, mapY + 1))))
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

	while (!stop)
	{
		stop = true;

		float** temp = new float* [height]();
		for (int i = 0; i < height; i++)
		{
			temp[i] = new float[width]();
			for (int j = 0; j < width; j++)
			{
				temp[i][j] = lights[i][j];
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Tile tile = map.Get(beginX + x, beginY + y);

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

						if (tile.block != Block::VOID)
						{
							finalIntensity = max - 0.15f;
						}
						else
						{
							finalIntensity = max - 0.05f;
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

		for (int i = 0; i < height; i++)
		{
			delete[] temp[i];
		}
		delete[] temp;
	}
}

float Lights::GetIntensity(int x, int y)
{
	return lights[y][x];
}

void Lights::Destroy()
{
	for (int i = 0; i < height; i++)
	{
		delete[] lights[i];
	}

	delete[] lights;
}