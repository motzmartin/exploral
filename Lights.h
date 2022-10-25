#pragma once

#include "Map.h"

class Lights
{
public:
	void Initialize(int w, int h);
	void Generate(Map& map, int beginX, int beginY, int mouseX, int mouseY);
	float GetIntensity(int x, int y);
	void Destroy();

private:
	float** lights;

	int width;
	int height;
};