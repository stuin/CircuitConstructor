#pragma once

#include "Skyrmion/GridMaker.h"

enum CollisionLayer {
	BACKGROUND,
	TREEMAP,
	MAP,
	TEMPMAP,
	BOX,
	BUTTON,
	PLAYER,
	SECTION,
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
	SLOPERIGHT,
	TEMPPLATFORM
};

enum TreeType {
	TREEEMPTY,
	NONE,
	TREE
};

#define SNOW_OFFSET 100

static const std::map<unsigned int, int> displayIndex = {
	{' ', -1},
	{'/', 8},
	{'-', 0},
	{'\\', 8+168},
	{'#', 10},
	{'\'', 6},
	{'[', 4},
	{']', 4+168},
	{',', 12},
	{'"', 0+18+18},
	{'T', 1+18+18},
	{'N', 2+18+18},
	{'_', 3+18+18},
	{'H', 4+18+18},
	{'f', 5+18+18},
	{' '+SNOW_OFFSET, -1},
	{'/'+SNOW_OFFSET, 8+18},
	{'-'+SNOW_OFFSET, 0+18},
	{'\\'+SNOW_OFFSET,8+18+168},
	{'#'+SNOW_OFFSET, 10+18},
	{'\''+SNOW_OFFSET, 6+18},
	{'['+SNOW_OFFSET, 4+18},
	{']'+SNOW_OFFSET, 4+18+168},
	{','+SNOW_OFFSET, 12+18},
	{'"'+SNOW_OFFSET, 0+18+18},
	{'T'+SNOW_OFFSET, 1+18+18},
	{'N'+SNOW_OFFSET, 2+18+18},
	{'_'+SNOW_OFFSET, 3+18+18},
	{'H'+SNOW_OFFSET, 4+18+18},
	{'f'+SNOW_OFFSET, 5+18+18}
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
	{'"', PLATFORM},
	{'~', TEMPPLATFORM},
	{'T', PLATFORM},
	{'_', EMPTY},
	{'H', PLATFORM},
	{' '+SNOW_OFFSET, EMPTY},
	{'/'+SNOW_OFFSET, SLOPERIGHT},
	{'-'+SNOW_OFFSET, FULL},
	{'\\'+SNOW_OFFSET, SLOPELEFT},
	{'#'+SNOW_OFFSET, FULL},
	{'\''+SNOW_OFFSET, PLATFORM},
	{'['+SNOW_OFFSET, FULL},
	{']'+SNOW_OFFSET, FULL},
	{','+SNOW_OFFSET, EMPTY},
	{'"'+SNOW_OFFSET, PLATFORM},
	{'T'+SNOW_OFFSET, PLATFORM},
	{'_'+SNOW_OFFSET, EMPTY},
	{'H'+SNOW_OFFSET, PLATFORM}
};

static const std::map<unsigned int, int> frictionIndex = {
	{' ', 100},
	{' '+SNOW_OFFSET, 20},
	{'/'+SNOW_OFFSET, 20},
	{'-'+SNOW_OFFSET, 20},
	{'\\'+SNOW_OFFSET, 20},
	{'#'+SNOW_OFFSET, 20},
	{'\''+SNOW_OFFSET, 20},
	{'['+SNOW_OFFSET, 20},
	{']'+SNOW_OFFSET, 20}
};

static const std::map<unsigned int, int> tempDisplayIndex = {
	{'~', 0+18+18},
	{'~'+SNOW_OFFSET, 0+18+18}
};

static const std::map<unsigned int, int> treeGrowIndex = {
	{' ', TREEEMPTY},
	{'-', TREE},
	{'#', TREE},
	{'[', TREE},
	{']', TREE},
	{'-'+SNOW_OFFSET, TREE},
	{'#'+SNOW_OFFSET, TREE},
	{'['+SNOW_OFFSET, TREE},
	{']'+SNOW_OFFSET, TREE}
};

static const std::map<unsigned int, int> treeDisplayIndex = {
	{'t', 0},
	{'T', 1}
};

