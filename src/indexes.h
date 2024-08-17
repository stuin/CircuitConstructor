#pragma once

#include "Skyrmion/GridMaker.h"

enum CollisionLayer {
	MAP,
	BOX,
	PLAYER,
	INPUT,
	TITLE
};

enum TileType {
	EMPTY,
	FULL,
	PLATFORM,
	SLOPELEFT,
	SLOPERIGHT
};

#define SNOW_OFFSET 10

static const std::map<char, int> displayIndex = {
	{' ', -1},
	{'P', -1},
	{'w', -1},
	{'/', 0},
	{'-', 1},
	{'\\', 2},
	{'#', 3},
	{'\'', 4},
	{' '+SNOW_OFFSET, -1},
	{'P'+SNOW_OFFSET, -1},
	{'w'+SNOW_OFFSET, -1},
	{'/'+SNOW_OFFSET, 10},
	{'-'+SNOW_OFFSET, 11},
	{'\\'+SNOW_OFFSET, 12},
	{'#'+SNOW_OFFSET, 13},
	{'\''+SNOW_OFFSET, 14}
};

static const std::map<char, int> collisionIndex = {
	{' ', EMPTY},
	{'P', EMPTY},
	{'w', EMPTY},
	{'/', SLOPERIGHT},
	{'-', FULL},
	{'\\', SLOPELEFT},
	{'#',  FULL},
	{'\'', PLATFORM},
	{' '+SNOW_OFFSET, EMPTY},
	{'P'+SNOW_OFFSET, EMPTY},
	{'w'+SNOW_OFFSET, EMPTY},
	{'/'+SNOW_OFFSET, SLOPERIGHT},
	{'-'+SNOW_OFFSET, FULL},
	{'\\'+SNOW_OFFSET, SLOPELEFT},
	{'#'+SNOW_OFFSET, FULL},
	{'\''+SNOW_OFFSET, PLATFORM}
};