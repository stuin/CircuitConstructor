#pragma once

#include "Skyrmion/GridMaker.h"

enum CollisionLayer {
	MAP,
	BOX,
	PLAYER,
	SECTION,
	CAMERA_SECTION,
	INPUT,
	TITLE
};

enum Signals {
	RESET_SECTION
};

enum TileType {
	EMPTY,
	FULL,
	PLATFORM,
	SLOPELEFT,
	SLOPERIGHT
};

#define SNOW_OFFSET 100

static const std::map<unsigned int, int> displayIndex = {
	{' ', -1},
	{'/', 8},
	{'-', 0},
	{'\\', 8+144},
	{'#', 10},
	{'\'', 6},
	{'[', 4},
	{']', 4+144},
	{',', 12},
	{' '+SNOW_OFFSET, -1},
	{'/'+SNOW_OFFSET, 8+18},
	{'-'+SNOW_OFFSET, 0+18},
	{'\\'+SNOW_OFFSET,8+18+144},
	{'#'+SNOW_OFFSET, 10+18},
	{'\''+SNOW_OFFSET, 6+18},
	{'['+SNOW_OFFSET, 4+18},
	{']'+SNOW_OFFSET, 4+18+144},
	{','+SNOW_OFFSET, 12+18}
};

static const std::map<unsigned int, int> randomizerIndex = {
	{'/', 2},
	{'-', 4},
	{'\\', 2},
	{'#', 2},
	{'\'', 2},
	{'[', 2},
	{']', 2},
	{',', 1},
	{'/'+SNOW_OFFSET, 2},
	{'-'+SNOW_OFFSET, 4},
	{'\\'+SNOW_OFFSET, 2},
	{'#'+SNOW_OFFSET, 2},
	{'\''+SNOW_OFFSET, 2},
	{'['+SNOW_OFFSET, 2},
	{']'+SNOW_OFFSET, 2},
	{','+SNOW_OFFSET, 3}
};

static const std::map<unsigned int, int> collisionIndex = {
	{' ', EMPTY},
	{'/', SLOPERIGHT},
	{'-', FULL},
	{'\\', SLOPELEFT},
	{'#',  FULL},
	{'\'', PLATFORM},
	{'[', FULL},
	{']', FULL},
	{',', EMPTY},
	{' '+SNOW_OFFSET, EMPTY},
	{'/'+SNOW_OFFSET, SLOPERIGHT},
	{'-'+SNOW_OFFSET, FULL},
	{'\\'+SNOW_OFFSET, SLOPELEFT},
	{'#'+SNOW_OFFSET, FULL},
	{'\''+SNOW_OFFSET, PLATFORM},
	{'['+SNOW_OFFSET, FULL},
	{']'+SNOW_OFFSET, FULL},
	{','+SNOW_OFFSET, EMPTY}
};