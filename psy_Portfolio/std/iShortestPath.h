#pragma once

#include "iType.h"

struct SPTile
{
	int value;
	bool visit;
	int* path;
	int num;
};

class iShortestPath
{
public:
	iShortestPath();
	virtual ~iShortestPath();

	void init(int tileX, int tileY);
	void run(int* mapTile, int start, int end, int* path, int& num);
public:
	int tileX, tileY, tileXY;
	SPTile* tile;
};

#define SP_INF 99

