#pragma once

#include "iPoint.h"
#include "iSize.h"

struct iRect
{
	iPoint origin;
	iSize size;

	bool operator ==(const iRect& r);
	bool operator !=(const iRect& r);
};

#define iRectZero iRectMake(0, 0, 0, 0)
#define iRectOne iRectMake(1, 1, 1, 1)
iRect iRectMake(float x, float y, float width, float height);

bool containPoint(iPoint point, iRect rect);
bool containRect(iRect rt0, iRect rt1);
