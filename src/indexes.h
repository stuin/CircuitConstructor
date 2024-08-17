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
	SLOPELEFT,
	SLOPERIGHT
};

static const std::map<char, int> displayIndex = {
	{' ', -1},
	{'P', -1},
	{'b', -1},
	{'D', 3},
	{'-', 1},
	{'h', 4},
	{'\\', 2},
	{'/', 0}
};

static const std::map<char, int> collisionIndex = {
	{' ', EMPTY},
	{'P', EMPTY},
	{'b', EMPTY},
	{'D', FULL},
	{'-', FULL},
	{'h', EMPTY},
	{'\\', SLOPELEFT},
	{'/', SLOPERIGHT}
};