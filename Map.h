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
	void Generate(int width, int height);
	Tile GetTile(int x, int y);

private:
	Tile Get(int x, int y);

	std::vector<std::vector<Tile>> tiles;

	FastNoiseLite surfaceLayerVariations;
	FastNoiseLite surfaceHoles;
	FastNoiseLite surfaceWallHoles;
	FastNoiseLite caves;
};