#pragma once

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
	void Initialize(int w, int h);
	void Generate();
	Tile Get(int x, int y);
	bool IsEmpty(int x, int y);
	void Set(int x, int y, Block block, Wall wall);
	int GetWidth();
	int GetHeight();
	void Destroy();

private:
	Tile GenByCoords(int x, int y);

	Tile** tiles;

	FastNoiseLite surface;
	FastNoiseLite caves;
	FastNoiseLite stone;

	int width;
	int height;
};