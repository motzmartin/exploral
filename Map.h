#pragma once

#include <vector>
#include <FastNoiseLite.h>

enum class Type
{
	NONE,
	GRASS,
	DIRT,
	STONE
};

struct Tile
{
	Type block;
	Type wall;
};

class Map
{
public:
	void Generate(int width_, int height_);
	Tile GetTile(int x, int y);
	int GetWidth();
	int GetHeight();

private:
	Tile Get(int x, int y);

	std::vector<std::vector<Tile>> tiles;

	FastNoiseLite surfaceLayerVariations;
	FastNoiseLite surfaceHoles;
	FastNoiseLite surfaceWallHoles;
	FastNoiseLite caves;

	int width;
	int height;
};