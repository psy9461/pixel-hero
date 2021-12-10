#include "iRect.h"

#include "iStd.h"

iRect iRectMake(float x, float y, float width, float height)
{
	iRect r;
	r.origin.x = x;
	r.origin.y = y;
	r.size.width = width;
	r.size.height = height;
	return r;
}

bool iRect::operator==(const iRect& r)
{
	return	(origin.x == r.origin.x && origin.y == r.origin.y &&
			 size.width == r.size.width && size.height == r.size.height);
}

bool iRect::operator!=(const iRect& r)
{
	return	(origin.x != r.origin.x || origin.y != r.origin.y ||
			 size.width != r.size.width || size.height != r.size.height);
}


bool containPoint(iPoint point, iRect rect)
{
#if 0
	return (
		point.x < rect.origin.x&&
		point.x < rect.origin.x + rect.size.width &&
		point.y > rect.origin.y&&
		point.y < rect.origin.y + rect.size.height);
#else
	return !(
		point.x < rect.origin.x ||
		point.x > rect.origin.x + rect.size.width ||
		point.y < rect.origin.y ||
		point.y > rect.origin.y + rect.size.height);
#endif
}

bool containRect(iRect rt0, iRect rt1)
{
	return !(
		rt0.origin.x + rt0.size.width < rt1.origin.x ||
		rt0.origin.x > rt1.origin.x + rt1.size.width ||
		rt0.origin.y + rt0.size.height < rt1.origin.y ||
		rt0.origin.y > rt1.origin.y + rt1.size.height
		);
}
