#pragma once

struct iPoint
{
	float x, y;

	bool operator ==(const iPoint& p);
	bool operator !=(const iPoint& p);

	iPoint operator +(const iPoint& p);
	iPoint operator -(const iPoint& p);
	iPoint operator *(const iPoint& p);
	iPoint operator /(const iPoint& p);

	iPoint& operator +=(const iPoint& p);
	iPoint& operator -=(const iPoint& p);
	iPoint& operator *=(const iPoint& p);
	iPoint& operator /=(const iPoint& p);

	iPoint operator +(const float s);
	iPoint operator -(const float s);
	iPoint operator *(const float s);
	iPoint operator /(const float s);

	iPoint& operator +=(const float s);
	iPoint& operator -=(const float s);
	iPoint& operator *=(const float s);
	iPoint& operator /=(const float s);
};


#define iPointZero iPointMake(0, 0)
#define iPointOne iPointMake(1, 1)
iPoint iPointMake(float x, float y);

float iPointAngle(const iPoint& v1, const iPoint& v2, const iPoint& v3);
iPoint iPointRotate(iPoint p, iPoint t, float degree);

float iPointLength(iPoint p);
iPoint iPointVector(iPoint p);
float iPointDot(iPoint p0, iPoint p1);
float iPointDistance(iPoint p0, iPoint p1);
float iPointDistance2(iPoint p0, iPoint p1);

float distanceFromLine(iPoint p, iPoint sp, iPoint ep);

float linear(float rate, float start, float end);
iPoint linear(float rate, iPoint start, iPoint end);

float easeIn(float rate, float start, float end);
iPoint easeIn(float rate, iPoint start, iPoint end);

float easeOut(float rate, float start, float end);
iPoint easeOut(float rate, iPoint start, iPoint end);
