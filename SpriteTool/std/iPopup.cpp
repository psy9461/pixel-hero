#include "iPopup.h"

#include "iStd.h"

Texture* texFboForiPopup;

iPopup::iPopup()
{
	arrayImg = new iArray(freeImg);

	bShow = false;
	style = iPopupNone;
	stat = iPopupClose;
	openPoint = iPointZero;
	closePoint = iPointZero;
	_aniDt = 0.2f;
	aniDt = 0.0f;

	methodOpen = NULL;
	methodClose = NULL;
	methodBefore = NULL;
	methodAfter = NULL;

	selected = -1;
}

iPopup::~iPopup()
{
	delete arrayImg;
}

void iPopup::freeImg(void* parm)
{
	delete (iImage*)parm;
}

void iPopup::addObject(iImage* img)
{
	arrayImg->addObject(img);
}

void iPopup::paint(float dt)
{
#if 0
	dt /= 10;
#endif
	if (bShow == false)
		return;

	float alpha = 1.0f;
	iPoint p = closePoint;
	float rx = 1.0f, ry = 1.0f;
	float degree = 0;

	if (style == iPopupNone)
	{
		if (stat == iPopupOpen)
		{
			stat = iPopupProc;
			if (methodOpen)
				methodOpen(this);
		}
		else if (stat == iPopupProc)
		{
		}
		else if (stat == iPopupClose)
		{
			bShow = false;
			if (methodClose)
				methodClose(this);
		}
	}
	else if (style == iPopupAlpha)
	{
		if (stat == iPopupOpen)
		{
			alpha = aniDt / _aniDt;

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				stat = iPopupProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (stat == iPopupProc)
		{
		}
		else if (stat == iPopupClose)
		{
			alpha = 1.0f - aniDt / _aniDt;

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
			}
		}
	}
	else if (style == iPopupMove)
	{
		if (stat == iPopupOpen)
		{
			//p = linear(aniDt / _aniDt, openPoint, closePoint);
			p = easeIn(aniDt / _aniDt, openPoint, closePoint);
			//p = easeOut(aniDt / _aniDt, openPoint, closePoint);

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				stat = iPopupProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (stat == iPopupProc)
		{
		}
		else if (stat == iPopupClose)
		{
			//p = linear(aniDt / _aniDt, closePoint, openPoint);
			p = easeIn(aniDt / _aniDt, closePoint, openPoint);
			//p = easeOut(aniDt / _aniDt, closePoint, openPoint);

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
			}
		}
	}
	else if (style == iPopupZoom)
	{
		if (stat == iPopupOpen)
		{
			float r = linear(aniDt / _aniDt, 0, 1);
			//float r = easeIn(aniDt / _aniDt, 0, 1);
			//float r = easeOut(aniDt / _aniDt, 0, 1);
			rx = r;
			ry = r;
			p = linear(r, openPoint, closePoint);

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				stat = iPopupProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (stat == iPopupProc)
		{
		}
		else if (stat == iPopupClose)
		{
			float r = linear(aniDt / _aniDt, 0, 1);
			//float r = easeIn(aniDt / _aniDt, 0, 1);
			//float r = easeOut(aniDt / _aniDt, 0, 1);
			rx =
			ry = 1.0f - r;
			p = linear(r, closePoint, openPoint);

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
			}
		}
	}
	//else if (style == iPopupTong)
	//{
	//
	//}
	else if (style == iPopupRotate)
	{
		if (stat == iPopupOpen)
		{
			float r = linear(aniDt / _aniDt, 0, 1);
			rx = r;
			ry = r;
			p = linear(r, openPoint, closePoint);
			degree = r * 360;

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				stat = iPopupProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (stat == iPopupProc)
		{
		}
		else if (stat == iPopupClose)
		{
			float r = linear(aniDt / _aniDt, 0, 1);
			rx =
			ry = 1.0f - r;
			p = linear(r, closePoint, openPoint);
			degree = r * 360;

			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
			}
		}
	}

	if (methodBefore)
		methodBefore(this, dt, aniDt / _aniDt);
#if 0
	setRGBA(1, 1, 1, alpha);
	for (int i = 0; i < arrayImg->count; i++)
	{
		iImage* img = (iImage*)arrayImg->objectAtIndex(i);
		//img->paint(dt, p.x, p.y, rx, ry);
		Texture* tex = (Texture*)img->arrayTex->objectAtIndex(img->frame);
		drawImage(tex,	p.x + img->position.x * rx,
						p.y + img->position.y * ry, TOP | LEFT,
			0, 0, tex->width, tex->height,
			rx, ry, 2, 0);
	}
	setRGBA(1, 1, 1, 1);
#else
	iPoint off = iPointZero;
	for (int i = 0; i < arrayImg->count; i++)
	{
		iImage* img = (iImage*)arrayImg->objectAtIndex(i);
		//img->paint(dt, 0, 0, 1.0f, 1.0f);
		if (off.x > img->position.x)
			off.x = img->position.x;
		if (off.y > img->position.y)
			off.y = img->position.y;
	}

	fbo->bind(texFboForiPopup);
	fbo->clear(0, 0, 0, 0);
	for (int i = 0; i < arrayImg->count; i++)
	{
		iImage* img = (iImage*)arrayImg->objectAtIndex(i);
		img->paint(dt, -off.x, -off.y, 1.0f, 1.0f);
	}
	fbo->unbind();
	p += off;
	Texture* t = texFboForiPopup;
	setGLBlend(GLBlendMultiplied);
	setRGBA(1, 1, 1, alpha);
	if (degree == 0.0f)
	{
		drawImage(t, p.x, p.y, TOP | LEFT,
			0, 0, t->width, t->height, rx, ry, 2, 0, REVERSE_HEIGHT);
	}
	else
	{
		p.x += (t->width / 2 * rx);
		p.y += (t->height / 2 * ry);
		drawImage(t, p.x, p.y, VCENTER | HCENTER,
			0, 0, t->width, t->height, rx, ry, 2, degree, REVERSE_HEIGHT);
	}
	setGLBlend(GLBlendAlpha);
	setRGBA(1, 1, 1, 1);
#endif
	if (methodAfter)
		methodAfter(this, dt, aniDt / _aniDt);
}

void iPopup::show(bool show)
{
	if (show)
	{
		if (stat != iPopupClose)
			return;

		bShow = true;
		stat = iPopupOpen;
		aniDt = 0.0f;
	}
	else// if (show == false)
	{
		if (stat != iPopupProc)
			return;

		stat = iPopupClose;
		aniDt = 0.0f;
	}
}
