#pragma once

#include "iType.h"

#include "iArray.h"
#include "iRect.h"
struct Tex
{
	Texture* tex;
	char* path;
};

Texture* addTextureInTex(const char* path);
bool delTextureInTex(const char* path);

extern Tex* _tex;
struct Frame
{
	Tex* t;
	iPoint p;
	float delta;
	iRect rtAtt;
	iRect rtDef;
};

class Sprite
{
#if 1
public:
	void save(const char* fileName);
#endif

public:
	Sprite();

	Sprite(const char* szForamt, ...);
	virtual ~Sprite();

	static void cbBe(void* parm);
	static void cbFrame(void* parm);

	void set(int be);
	void paint(float dt, iPoint off);

	iRect getRtAtt(bool reverse = false);
	iRect getRtDef(bool reverse = false);

public:
	iArray* arrayBe;

	int be;

	int index;
	float delta;
	iPoint position;

	// ani speed up x 2
	float scale;
	// leftRight
};

