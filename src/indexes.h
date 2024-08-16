#pragma once

#include "Skyrmion/GridMaker.h"

enum CollisionLayer {
	MAP,
	PLAYER,
	INPUT,
	TITLE
};

static const std::map<char, int> displayIndex = {
	{' ', -1},
	{'#', 3},
	{'.', 4},
	{'P', 4}
};

static const std::map<char, int> collisionIndex = {
	{' ', 0},
	{'#', 0},
	{'.', 1},
	{'P', 1}
};