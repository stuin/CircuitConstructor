#pragma once

#include "Skyrmion/GridMaker.h"

enum CollisionLayer {
	BACKGROUND,
	TREEMAP,
	MAP,
	TEMPMAP,
	BUTTON,
	BOX,
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

enum GrowthType {
	TREEEMPTY,
	NONE,
	TREE,
	SNOWTREE,
	ROCK,
	SNOWROCK
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
	{'"', 0+24},
	{'T', 1+24},
	{'N', 7+24},
	{'H', 6+24},
	{'_', 8+24},
	{'f', 3+24},
	{'b', 9+24},
	{'>', 4+24},
	{'<', 10+24},
	{' '+SNOW_OFFSET, -1},
	{'/'+SNOW_OFFSET, 8+12},
	{'-'+SNOW_OFFSET, 0+12},
	{'\\'+SNOW_OFFSET,8+12+144},
	{'#'+SNOW_OFFSET, 10+12},
	{'\''+SNOW_OFFSET, 6+12},
	{'['+SNOW_OFFSET, 4+12},
	{']'+SNOW_OFFSET, 4+12+144},
	{'"'+SNOW_OFFSET, 0+24},
	{'T'+SNOW_OFFSET, 1+24},
	{'N'+SNOW_OFFSET, 7+24},
	{'H'+SNOW_OFFSET, 6+24},
	{'_'+SNOW_OFFSET, 8+24},
	{'f'+SNOW_OFFSET, 3+24},
	{'b'+SNOW_OFFSET, 9+24},
	{'>'+SNOW_OFFSET, 4+24},
	{'<'+SNOW_OFFSET, 10+24},
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
	{'~', 0+24},
	{'~'+SNOW_OFFSET, 0+24}
};

static const std::map<unsigned int, int> treeGrowIndex = {
	{' ', TREEEMPTY},
	{'-', TREE},
	{'[', TREE},
	{']', TREE},
	{'#', ROCK},
	{'-'+SNOW_OFFSET, SNOWTREE},
	{'['+SNOW_OFFSET, SNOWTREE},
	{']'+SNOW_OFFSET, SNOWTREE},
	{'#'+SNOW_OFFSET, SNOWROCK}
};

static const std::map<unsigned int, int> treeDisplayIndex = {
	{1, 5},
	{2, 5},
	{3, 10},
	{4, 15},
	{10, 18},
	{11, 19},
	{12, 13},
	{13, 14},
	{14, 8},
	{15, 9},
	{16, 3},
	{17, 4},
	{20, 18},
	{21, 19},
	{22, 13},
	{23, 14},
	{24, 8},
	{25, 9},
	{26, 3},
	{27, 4},
};

static const std::map<unsigned int, int> decorDisplayIndex = {
	{' ', -1},
	{'f', 0},
	{'r', 4},
	{'p', -1},
	{'b', 12},
	{'f'+SNOW_OFFSET, 2},
	{'r'+SNOW_OFFSET, 6},
	{'p'+SNOW_OFFSET, 8},
	{'b'+SNOW_OFFSET, 14}
};

static const std::map<unsigned int, int> decorRandomIndex = {
	{'f', 2},
	{'r', 2},
	{'p', 1},
	{'b', 1},
	{'f'+SNOW_OFFSET, 2},
	{'r'+SNOW_OFFSET, 2},
	{'p'+SNOW_OFFSET, 3},
	{'b'+SNOW_OFFSET, 1}
};

static const std::map<unsigned int, int> foregroundDisplayIndex = {
	{' ', -1},
	{1, 0},
	{2, 7},
	{11, 4},
	{12, 5},
	{21, 13},
	{22, 6}
};

static const std::map<unsigned int, int> foregroundRandomIndex = {
	{' ', -1},
	{1, 4},
	{2, 5}
};

