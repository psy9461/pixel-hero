#pragma once

#include "iType.h"

#include "Common.h"

#include "Entity.h"

struct Tile;
extern Tile* tileMap;

extern Texture*** tileImgs;
extern float tileScale;

extern Texture* texMinimap;

extern Enemy* boss;

void loadMap(int level, const char* stageIndex);
void freeMap();
iPoint drawMap();

// ---------------------------------
//   Map
// ---------------------------------

#define TILEWIDTH	40
#define TILEHEIGHT	40

#define THEME_INTERVAL 1000

enum GroundType
{
	None = 0,
	Solid,
	Logged,
	Slippery
};

extern int tileW, tileH, tileX, tileY, tileNum, maxTileX;

struct TileInfo
{
	GroundType gt;
	bool pass;
};

struct Tile
{
	int value;
	int level = -1;
	TileInfo ti;
};




