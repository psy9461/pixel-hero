#pragma once

#include "iType.h"
#include "iArray.h"
#include "iPoint.h"
#include <stdio.h>

#include "iRect.h"
#include "iSize.h"

typedef void (*ANIMATION_METHOD)(void* parm);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void freeObject(void* parm);
	void addObject(Texture* tex);
	void setTexObject(int index);

	iRect touchRect(iPoint off = iPointZero, iSize size = iSizeZero);

	void startAnimation(ANIMATION_METHOD m = NULL, void* p = NULL);
	void stopAnimation();

	void paint(float dt, iPoint off, iPoint rate);
	void paint(float dt, float x, float y, float sx, float sy);

	iImage* copy();

public:
	iArray* arrayTex;
	Texture* tex;

	bool ani;
	bool round;
	uint8 repeatNum, repeatIdx;// 0-: inf
	uint8 frame, lastFrame;
	float _delta, delta;
	
	iPoint position;
	int anc;
	int8 reverse;
	float alpha;
	float degree;

	ANIMATION_METHOD method;
	void* parm;
};

