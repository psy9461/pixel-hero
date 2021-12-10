#include "iNumber.h"

#include "iStd.h"

iNumber::iNumber()
{
	num = 0;
	tNum = 0;
	delta = 0.0f;
	_delta = iNumber_delta;

	str = new char[20];
}

iNumber::~iNumber()
{
	delete str;
}

int iNumber::get(float dt)
{
	int n = num;

	if (num != tNum)
	{
		n = linear(delta / _delta, num, tNum);

		delta += dt;
		if (delta > _delta)
		{
			delta = _delta;
			num = tNum;
		}
	}

	return n;
}

const char* iNumber::getDollar(float dt)
{
	int n = get(dt);
	//sprintf(str, "$ %d", n);

	// str 1000
	sprintf(str, "%d", n);

	// s 0001 => 000,1 $
	char s[30];
	int i, j, len = strlen(str);
	for (i = 0; i < len; i++)
		s[len - 1 - i] = str[i];
	s[len] = 0;

	// i s[i]
	// j str[j]
	for (i = 0, j = 0; i < len; i++, j++)
	{
		if (i && i % 3 == 0)
		{
			str[j] = ',';
			j++;
		}
		str[j] = s[i];
	}
	str[j] = ' ';
	str[j + 1] = '$';
	str[j + 2] = 0;

	// "000,1 $" => "$ 1,000"
	n = strlen(str);
	len = n / 2;
	for (i = 0; i < len; i++)
	{
		char t = str[i];
		str[i] = str[n - 1 - i];
		str[n - 1 - i] = t;
	}

	return str;
}

const char* iNumber::getWon(float dt)
{
	int n = get(dt);
	sprintf(str, "\\ %d", n);

	return str;
}

void iNumber::set(int n)
{
	num = get(0.0f);
	tNum = n;
	delta = 0.0f;
}

void iNumber::add(int n)
{
	num = get(0.0f);
	tNum += n;
	delta = 0.0f;
}

