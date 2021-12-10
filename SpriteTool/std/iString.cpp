#include "iString.h"

#include "iStd.h"

#define _strLength 16

iString::iString()
{
	str = new char[_strLength];
	str[0] = 0;
	strLength = _strLength;
}

iString::iString(const char* szFormat, ...)
{
	str = new char[_strLength];
	str[0] = 0;
	strLength = _strLength;

	char szText[256];
	va_start_end(szText, szFormat);
	set(szText);
}

iString::iString(const iString& s)
{
	str = new char[_strLength];
	str[0] = 0;
	strLength = _strLength;

	set(s.str);
}

iString::~iString()
{
	delete str;
}

iString& iString::operator =(const char* str)
{
	set(str);
	return *this;
}

iString& iString::operator =(const iString& str)
{
	set(str.str);
	return *this;
}

static char* _str = NULL;
static int _strLen = -1;

const char* iString::operator +(const char* s)
{
	int l = strlen(str);
	int len = l + strlen(s);
	char* r;
	if (len > _strLen)
	{
		if (_str)
			delete _str;
		_str = new char[_strLen = len + 1];
	}
	r = _str;
	strcpy(r, str);
	strcpy(&r[l], s);
	r[len] = 0;
	return r;
}

const char* iString::operator +(const iString& s)
{
	int l = strlen(str);
	int len = l + strlen(s.str);
	char* r;
	if (len > _strLen)
	{
		if (_str)
			delete _str;
		_str = new char[_strLen = len + 1];
	}
	r = _str;
	strcpy(r, str);
	strcpy(&r[l], s.str);
	r[len] = 0;
	return r;
}

iString& iString::operator += (const char* s)
{
	set(s);
	return *this;
}

iString& iString::operator += (const iString& s)
{
	set(s.str);
	return *this;
}

void iString::set(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szText, szFormat);

	int len = strlen(szText);
	if (len > strLength)
	{
		delete str;
		str = new char[strLength = len + 1];
	}
	strcpy(str, szText);
}

void iString::add(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szText, szFormat);

	int l = strlen(str);
	int len = l + strlen(szText);
	if( len > strLength)
	{
		char* s = new char[strLength = len + 1];
		strcpy(s, str);
		delete str;
		str = s;
	}
	//strcat(str, szText);
	strcpy(&str[l], szText);
}

char** iString::getStringLine(const char* str, int& lineNum, int d)
{
	int i, l, off, len = strlen(str);

	lineNum = 1;
	for (i = 0; i < len; i++)
	{
		if (str[i] == d)
			lineNum++;
	}

	char** line = new char* [lineNum];
	for (i = 0, l = 0, off = 0; i < len; i++)
	{
		if (str[i] == d)
		{
			int n = i - off;
			char* s = new char[n + 1];
			memcpy(s, &str[off], n);
			s[n] = 0;
			line[l] = s;
			l++;
			off = i + 1;
		}
	}

	int n = i - off;
	char* s = new char[n + 1];
	memcpy(s, &str[off], n);
	s[n] = 0;
	line[l] = s;
	//l++;

	return line;
}

void iString::freeStringLine(char** line, int lineNum)
{
	for (int i = 0; i < lineNum; i++)
		delete line[i];
	delete line;
}

#if 1
void iString::MEMCPY(void* dst, const void* src, int size)
{
	char* d = (char*)dst;
	const char* s = (char*)src;
	for (int i = 0; i < size; i++)
		d[i] = s[i];
}

void iString::MEMSET(void* dst, unsigned char v, int size)
{
	char* d = (char*)dst;
	for (int i = 0; i < size; i++)
		d[i] = v;
}

void iString::STRCPY(char* dst, const char* src)
{
	for (int i = 0; ; i++)
	{
		dst[i] = src[i];
		if (src[i] == 0)
			return;
	}
}

void iString::STRCAT(char* dst, const char* src)
{
	char* d = &dst[strlen(dst)];
	for (int i = 0; ; i++)
	{
		d[i] = src[i];
		if (src[i] == 0)
			return;
	}
}
int iString::STRCMP(const char* s0, const char* s1)
{
	int s0len = strlen(s0);
	int s1len = strlen(s1);
	if (s0len < s1len)
		return -1;
	else if (s0len > s1len)
		return 1;

	int len = s0len;// s1len;
	for (int i = 0; i < len; i++)
	{
		if (s0[i] < s1[i])
			return -1;
		if (s0[i] > s1[i])
			return -1;
	}
	return 0;
}
#endif

bool iString::isUnicode(const char* str)
{
	return (str[0] < 0);

}
