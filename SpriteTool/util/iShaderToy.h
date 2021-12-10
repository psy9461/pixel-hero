#pragma once

#include "iType.h"
#include "iPoint.h"
#include "iOpenGL.h"

// 생성할때 필요한 타입
struct STFrag
{
	const char* strFrag;
	Texture* texs[4];
	int bufs[4];
	TextureFilter filter[4];
	TextureWrap wrap[4];
};

struct STInfo
{
	const char* strVert;
	STFrag frag[5];
};

// 사용할때 필요한 타입
struct STBind
{
	Texture* texs[4];
	int bufs[4];
	TextureFilter filter[4];
	TextureWrap wrap[4];
};


class iShaderToy
{
public:
	iShaderToy(STInfo* info);
	virtual ~iShaderToy();

	void paint(float dt, iPoint off);
	void key(iKeyStat stat, iPoint point);

public:
	GLuint vbo, vbe;
	GLuint* programID;

	Texture*** texFbo;
	STBind* bind;

	float takeTime;
	float* mouse;// 4
	bool onMouse;
	int* keyboard;
	uint32 frame;

	bool toggle;
};
