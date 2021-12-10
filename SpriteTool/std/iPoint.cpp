#include "iPoint.h"

#include "iStd.h"

bool iPoint::operator ==(const iPoint& p)
{
	return x == p.x && y == p.y;
}
bool iPoint::operator !=(const iPoint& p)
{
	return x != p.x || y != p.y;
}

iPoint iPoint::operator+(const iPoint& p)
{
	iPoint r;
	r.x = x + p.x;
	r.y = y + p.y;
	return r;
}

iPoint iPoint::operator-(const iPoint& p)
{
	iPoint r;
	r.x = x - p.x;
	r.y = y - p.y;
	return r;
}

iPoint iPoint::operator*(const iPoint& p)
{
	iPoint r;
	r.x = x * p.x;
	r.y = y * p.y;
	return r;
}

iPoint iPoint::operator/(const iPoint& p)
{
	iPoint r;
	r.x = x / p.x;
	r.y = y / p.y;
	return r;
}

iPoint& iPoint::operator+=(const iPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

iPoint& iPoint::operator-=(const iPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

iPoint& iPoint::operator*=(const iPoint& p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}

iPoint& iPoint::operator/=(const iPoint& p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}

iPoint iPoint::operator+(const float s)
{
	iPoint r;
	r.x = x + s;
	r.y = y + s;
	return r;
}

iPoint iPoint::operator-(const float s)
{
	iPoint r;
	r.x = x - s;
	r.y = y - s;
	return r;
}

iPoint iPoint::operator*(const float s)
{
	iPoint r;
	r.x = x * s;
	r.y = y * s;
	return r;
}

iPoint iPoint::operator/(const float s)
{
	iPoint r;
	r.x = x / s;
	r.y = y / s;
	return r;
}

iPoint& iPoint::operator+=(const float s)
{
	x += s;
	y += s;
	return *this;
}

iPoint& iPoint::operator-=(const float s)
{
	x -= s;
	y -= s;
	return *this;
}

iPoint& iPoint::operator*=(const float s)
{
	x *= s;
	y *= s;
	return *this;
}

iPoint& iPoint::operator/=(const float s)
{
	x /= s;
	y /= s;
	return *this;
}

iPoint iPointMake(float x, float y)
{
	iPoint p;
	p.x = x;
	p.y = y;
	return p;
}

float iPointAngle(const iPoint& v1, const iPoint& v2, const iPoint& v3)
{
#if 1// 제 2 코사인법칙(a2 = b2 + c2 - 2bc cosA)
	// v13^2 = v12^2 + v23^2 - 2 * v12 * v23 * cosA
	// A = acos( (b^2 + c^2 - a^2) / (2ab) );
	float a2 = (v1.x - v3.x) * (v1.x - v3.x) + (v1.y - v3.y) * (v1.y - v3.y);
	float b2 = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
	float c2 = (v2.x - v3.x) * (v2.x - v3.x) + (v2.y - v3.y) * (v2.y - v3.y);
	float degree = acos((b2 + c2 - a2) / sqrtf(4 * b2 * c2)) * 180 / M_PI;
	if (v3.y > v1.y)
		degree = 360 - degree;
	return degree;
#else// 내적(aXb = |a||b|cosA)
	iPoint a = iPointMake(v1.x - v2.x, v1.y - v2.y);
	iPoint b = iPointMake(v3.x - v2.x, v3.y - v2.y);
	float degree = acos((a.x * b.x + a.y * b.y) / sqrtf((a.x * a.x + a.y * a.y) * (b.x * b.x + b.y * b.y)))
		* 180 / M_PI;
	if (v3.y > v1.y)
		degree = 360 - degree;
	return degree;
#endif
}

iPoint iPointRotate(iPoint p, iPoint t, float degree)
{
	while (degree < 0)		degree += 360;
	while (degree > 360)	degree -= 360;
	float sinr = _sin(degree);
	float cosr = _cos(degree);
#if 0// bottom|left - math
	iPoint r;
	r.x = t.x + (p.x - t.x) * cosr - (p.y - t.y) * sir;
	r.y = t.y + (p.y - t.y) * sinr + (p.y - t.y) * cosr;
#else// top|left
	iPoint r = iPointMake(p.x - t.x, t.y - p.y);
	iPoint _p;
	_p.x = r.x * cosr - r.y * sinr;
	_p.y = r.x * sinr + r.y * cosr;

	_p.x = t.x + _p.x;
	_p.y = t.y - _p.y;
	return _p;
#endif
}

float iPointLength(iPoint p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}

iPoint iPointVector(iPoint p)
{
	float len = sqrt(p.x * p.x + p.y * p.y);
	if (len == 0.0f)
		return iPointZero;
	return p / len;
}

float iPointDot(iPoint p0, iPoint p1)
{
	return p0.x * p1.x + p0.y * p1.y;
}

float iPointDistance(iPoint p0, iPoint p1)
{
	iPoint p;
	p.x = p0.x - p1.x;
	p.y = p0.y - p1.y;
	return sqrt(p.x * p.x + p.y * p.y);
}
float iPointDistance2(iPoint p0, iPoint p1)
{
	iPoint p;
	p.x = p0.x - p1.x;
	p.y = p0.y - p1.y;
	return p.x * p.x + p.y * p.y;
}


float distanceFromLine(iPoint p, iPoint sp, iPoint ep)
{
	iPoint a = p - sp;
	iPoint m = a / iPointLength(a);

	iPoint t = ep - sp;
	float len = iPointLength(t);
	t /= len;

	float projLen;// |a|cosS
	// a.b = |a||b|cosS
	// |a|cosS = a.b / |b|
	projLen = iPointDot(sp, ep) / len;
	if (projLen < 0)
		projLen = 0;
	else if (projLen > len)
		projLen = len;

	iPoint proj = t * projLen;// 단위벡터(ep-sp) * projLen

	//iPoint n = ep - sp;
	//float len = iPointLength(n);
	//n /= len;
	return iPointLength(a - proj);
}


float linear(float rate, float start, float end)
{
	return start + (end - start) * rate;
}

iPoint linear(float rate, iPoint start, iPoint end)
{
	return start + (end - start) * rate;
}

float easeIn(float rate, float start, float end)
{
	//return start + (end - start) * _sin( 90 * _sin(90 * rate) );
	return start + (end - start) * _sin(90 * rate);
}
iPoint easeIn(float rate, iPoint start, iPoint end)
{
	return start + (end - start) * _sin(90 * rate);
}

float easeOut(float rate, float start, float end)
{
	//return start + (end - start) * rate * rate * rate;
	return start + (end - start) * rate * rate;
}
iPoint easeOut(float rate, iPoint start, iPoint end)
{
	//return start + (end - start) * (rate * rate * rate);
	return start + (end - start) * (rate * rate);
}
