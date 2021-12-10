#pragma once

#include "iType.h"

#include "iOpenGL.h"

class iFBO
{
public:
	iFBO(int width, int height);
	virtual ~iFBO();

	static void clear(float r, float g, float b, float a);
	void bind();
	void bind(Texture* tex);
	void unbind();

	void read(int x, int y, int width, int height, uint32* pixels);

public:
	GLuint fbo;
	GLuint depthBuffer;

	Texture* tex;// Gluint colorBuffer

	Texture** texStack;
	int stackNum;
	iSize size;
};

extern iFBO* fbo;
