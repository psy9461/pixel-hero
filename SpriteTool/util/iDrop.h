#pragma once

#include "iType.h"
#include "iPoint.h"

struct Drop
{
	Drop();
	virtual ~Drop();

	virtual bool paint(float dt, iPoint off) = 0;
};

struct DropItem;
typedef void (*MethodDropItem)(DropItem* di);

enum DropItemState
{
	DropItemStateAppear = 0,
	DropItemStateWait,
	DropItemStateMove,
	DropItemStateDisappear,

	DropItemStateDisable
};

struct DropItem : Drop
{
	DropItem();
	virtual ~DropItem();

	int index, num;
	iPoint position, tPosition;
	DropItemState state;

	float delta;

	MethodDropItem methodArrive;

	virtual bool paint(float dt, iPoint off);

	void enable(int index, int num, iPoint position);
	void move(iPoint position);

	bool contain(iPoint p);
};


void loadDropItem(MethodDropItem methodArrive);
void freeDropItem();
void drawDropItem(float dt, iPoint off);
void addDropItem(int index, int num, iPoint position);
bool containDropItem(iPoint position);
