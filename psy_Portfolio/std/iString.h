#pragma once

class iString
{
public:
	iString();
	iString(const char* szFormat, ...);
	iString(const iString& s);
	virtual ~iString();

	iString& operator =(const char* str);
	iString& operator =(const iString& str);

	const char* operator +(const char* s);
	const char* operator +(const iString& s);

	iString& operator += (const char* s);
	iString& operator += (const iString& s);

	void set(const char* szFormat, ...);
	void add(const char* szFormat, ...);
	static char** getStringLine(const char* str, int& lineNum, int d='\n');
	static void freeStringLine(char** line, int lineNum);
#if 1
	static void MEMCPY(void* dst, const void* src, int size);
	static void MEMSET(void* dst, unsigned char v, int size);

	static void STRCPY(char* dst, const char* src);
	static void STRCAT(char* dst, const char* src);
	static int STRCMP(const char* s0, const char* s1);
#endif

	static bool isUnicode(const char* str);

public:
	char* str;
	int strLength;
};

/*
	iString a = "Hello ";
	iString b = "World";
	iString c = a + b;

	iString s = "hello\nwolrd\n";
	char** line = {
		"hello\n",
		"wolrd\n"
	};
*/

