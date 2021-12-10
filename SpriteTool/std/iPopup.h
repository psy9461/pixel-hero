#pragma once

#include "iArray.h"
#include "iImage.h"

extern Texture* texFboForiPopup;

enum iPopupStat
{
	iPopupOpen = 0,
	iPopupProc,
	iPopupClose
};

enum iPopuStyle
{
	iPopupNone = 0,
	iPopupAlpha,
	iPopupMove,
	iPopupZoom,
	iPopupTong,
	iPopupRotate,
};

class iPopup;
typedef void (*Pop_OpenClose_Method)(iPopup* pop);
typedef void (*Pop_Draw_Method)(iPopup* pop, float dt, float rate);

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	static void freeImg(void* parm);
	void addObject(iImage* img);

	void paint(float dt);

	void show(bool show);


public:
	iArray* arrayImg;

	bool bShow;
	iPopuStyle style;
	iPopupStat stat;
	iPoint openPoint, closePoint;
	float aniDt, _aniDt;

	Pop_OpenClose_Method methodOpen;
	Pop_OpenClose_Method methodClose;
	Pop_Draw_Method methodBefore;
	Pop_Draw_Method methodAfter;

	int8 selected;
};



