#include "iShortestPath.h"

#include "iStd.h"

iShortestPath::iShortestPath()
{
	tileX = 1;
	tileY = 1;
	tileXY = tileX * tileY;
	tile = new SPTile[tileXY];
	tile[0].path = new int[tileXY];
}

iShortestPath::~iShortestPath()
{
	for (int i = 0; i < tileXY; i++)
		delete tile[i].path;
	delete tile;
}

void iShortestPath::init(int tileX, int tileY)
{
	int i;
	for (i = 0; i < tileXY; i++)
		delete tile[i].path;
	delete tile;

	this->tileX = tileX;
	this->tileY = tileY;
	tileXY = tileX * tileY;
	tile = new SPTile[tileXY];
	for (i = 0; i < tileXY; i++)
		tile[i].path = new int[tileXY];
}

void iShortestPath::run(int* mapTile, int start, int end, int* path, int& num)
{
	SPTile* t;
	int i, j, k;

	for (i = 0; i < tileXY; i++)
	{
		t = &tile[i];
		t->value = SP_INF;
		t->visit = false;
		//t->path[0];
		t->num = 0;
	}

	t = &tile[start];
	t->value = 0;
	t->visit = false;// !!!
	t->path[0] = start;
	t->num = 1;

	for(;;)
	{
		// 방문하지 않은 곳 중에 값이 최소값
		int curr = -1;
		int min = SP_INF;
		for (i = 0; i < tileXY; i++)
		{
			t = &tile[i];
			if (t->value < min &&
				t->visit == false)
			{
				min = t->value;
				curr = i;
			}
		}

		if (curr == -1)
			break;
		SPTile* p0 = &tile[curr];
		p0->visit = true;

		for (i = 0; i < 4; i++)
		{
			if (i == 0)
			{	// left
				if (curr % tileX == 0) continue;
				j = curr - 1;
			}
			else if (i == 1)
			{	// right
				if (curr % tileX == tileX - 1) continue;
				j = curr + 1;
			}
			else if (i == 2)
			{	// up
				if (curr / tileX == 0) continue;
				j = curr - tileX;
			}
			else// if( i==3 )
			{	// down
				if (curr / tileX == tileY - 1) continue;
				j = curr + tileX;
			}

			SPTile* p1 = &tile[j];
			if (p1->value > p0->value + mapTile[j])
			{
				p1->value = p0->value + mapTile[j];

				memcpy(p1->path, p0->path, sizeof(int) * p0->num);
				p1->num = p0->num;

				p1->path[p0->num] = j;
				p1->num++;
			}
		}
	}

	t = &tile[end];
	memcpy(path, t->path, sizeof(int) * t->num);
	num = t->num;
}


