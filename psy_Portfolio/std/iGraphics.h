#pragma once

#if 1// AppForCtrl.cpp
#include <sys/types.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <windows.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
//#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#include "iType.h"

#include "iSize.h"
#include "iRect.h"

#define setStringYellow setStringRGBA(1, 0.823, 0.407, 1)
#define setStringBorderBrown setStringBorderRGBA(0.55, 0.27, 0.07, 1)
#define setStringNameKor setStringName("assets/UI/font/neodgm_pro.ttf")
#define setStringNameEng setStringName("assets/CraftPixNet Survival Kit.otf")

struct iRGBA {
	float r;
	float g;
	float b;
	float a;
};

extern iRGBA yellowMain;
extern iRGBA brownBorder;

typedef Image igImage;

class iGraphics
{
private:
	iGraphics();
public:
	static iGraphics* share();
public:
	virtual ~iGraphics();

	// gdiplus
	void start(HDC hdc);
	void end();

	void init(iSize size);
	void init(int width, int height);

	void clear(float r, float g, float b, float a);
	void clear();

	Texture* getTexture();

	void drawLine(iPoint sp, iPoint ep);
	void drawLine(float x0, float y0, float x1, float y1);

	void drawRect(iRect rt, float radius = 0.0f);
	void drawRect(float x, float y, float width, float height, float radius = 0.0f);
	
	void fillRect(iRect rt, float radius = 0.0f);
	void fillRect(float x, float y, float width, float height, float radius = 0.0f);

	igImage* createIgImage(const char* szFormat, ...);
	void freeIgImage(igImage* img);
	static int igImageWidth(igImage* img);
	static int igImageHeight(igImage* img);
	void drawIgImage(igImage* tex, float x, float y, int anc,
		float ix, float iy, float iw, float ih, float rx, float ry,
		int xyz, float degree, int reverse = REVERSE_NONE);
	void drawIgImage(igImage* img, float x, float y, int anc);

	static iRect rectOfString(const char* szFormat, ...);
	void drawString(float x, float y, int anc, const char* szFormat, ...);

public:
	Bitmap* bmp;
	Graphics* g;
};

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* str);


