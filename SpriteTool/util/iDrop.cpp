#include "iDrop.h"

#include "iStd.h"

Drop::Drop()
{

}

Drop::~Drop()
{

}

DropItem::DropItem() : Drop()
{

}

DropItem::~DropItem()
{

}

bool DropItem::paint(float dt, iPoint off)
{
	iPoint p = position;
	float a = 1.0f, s = 1.0f;
	int anc = BOTTOM | HCENTER;

	if (state == DropItemStateAppear)
	{
		p.y -= 50 * _sin(180 * delta / 0.5f);

		a = delta / 0.25f;
		if (a > 1.0f)
			a = 1.0f;

		s = a;

		delta += dt;
		if (delta >= 0.5f)
		{
			state = DropItemStateWait;
			delta = 0.0f;
		}
	}
	else if (state == DropItemStateWait)
	{
		p.y;
		a = 1.0f;
		s = 1.0f;
	}
	else if (state == DropItemStateMove)
	{
		p = linear(delta / 0.5f, position, tPosition);
		p.y += 100 * _sin(180 * delta / 0.5f);

		delta += dt;
		if (delta >= 0.5f)
		{
			state = DropItemStateDisappear;
			delta = 0.0f;
			if (methodArrive)
				methodArrive(this);
		}
	}
	else// if (state == DropItemStateDisappear)
	{
		// a 1 -> 0
		p = tPosition;
		a = 1.0f - delta / 0.5f;
		s = 1.0 + (5.0f * delta / 0.5f);
		anc = VCENTER | HCENTER;
		if (delta >= 0.5f)
		{
			state = DropItemStateDisable;
			return true;
		}
	}
	setRGBA(1, 1, 1, a);
	static Texture* tex = createImage("assets/potion.png");
	drawImage(tex, p.x, p.y, anc,
		0, 0, tex->width, tex->height, s, s, 2, 0, REVERSE_NONE);

	return false;
}

void DropItem::enable(int index, int num, iPoint position)
{
	this->index = index;
	this->position = position;
	this->num = num;
	state = DropItemStateAppear;
	delta = 0.0f;
}

void DropItem::move(iPoint position)
{
	if (state == DropItemStateMove)
		return;

	tPosition = position + iPointMake(	-10 + random() % 21,
										-10 + random() % 21);
	state = DropItemStateMove;
	delta = 0.0f;
}

bool DropItem::contain(iPoint p)
{
	if (state >= DropItemStateMove)
		return false;

	iRect rt = iRectMake(position.x - 32, position.y - 64, 64, 64);

	return containPoint(p, rt);
}

DropItem* _dropItem;
DropItem** dropItem;
int dropItemNum;
#define _dropItemNum 100

void loadDropItem(MethodDropItem methodArrive)
{
	_dropItem = new DropItem[_dropItemNum];
	for (int i = 0; i < _dropItemNum; i++)
	{
		DropItem* di = &_dropItem[i];
		di->state = DropItemStateDisable;
		di->methodArrive = methodArrive;
	}
	dropItem = new DropItem * [_dropItemNum];
	dropItemNum = 0;
}

void freeDropItem()
{
	delete _dropItem;
	delete dropItem;
}

void drawDropItem(float dt, iPoint off)
{
	// 오브젝트 정렬
	for (int i = 0; i < dropItemNum; i++)
	{
		if(dropItem[i]->state < DropItemStateMove)
		{
			if (dropItem[i]->paint(dt, off))
			{
				dropItemNum--;
				dropItem[i] = dropItem[dropItemNum];
			}
		}
	}

	//마지막 레이어
	for (int i = 0; i < dropItemNum; i++)
	{
		if (dropItem[i]->state < DropItemStateMove)
			continue;
		
		if (dropItem[i]->paint(dt, off))
		{
			dropItemNum--;
			dropItem[i] = dropItem[dropItemNum];
		}
		
	}
}

void addDropItem(int index, int num, iPoint position)
{
	for (int i = 0; i < _dropItemNum; i++)
	{
		DropItem* di = &_dropItem[i];
		if (di->state == DropItemStateDisable)
		{
			di->enable(index, num, position);
			dropItem[dropItemNum] = di;
			dropItemNum++;
			return;
		}
	}
	printf("Not Enough DropItem\n");
}

bool containDropItem(iPoint position)
{
	for (int i = 0; i < dropItemNum; i++)
	{
		DropItem* di = dropItem[i];
		if (di->contain(position))
		{
			di->move(iPointMake(50, 50));
			return true;
		}
	}
	return false;
}


