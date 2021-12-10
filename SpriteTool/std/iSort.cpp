#include "iSort.h"

iSort::iSort(int max)
{
	_sd = new SortData[max];
	sd = new SortData*[max];
	for (int i = 0; i < max; i++)
		sd[i] = &_sd[i];
	sdNum = 0;
}

iSort::~iSort()
{
	delete _sd;
	delete sd;
}

void iSort::init()
{
	sdNum = 0;
}

void iSort::add(int index, float y)
{
	SortData* d = sd[sdNum];
	d->index = index;
	d->y = y;
	sdNum++;
}

void iSort::add(float y)
{
	SortData* d = sd[sdNum];
	d->index = sdNum;
	d->y = y;
	sdNum++;
}

void iSort::update()
{
#if 1
	int i, j, k = sdNum - 1;
	for (i = 0; i < k; i++)
	{
		for (j = i; j < sdNum; j++)
		{
			if (sd[i]->y > sd[j]->y)
			{
				SortData* tmp = sd[i];
				sd[i] = sd[j];
				sd[j] = tmp;
			}
		}
	}
#else
#endif
}

int iSort::get(int index)
{
	return sd[index]->index;
}
