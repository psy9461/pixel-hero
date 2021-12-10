#pragma once

struct SortData
{
	int index;
	float y;
};

class iSort
{
public:
	iSort(int max = 100);
	virtual ~iSort();

	void init();

	void add(int index, float y);
	void add(float y);

	void update();

	int get(int index);
public:
	SortData** sd;
	SortData* _sd;
	int sdNum;
};

