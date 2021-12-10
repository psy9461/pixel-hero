#pragma once

#include "iType.h"

#define iNumber_delta 0.2f

class iNumber
{
public:
	iNumber();
	virtual ~iNumber();

	int get(float dt);
	void set(int num);
	void add(int num);

	const char* getDollar(float dt);
	const char* getWon(float dt);

public:
	int num, tNum;
	float delta, _delta;

	char* str;
};

