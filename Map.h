#pragma once

#include <vector>
#include <FastNoiseLite.h>
#include <random>

enum class Block
{
	VOID,
	GRASS,
	DIRT,
	COMPACT_DIRT,
	STONE
};

enum class Wall
{
	NONE,
	GRASS,
	DIRT,
	COMPACT_DIRT,
	STONE
};

struct Tile
{
	Block block;
	Wall wall;
};

class Map
{
public:
	void Generate(int w, int h);
	Tile GetTile(int x, int y);
	int GetWidth();
	int GetHeight();

private:
	Tile Get(int x, int y);

	std::vector<std::vector<Tile>> tiles;

	FastNoiseLite surface;
	FastNoiseLite caves;
	FastNoiseLite stone;

	int width;
	int height;
};