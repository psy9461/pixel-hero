#pragma once

#define MAX_CHAR	6
#define MAX_PROJ	1
#define KIND_TILE	6
#define MAX_TILE	24


struct TileData {
	const char* path;
	int tileCnt;
	const char* tileName[MAX_TILE];
};

extern const char* _sprPathChar[MAX_CHAR];
extern const char* _sprPathProj[MAX_PROJ];
extern TileData _tileData[KIND_TILE];

extern const char* pathEntity[MAX_CHAR];

extern const char* bgImgPath[1];


