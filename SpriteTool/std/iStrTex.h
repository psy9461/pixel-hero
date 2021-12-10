#pragma once

#include "iType.h"

typedef Texture* (*MethodStrTex)(const char* str);

class iStrTex
{
public:
	iStrTex(MethodStrTex method);
	virtual ~iStrTex();

	static char** getStringLineWidth(const char* str, int& lineNum, float width, int d = '\n');

	void setString(const char* szFormat, ...);

	void paint(float x, float y, int anc, const char* szFormat, ...);
	void paint(iPoint p, int anc, const char* szFormat, ...);

	void paint(float x, float y, int anc);
	void paint(iPoint p, int anc);

public:
	char* str;

	MethodStrTex method;
	Texture* tex;

	float widthMax;
};

