#pragma once

#include "iType.h"

#include "iStd.h"

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


typedef void (*MethodAfter)(void* parm);

class Sprite
{
#if 1
public:
	void save(const char* fileName);
#endif

public:
	Sprite();

	Sprite(const char* szFormat, ...);
	virtual ~Sprite();

	static void cbBe(void* parm);
	static void cbFrame(void* parm);

	void paint(float dt, iPoint pos, MethodAfter m, void* parm, int be, int& index, float& delta,  bool leftRight = false, float scale = 1.0f, bool repeat = true);

	iRect getRtAtt(int be, int index, iPoint pos, float scale, bool reverse = false);
	iRect getRtDef(int be, int index, iPoint pos, float scale, bool reverse = false);

public:
	iArray* arrayBe;
};



