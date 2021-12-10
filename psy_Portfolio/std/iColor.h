#pragma once

#include "iPoint.h"
#include "iSize.h"

#include "iType.h"

struct iColor4b;

struct iColor4f
{
	float r, g, b, a;

	//iColor4f operator =(const iColor4f& c);
	iColor4f operator =(const iColor4b& c);
};

struct iColor4b
{
	uint8 r, g, b, a;

	iColor4b operator =(const iColor4f& c);
	//iColor4b operator =(const iColor4b& c);
};

iColor4f iColor4fMake(float r, float g, float b, float a);
iColor4b iColor4bMake(uint8 r, uint8 g, uint8 b, uint8 a);
