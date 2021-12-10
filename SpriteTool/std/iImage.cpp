#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	arrayTex = new iArray(freeObject);

	ani = false;
	round = false;
	repeatNum = 0;
	repeatIdx = 0;
	frame = 0;
	lastFrame = false;
	_delta = 0.08f;
	delta = 0.0f;

	position = iPointZero;
	anc = TOP | LEFT;
	reverse = REVERSE_NONE;
	alpha = 1.0f;
	degree = 0.0f;

	method = NULL;
	parm = NULL;
}

iImage::~iImage()
{
	delete arrayTex;
}

void iImage::freeObject(void* parm)
{
	freeImage((Texture*)parm);
}

void iImage::addObject(Texture* tex)
{
	arrayTex->addObject(tex);
	tex->retainCount++;
	if (this->tex == NULL)
		this->tex = tex;
}

void iImage::setTexObject(int index)
{
	tex = (Texture*)arrayTex->objectAtIndex(index);
	frame = index;
}

iRect iImage::touchRect(iPoint off, iSize size)
{
	iRect rt;
	rt.origin = position + off - iPointMake(size.width / 2, size.height / 2);
	switch (anc) {

	case LEFT | TOP:																		break;
	case LEFT | VCENTER:									rt.origin.y-=tex->height / 2;	break;
	case LEFT | BOTTOM:										rt.origin.y-=tex->height;		break;

	case HCENTER | TOP:		rt.origin.x-=tex->width / 2;									break;
	case HCENTER | VCENTER:	rt.origin.x-=tex->width / 2;	rt.origin.y-=tex->height / 2;	break;
	case HCENTER | BOTTOM:	rt.origin.x-=tex->width / 2;	rt.origin.y-=tex->height;		break;

	case RIGHT | TOP:		rt.origin.x-=tex->width;										break;
	case RIGHT | VCENTER:	rt.origin.x-=tex->width;		rt.origin.y-=tex->height / 2;	break;
	case RIGHT | BOTTOM:	rt.origin.x-=tex->width;		rt.origin.y-=tex->height;		break;
	}
	rt.size = iSizeMake(tex->width, tex->height) + size;

	return rt;
}


void iImage::startAnimation(ANIMATION_METHOD m, void* p)
{
	ani = true;
	//repeatNum;// 밖에서 설정
	repeatIdx = 0;
	frame = 0;
	//_delta;// 밖에서 설정
	delta = 0.0f;

	method = m;
	parm = p;
}

void iImage::stopAnimation()
{
	ani = false;
}

void iImage::paint(float dt, iPoint off, iPoint rate)
{
	paint(dt, off.x, off.y, rate.x, rate.y);
}

void iImage::paint(float dt, float x, float y, float rx, float ry)
{
	if (ani)
	{
		delta += dt;
		if (delta > _delta)
		{
			//delta -= _delta;
			delta = 0.0f;

			frame++;
			if (frame > arrayTex->count - 1)
			{
				frame = 0;

				if (repeatNum == 0)// inf
				{

				}
				else
				{
					repeatIdx++;
					if (repeatIdx == repeatNum)
					{
						if( method )
							method(parm);
						ani = false;

						if (lastFrame)
							frame = arrayTex->count - 1;
					}
				}
			}
		}
	}

	float r, g, b, a;
	getRGBA(r, g, b, a);
	setRGBA(r, g, b, alpha);

	x += position.x;
	y += position.y;
	int anc_ = anc;
	tex = (Texture*)arrayTex->objectAtIndex(frame);
	if (degree)
	{
		//if( anc & HCENTER)
		if (anc & LEFT)
			x += tex->width / 2;
		else if (anc & RIGHT)
			x -= tex->width / 2;

		//if( anc & VCENTER)
		if( anc & TOP )
			y += tex->height / 2;
		else if( anc & BOTTOM )
			y -= tex->height / 2;

		anc_ = VCENTER | HCENTER;
	}

	//drawImage(tex, x, y, anc_);
	drawImage(tex, x, y, anc_,
		0, 0, tex->width, tex->height,
		rx, ry, 2, degree, reverse);

	setRGBA(r, g, b, a);
}

iImage* iImage::copy()
{
	iImage* img = new iImage();
	iArray* a = img->arrayTex;

	memcpy(img, this, sizeof(iImage));
	img->arrayTex = a;
	for (int i = 0; i < arrayTex->count; i++)
		img->addObject((Texture*)arrayTex->objectAtIndex(i));

	return img;
}

