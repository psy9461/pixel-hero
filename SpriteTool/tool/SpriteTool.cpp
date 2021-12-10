#include "SpriteTool.h"

#include "iStd.h"
#include "Game.h"


Sprite* sprite = NULL;
Frame* dummyFrame;

HWND hWndOpenGL;
HWND hBtnImgLoad;
HWND hCheckBoxReverse;
HWND hEbScale;
HWND hBtnPlay;
HWND hCbRepeat;

HWND* hBtnToolMgt;
HWND* hLbSpriteData;
HWND** hBtnUpDown;
HWND* hBtnStateMgt;
HWND* hBtnDataMgt;
HWND hStTexPath;
HWND hEbStateName;

HWND* hEbFramePosDelta;
HWND** hEbFrameRt;
void updateEditBox(HWND hwnd);
bool ignoreUpdateEditBox;

iRect rtOpenGL;
iRect rtDefCol;
bool setup = false;

void loadSpriteTool()
{
	ignoreUpdateEditBox = false;

	sprite = new Sprite();
	dummyFrame = genDummyFrame();
	rtOpenGL = iRectMake(400, 45, 500, 375);
	
	int i, j;

	hWndOpenGL = createWndOpenGL(rtOpenGL.origin.x, rtOpenGL.origin.y, rtOpenGL.size.width, rtOpenGL.size.height, loadGame);

	hBtnImgLoad = createWndButton("LOAD IMG", 680, 430, 80, 30);
	hCheckBoxReverse = createWndCheckBox("Reverse", 570, 430, 80, 30);
	hEbScale = createWndEditBox("1", 470, 430, 80, 30, WndEditFloat);
	createWndStatic("Scale", 400, 430, 70, 30, NULL);
	hBtnPlay = createWndButton("Play", 800, 430, 80, 30);
	hCbRepeat = createWndCheckBox("Repeat", 900, 430, 80, 30);
	
	hBtnToolMgt = new HWND[2];
	for (i = 0; i < 2; i++)
		hBtnToolMgt[i] = createWndButton(strToolMgtBtn[i], 50 + i * 70, 7, 60, 30);

	hLbSpriteData = new HWND[2];
	for (i = 0; i < 2; i++)
	{
		hLbSpriteData[i] = createWndListBox(strLB[i], 1, 50 + i * 130, 45, 120, 620);
		setWndListBox(hLbSpriteData[i], 0);
		
		if (i == 0)
		{
			for (j = 0; j < 6; j++)
			{
				iArray* arrayFrame = new iArray(Sprite::cbFrame);
				sprite->arrayBe->addObject(arrayFrame);

				addWndListBox(hLbSpriteData[0], j, strStateName[j]);
			}
		}
	}

	hBtnUpDown = new HWND * [2];
	for (i = 0; i < 2; i++)
	{
		hBtnUpDown[i] = new HWND[2];
		for (j = 0; j < 2; j++)
		{
			hBtnUpDown[i][j] = createWndButton(strUpDown[j], 10 + 300 * i, 310 + j * 40, 30, 30);
		}
	}

	hEbStateName = createWndEditBox("", 50, 670, 116, 30, WndEditAll);

	hBtnStateMgt = new HWND[3];
	for (i = 0; i < 3; i++)
		hBtnStateMgt[i] = createWndButton(strStateBtn[i], 45 + i * 60, 710, 50, 30);

	hBtnDataMgt = new HWND[2];
	for (i = 0; i < 2; i++)
		hBtnDataMgt[i] = createWndButton(strDataMgtBtn[i], 350 + i * 70, 500, 60, 30);


	hStTexPath = createWndStatic("Path", 590, 500, 400, 30, NULL);

	hEbFramePosDelta = new HWND[3];
	
	for (i = 0; i < 3; i++)
		hEbFramePosDelta[i] = createWndEditBox("0", 350 + i * 70, 600, 60, 30, WndEditFloat, updateEditBox);

	for (i = 0; i < 3; i++)
		createWndStatic(strPosDelta[i], 350 + i * 70, 550, 60, 30, NULL);

	hEbFrameRt = new HWND*[2];
	for (i = 0; i < 2; i++)
	{
		hEbFrameRt[i] = new HWND[4];
		for (j = 0; j < 4; j++)
			hEbFrameRt[i][j] = createWndEditBox("0", 650 + j * 70, 600 + i * 100, 60, 30, WndEditFloat, updateEditBox);
	}

	for (i = 0; i < 2; i++)
	{
		createWndStatic(strRt[i][0], 590, 600 + i * 100, 50, 30, NULL);
		for (j = 0; j < 4; j++)
			createWndStatic(strRt[i][j+1], 650 + j * 70, 550 + i * 100, 60, 30, NULL);
	}


	setup = true;
}

void freeSpriteTool()
{
	delete sprite;
	Sprite::cbFrame(dummyFrame);

	delete hBtnToolMgt;
	delete hLbSpriteData;
	for (int i = 0; i < 2; i++)
		delete hBtnUpDown[i];
	delete hBtnUpDown;
	delete hBtnStateMgt;
	delete hBtnDataMgt;
	delete hEbFramePosDelta;
	for (int i = 0; i < 2; i++)
		delete hEbFrameRt[i];
	delete hEbFrameRt;	
}

const char* strPathTex = NULL;
bool animation = false;
int aniIndex = 0;
const char* strPathSpr = NULL;
int prevLB = -1;
void drawSprite(float dt)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Frame* f = getCurrFrame(true);
	if (f == NULL) return;
	Texture* tex = f->t->tex;

	bool applyFrame = false;

#if 1
	int idx = indexWndListBox(hLbSpriteData[1]);
	if (idx != prevLB)
	{
		prevLB = idx;
		applyFrame = true;
	}
#endif

	if (animation)
	{
		setWndListBox(hLbSpriteData[1], aniIndex);

		applyFrame = true;

		sprite->delta += dt;
		if (sprite->delta >= f->delta)
		{
			sprite->delta -= f->delta;

			int count = countWndListBox(hLbSpriteData[1]);
			aniIndex++;
			if (aniIndex > count - 2)
			{
				aniIndex = 0;
				animation = getWndCheckBox(hCbRepeat);
				if (animation == false)
					setWndText(hBtnPlay, "Play");
			}

		}
	}

#if 1/////////////////////////////////////////////////
	if (applyFrame)
	{
		ignoreUpdateEditBox = true;
		setWndText(hEbFramePosDelta[0], "%.2f", f->p.x);
		setWndText(hEbFramePosDelta[1], "%.2f", f->p.y);

		setWndText(hEbFramePosDelta[2], "%.2f", f->delta);
		setWndText(hEbFrameRt[0][0], "%.2f", f->rtAtt.origin.x);
		setWndText(hEbFrameRt[0][1], "%.2f", f->rtAtt.origin.y);
		setWndText(hEbFrameRt[0][2], "%.2f", f->rtAtt.size.width);
		setWndText(hEbFrameRt[0][3], "%.2f", f->rtAtt.size.height);
		setWndText(hEbFrameRt[1][0], "%.2f", f->rtDef.origin.x);
		setWndText(hEbFrameRt[1][1], "%.2f", f->rtDef.origin.y);
		setWndText(hEbFrameRt[1][2], "%.2f", f->rtDef.size.width);
		setWndText(hEbFrameRt[1][3], "%.2f", f->rtDef.size.height);
		ignoreUpdateEditBox = false;
	}
#endif
	if (f->t->path[0] == 0 || f->t->path == NULL)
		setWndText(hStTexPath, "Path");
	else
		setWndText(hStTexPath, f->t->path);

	
	if (tex)
	{
#if 0
		sprite->paint(dt, iPointMake(devSize.width / 2, devSize.height - 150),
			getWndCheckBox(hCheckBoxReverse), getWndFloat(hEbScale), false);
#else
		//Frame* f = getCurrFrame();
		iPoint p = iPointMake(devSize.width / 2, devSize.height - 150); // 원점
		iPoint t = iPointMake(getWndInt(hEbFramePosDelta[0]), getWndInt(hEbFramePosDelta[1]));
		iPoint imgPos;
		bool leftRight = getWndCheckBox(hCheckBoxReverse);
		float scale = getWndFloat(hEbScale);

		if (leftRight)
			t.x = -(tex->width - fabsf(t.x));

		t *= scale;

		imgPos = p + t;
		drawImage(tex, imgPos.x, imgPos.y, TOP | LEFT, 0, 0,
			tex->width, tex->height, scale, scale, 2, 0, leftRight ? REVERSE_WIDTH : REVERSE_NONE);
#endif
		float defX = getWndFloat(hEbFrameRt[1][0]);
		float defY = getWndFloat(hEbFrameRt[1][1]);
		float defWidth = getWndFloat(hEbFrameRt[1][2]);
		float defHeight = getWndFloat(hEbFrameRt[1][3]);

		setRGBA(0, 0, 1, 1);
		setLineWidth(8);
		if (!leftRight)
			drawRect(p.x + defX * scale, p.y + defY * scale, defWidth * scale, defHeight * scale);
		else
			drawRect(p.x - (defX + defWidth) * scale, p.y + defY * scale, defWidth * scale, defHeight * scale);

		float atkX = getWndFloat(hEbFrameRt[0][0]);
		float atkY = getWndFloat(hEbFrameRt[0][1]);
		float atkWidth = getWndFloat(hEbFrameRt[0][2]);
		float atkHeight = getWndFloat(hEbFrameRt[0][3]);

		setRGBA(1, 0, 0, 1);
		if (!leftRight)
			drawRect(p.x + atkX * scale, p.y + atkY * scale, atkWidth * scale, atkHeight * scale);
		else
			drawRect(p.x  - (atkX + atkWidth) * scale, p.y + atkY * scale, atkWidth * scale, atkHeight * scale);
		setRGBA(1, 1, 1, 1);
	}

	setRGBA(0, 1, 0, 1);
	setLineWidth(10);
	drawLine(devSize.width / 2, 0, devSize.width / 2, devSize.height);
	drawLine(0, devSize.height - 150, devSize.width, devSize.height - 150);
	setRGBA(1, 1, 1, 1);

	if (strPathTex)
	{
		_loadFrameImg(strPathTex);
		strPathTex = NULL;
	}
	if (strPathSpr)
	{
		_loadSpriteFile(strPathSpr);
		strPathSpr = NULL;
	}
}

void updateEditBox(HWND hwnd)
{
	if (ignoreUpdateEditBox)
		return;

	Frame* f = getCurrFrame(true);

	if (hwnd == hEbFramePosDelta[0]) f->p.x = getWndFloat(hwnd);
	else if (hwnd == hEbFramePosDelta[1]) f->p.y = getWndFloat(hwnd);
	else if (hwnd == hEbFramePosDelta[2]) f->delta = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[0][0]) f->rtAtt.origin.x = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[0][1]) f->rtAtt.origin.y = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[0][2]) f->rtAtt.size.width = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[0][3]) f->rtAtt.size.height = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[1][0]) f->rtDef.origin.x = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[1][1]) f->rtDef.origin.y = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[1][2]) f->rtDef.size.width = getWndFloat(hwnd);
	else if (hwnd == hEbFrameRt[1][3]) f->rtDef.size.height = getWndFloat(hwnd);
}


iPoint prevPoint;
bool clicked = false;
void keySpriteTool(iKeyStat stat, iPoint point)
{
	switch (stat) {

	case iKeyStatBegan:
		if (point.x < 0 || point.x > rtOpenGL.size.width ||
			point.y < 0 || point.y > rtOpenGL.size.height)
				break;
		else {
			prevPoint = point;
			clicked = true;
			break;
		}

	case iKeyStatMoved:
		if (clicked)
		{
			// 좌표 coordinate해야 된당
			iPoint mp = point - prevPoint;
			prevPoint = point;

			iPoint t = iPointMake(getWndFloat(hEbFramePosDelta[0]), getWndFloat(hEbFramePosDelta[1]));
			t += mp;
			setWndText(hEbFramePosDelta[0], "%.2f", t.x);
			setWndText(hEbFramePosDelta[1], "%.2f", t.y);
#if 1
			float f = getWndFloat(hEbFrameRt[0][0]);
			setWndText(hEbFrameRt[0][0], "%.2f", f + mp.x);
			f = getWndFloat(hEbFrameRt[0][1]);
			setWndText(hEbFrameRt[0][1], "%.2f", f + mp.y);

			f = getWndFloat(hEbFrameRt[1][0]);
			setWndText(hEbFrameRt[1][0], "%.2f", f + mp.x);
			f = getWndFloat(hEbFrameRt[1][1]);
			setWndText(hEbFrameRt[1][1], "%.2f", f + mp.y);
#endif
			break;
		}

	case iKeyStatEnded:
		clicked = false;
		break;
	}
}

void updateSpriteTool(WPARAM wParam, LPARAM lParam)
{
	if (!setup) return;
	int i, j;

	HWND hwnd = (HWND)lParam;

	if (hwnd == hBtnImgLoad)
	{
		loadFrameImg();
		return;
	}
	else if (hwnd == hCheckBoxReverse || hwnd == hCbRepeat)
	{
		bool on = getWndCheckBox(hwnd);
		setWndCheckBox(hwnd, !on);
	}
	else if (hwnd == hBtnPlay)
	{
		char* str = getWndText(hBtnPlay);
		if (strcmp(str, "Play") == 0)
		{
			animation = true;
			sprite->delta = 0.0f;
			setWndText(hBtnPlay, "Stop");
		}
		else
		{
			animation = false;
			sprite->delta = 0.0f;
			setWndText(hBtnPlay, "Play");
			aniIndex = 0;
		}
		delete str;
	}

	for (i = 0; i < 3; i++)
	{
		if (hwnd == hBtnStateMgt[i])
		{
			void (*method[3])() = {
				addState, delState, udtState
			};

			method[i]();
			return;
		}
	}

	for (i = 0; i < 2; i++)
	{
		if (hwnd == hBtnToolMgt[i])
		{
			void (*method[2])() = {
				saveSpriteFile, loadSpriteFile
			};

			method[i]();
			return;
		}
		else if (hwnd == hLbSpriteData[i])
		{
			if (i == 0)
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					int index = indexWndListBox(hwnd);
					int count = countWndListBox(hwnd);
					if (index == count - 1)
						return;

					iArray* arrayFrame = (iArray*)sprite->arrayBe->objectAtIndex(index);

					clearWndListBox(hLbSpriteData[1]);
					for (int i = 0; i < arrayFrame->count; i++)
					{ 
						char strIndex[3];
						sprintf(strIndex, "%d", i);
						addWndListBox(hLbSpriteData[1], i, strIndex);
					}

					addWndListBox(hLbSpriteData[1], arrayFrame->count, strLB[0][1]);
					prevLB = -1;
					setWndListBox(hLbSpriteData[1], 0);

					return;
				}
			}
			else //if (i == 1)
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
#if 0 /////////////////////////////////////////////////
					Frame* f = getCurrFrame(false);
					//ignoreUpdateEditBox = true;
					setWndText(hStTexName, f->t->path);
					setWndText(hEbFramePosDelta[0], "%.2f", f->p.x);
					setWndText(hEbFramePosDelta[1], "%.2f", f->p.y);
					setWndText(hEbFramePosDelta[2], "%.2f", f->delta);
					setWndText(hEbFrameRt[0][0], "%.2f", f->rtAtt.origin.x);
					setWndText(hEbFrameRt[0][1], "%.2f", f->rtAtt.origin.y);
					setWndText(hEbFrameRt[0][2], "%.2f", f->rtAtt.size.width);
					setWndText(hEbFrameRt[0][3], "%.2f", f->rtAtt.size.height);
					setWndText(hEbFrameRt[1][0], "%.2f", f->rtDef.origin.x);
					setWndText(hEbFrameRt[1][1], "%.2f", f->rtDef.origin.y);
					setWndText(hEbFrameRt[1][2], "%.2f", f->rtDef.size.width);
					setWndText(hEbFrameRt[1][3], "%.2f", f->rtDef.size.height);
					//ignoreUpdateEditBox = false;
#endif
					return;
				}
			}
		}

		for(j = 0; j < 2; j++)
		{
			if (hwnd == hBtnUpDown[i][j])
			{
				void (*method[4])() = {
					upState, downState, upFrame, downFrame
				};

				method[(2 * i) + j]();
				return;
			}
		}
	}

	for (i = 0; i < 2; i++)
	{
		if (hwnd == hBtnDataMgt[i])
		{
			void (*method[2])() = {
				addFrame, delFrame
			};
			
			method[i]();
			return;
		}
	}
}

void loadSpriteFile()
{
	const char* path = openFileDlg(true, L"Sprite Files(*.spr)\0*.spr;.SPR\0", L"\\assets\\spritedata");
	if (path == NULL)
		return;

	strPathSpr = path;
}

void _loadSpriteFile(const char* path)
{
	// 저장된 sprite를 읽어서 load
	Sprite::cbFrame(dummyFrame);
	dummyFrame = genDummyFrame();

	if (sprite)
		delete sprite;

	sprite = new Sprite();

	wchar_t _dir[512];
	GetCurrentDirectory(512, _dir);
	char* dir = utf16_to_utf8(_dir);
	int lenDir = strlen(dir);
	lenDir++;

	clearWndListBox(hLbSpriteData[0]);
	clearWndListBox(hLbSpriteData[1]);

	int len;
	char* buf = loadFile(path, len);
	len = 0;

	int beNum;
	memcpy(&beNum, &buf[len], sizeof(int)); len += sizeof(int);
	for (int i = 0; i < beNum; i++)
	{
		int lenState;
		memcpy(&lenState, &buf[len], sizeof(int)); len += sizeof(int);
		char* state = new char[lenState + 1];
		memcpy(state, &buf[len], sizeof(char) * lenState); len += sizeof(char) * lenState;
		state[lenState] = 0;
		int index = countWndListBox(hLbSpriteData[0]);
		addWndListBox(hLbSpriteData[0], index, state);
		delete state;

		iArray* arrayFrame = new iArray(Sprite::cbFrame);
		sprite->arrayBe->addObject(arrayFrame);

		int frameNum;
		memcpy(&frameNum, &buf[len], sizeof(int)); len += sizeof(int);
		for (int j = 0; j < frameNum; j++)
		{
			iArray* arrayFrameAtIndex = (iArray*)sprite->arrayBe->objectAtIndex(i);
			Frame* f = new Frame;

			int lenPath;
			memcpy(&lenPath, &buf[len], sizeof(int)); len += sizeof(int);
			char* path = new char[lenPath + 1];
			memcpy(path, &buf[len], sizeof(char) * lenPath); len += sizeof(char) * lenPath;
			path[lenPath] = 0;
			f->t = new Tex;
			f->t->path = path;

			if (strstr(path, dir))
			{
				char* relPath = new char[lenPath - lenDir + 1];
				strcpy(relPath, &path[lenDir]);
				f->t->path = relPath;
				delete path;
			}

			f->t->tex = NULL;// getCurrFrame()
			memcpy(&f->p, &buf[len], sizeof(iPoint)); len += sizeof(iPoint);
			memcpy(&f->delta, &buf[len], sizeof(float)); len += sizeof(float);
			memcpy(&f->rtAtt, &buf[len], sizeof(iRect)); len += sizeof(iRect);
			memcpy(&f->rtDef, &buf[len], sizeof(iRect)); len += sizeof(iRect);

			arrayFrameAtIndex->addObject(f);
		}
	}

	addWndListBox(hLbSpriteData[0], countWndListBox(hLbSpriteData[0]), strLB[0][0]);
	addWndListBox(hLbSpriteData[1], countWndListBox(hLbSpriteData[1]), strLB[1][0]);
	setWndListBox(hLbSpriteData[0], countWndListBox(hLbSpriteData[0]) - 1);
	setWndListBox(hLbSpriteData[1], countWndListBox(hLbSpriteData[1]) - 1);

	delete buf;
}

void saveSpriteFile()
{
	const char* path = openFileDlg(false, L"Sprite Files(*.spr)\0*.spr;.SPR\0", L"\\assets\\spritedata");

	if (path == NULL)
		return;

	char strPath[1024];
	int l = strlen(path);

	if (strcmp(&path[l - 4], ".spr") == 0 ||
		strcmp(&path[l - 4], ".SPR") == 0)
		strcpy(strPath, path);
	else
	{
		sprintf(strPath, "%s.spr\0", path);
	}

	FILE* pf = fopen(strPath, "wb");
	
	// sprite의 데이터를 파일로 저장
	// 각 arrayBe의 Frame들을 저장
	int i, j;

	int beNum = sprite->arrayBe->count;
	fwrite(&beNum, sizeof(int), 1, pf);
	for (i = 0; i < beNum; i++)
	{
		iArray* arrayFrame = (iArray*)sprite->arrayBe->objectAtIndex(i);

		char* state = getWndListBox(hLbSpriteData[0], i);
		int lenState = strlen(state);
		fwrite(&lenState, sizeof(int), 1, pf);
		fwrite(state, sizeof(char), lenState, pf);
		delete state;

		int frameNum = arrayFrame->count;
		fwrite(&frameNum, sizeof(int), 1, pf);
		for (j = 0; j < frameNum; j++)
		{
			Frame* f = (Frame*)arrayFrame->objectAtIndex(j);
			int lenPath = strlen(f->t->path);
			fwrite(&lenPath, sizeof(int), 1, pf);
			fwrite(f->t->path, sizeof(char), lenPath, pf);
			fwrite(&f->p, sizeof(iPoint), 1, pf);
			fwrite(&f->delta, sizeof(float), 1, pf);
			fwrite(&f->rtAtt, sizeof(iRect), 1, pf);
			fwrite(&f->rtDef, sizeof(iRect), 1, pf);
		}
	}

	fclose(pf);
}

void loadFrameImg()
{
	strPathTex = openFileDlg(true, L"PNG Files(*.png)\0*.png;.PNG\0", L"\\assets\\object\\entity");
}

Frame* getCurrFrame(bool bCreateTex)
{
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index == count - 1)
		return NULL;

	iArray* arrayFrame = (iArray*)sprite->arrayBe->objectAtIndex(index);
	index = indexWndListBox(hLbSpriteData[1]);
	count = countWndListBox(hLbSpriteData[1]);

	Frame* f = dummyFrame;
	if (index < count - 1)
	{
		f = (Frame*)arrayFrame->objectAtIndex(index);
		if (f != NULL && bCreateTex && f->t->path && f->t->tex == NULL)
			f->t->tex = addTextureInTex(f->t->path);
	}

	return f;
}

void _loadFrameImg(const char* path)
{
	Frame* f = getCurrFrame(false);

	wchar_t _dir[512];

	GetCurrentDirectory(512, _dir);
	char* dir = utf16_to_utf8(_dir);
	int lenDir = strlen(dir);
	lenDir++;

	char* p;
	int lenPath = strlen(path);
	if (strstr(path, dir))
	{
		p = new char[lenPath - lenDir + 1];
		strcpy(p, &path[lenDir]);
	}
	else
	{
		p = new char[lenPath + 1];
		strcpy(p, path);
	}

	if (f->t->path)
		delete f->t->path;
	f->t->tex = addTextureInTex(p);
	f->t->path = p;

	delete dir;

	setWndText(hStTexPath, f->t->path);

	setWndText(hEbFramePosDelta[0], "%.2f", -f->t->tex->width / 2);
	setWndText(hEbFramePosDelta[1], "%.2f", -f->t->tex->height);
	setWndText(hEbFramePosDelta[2], "%.2f", 0.18f);

	setWndText(hEbFrameRt[1][0], "%.2f", -f->t->tex->width/2);
	setWndText(hEbFrameRt[1][1], "%.2f", -f->t->tex->height);
	setWndText(hEbFrameRt[1][2], "%.2f", f->t->tex->width);
	setWndText(hEbFrameRt[1][3], "%.2f", f->t->tex->height);
}

void addState()
{
	char* name = getWndText(hEbStateName);
	int index = countWndListBox(hLbSpriteData[0]) - 1;
	if (index == -1)
		index = 0;

	iArray* arrayFrame = new iArray(Sprite::cbFrame);
	sprite->arrayBe->addObject(arrayFrame);

	addWndListBox(hLbSpriteData[0], index, name);
	setWndListBox(hLbSpriteData[0], index + 1);
	setWndText(hEbStateName, "");
	SetFocus(hEbStateName);
}

void delState()
{
	int index = indexWndListBox(hLbSpriteData[0]);
	if (index == -1 || index >= countWndListBox(hLbSpriteData[0])-1)
		return;

	sprite->arrayBe->removeObject(index);
	removeWndListBox(hLbSpriteData[0], index);
	setWndListBox(hLbSpriteData[0], index);
	setWndText(hEbStateName, "");
}

void udtState()
{
	int index = indexWndListBox(hLbSpriteData[0]);
	if (index == -1 || index == countWndListBox(hLbSpriteData[0]) - 1)
		return;

	char* str = getWndText(hEbStateName);
	addWndListBox(hLbSpriteData[0], index, str);
	removeWndListBox(hLbSpriteData[0], index + 1);

	setWndListBox(hLbSpriteData[0], index);
}

void upState()
{
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index == count - 1 || index <= 0)
		return;

#if 0
	iArray* af0 = (iArray*)sprite->arrayBe->objectAtIndex(index - 1);
	iArray* af1 = (iArray*)sprite->arrayBe->objectAtIndex(index);
	iArray a;
	
	memcpy(&a, af0, sizeof(iArray));
	memcpy(af0, af1, sizeof(iArray));
	memcpy(af1, &a, sizeof(iArray));
#else
	//sprite->arrayBe->objectAtIndex(index)
	iNode* n = sprite->arrayBe->node;
	int i = count - 1;
	for (; n;)
	{
		if (i == index + 1)
			break;// return n->data;
		n = n->prev;
		i--;
	}

	void* d = n->data;// index
	n->data = n->prev->data; // index - 1
	n->prev->data = d;

#endif

	char* str = getWndListBox(hLbSpriteData[0], index - 1);
	removeWndListBox(hLbSpriteData[0], index - 1);
	addWndListBox(hLbSpriteData[0], index, str);

	setWndListBox(hLbSpriteData[0], index - 1);
}

void downState()
{
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index >= count - 2)
		return;

#if 0 
	iArray* af0 = (iArray*)sprite->arrayBe->objectAtIndex(index);
	iArray* af1 = (iArray*)sprite->arrayBe->objectAtIndex(index + 1);
	iArray a;

	memcpy(&a, af0, sizeof(iArray));
	memcpy(af0, af1, sizeof(iArray));
	memcpy(af1, &a, sizeof(iArray));
#else
	iNode* n = sprite->arrayBe->node;
	int i = count - 1;
	for (; n;)
	{
		if (i == index + 2)
			break;// return n->data;
		n = n->prev;
		i--;
	}

	void* d = n->data;// index
	n->data = n->prev->data; // index - 1
	n->prev->data = d;


#endif
	char* str = getWndListBox(hLbSpriteData[0], index + 1);
	removeWndListBox(hLbSpriteData[0], index + 1);
	addWndListBox(hLbSpriteData[0], index, str);

	setWndListBox(hLbSpriteData[0], index + 1);
}


void addFrame()
{
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index == count - 1)
		return;
	
	iArray* arrayFrame = (iArray*)sprite->arrayBe->objectAtIndex(index); 
	dummyFrame->p = iPointMake(getWndFloat(hEbFramePosDelta[0]), getWndFloat(hEbFramePosDelta[1]));
	dummyFrame->delta = getWndFloat(hEbFramePosDelta[2]);
	dummyFrame->rtAtt = iRectMake(getWndFloat(hEbFrameRt[0][0]), getWndFloat(hEbFrameRt[0][1]),
								  getWndFloat(hEbFrameRt[0][2]), getWndFloat(hEbFrameRt[0][3]));
	dummyFrame->rtDef = iRectMake(getWndFloat(hEbFrameRt[1][0]), getWndFloat(hEbFrameRt[1][1]),
								  getWndFloat(hEbFrameRt[1][2]), getWndFloat(hEbFrameRt[1][3]));

	arrayFrame->addObject(dummyFrame);

	dummyFrame = genDummyFrame();

	index = countWndListBox(hLbSpriteData[1]) - 1;
	char strIndex[3];
	sprintf(strIndex, "%d", index);
	addWndListBox(hLbSpriteData[1], index, strIndex);
}

void delFrame()
{
	// obejctAtIndex로 접근해서 Frame 삭제하고 lb랑 array 데이터 정리
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index < 0 || index == count - 1)
		return;

	iArray* arrayFrame = (iArray*)(sprite->arrayBe->objectAtIndex(index));

	index = indexWndListBox(hLbSpriteData[1]);
	count = countWndListBox(hLbSpriteData[1]);
	if (index < 0 || index == count - 1)
		return;

	arrayFrame->removeObject(index);
	index--;

	if (index < 0)
		index = 0;
	removeWndListBox(hLbSpriteData[1], count - 2);
	setWndListBox(hLbSpriteData[1], index);
	
	dummyFrame = genDummyFrame();
	// Eb들 다 비우기
}

void applyFrame()
{
	Frame* f = getCurrFrame(false);

	f->p = iPointMake(getWndFloat(hEbFramePosDelta[0]), getWndFloat(hEbFramePosDelta[1]));
	f->delta = getWndFloat(hEbFramePosDelta[2]);
	f->rtAtt = iRectMake(getWndFloat(hEbFrameRt[0][0]), getWndFloat(hEbFrameRt[0][1]),
		getWndFloat(hEbFrameRt[0][2]), getWndFloat(hEbFrameRt[0][3]));
	f->rtDef = iRectMake(getWndFloat(hEbFrameRt[1][0]), getWndFloat(hEbFrameRt[1][1]),
		getWndFloat(hEbFrameRt[1][2]), getWndFloat(hEbFrameRt[1][3]));
	f->t = dummyFrame->t;
	dummyFrame->t = NULL;
}

void upFrame()
{
	// 데이터만 바꿔주면 된다.
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index < 0 || index > count - 2)
		return;

	iArray* arrayFrame = (iArray*)sprite->arrayBe->objectAtIndex(index);

	index = indexWndListBox(hLbSpriteData[1]);
	count = countWndListBox(hLbSpriteData[1]);
	if (index == 0 || index == count - 1)
		return;

	Frame* f0 = (Frame*)arrayFrame->objectAtIndex(index - 1);
	Frame* f1 = (Frame*)arrayFrame->objectAtIndex(index);
	Frame f;

	memcpy(&f, f0, sizeof(Frame));
	memcpy(f0, f1, sizeof(Frame));
	memcpy(f1, &f, sizeof(Frame));

	setWndListBox(hLbSpriteData[1], index - 1);
}

void downFrame()
{
	int index = indexWndListBox(hLbSpriteData[0]);
	int count = countWndListBox(hLbSpriteData[0]);
	if (index > count - 2)
		return;

	iArray* arrayFrame = (iArray*)sprite->arrayBe->objectAtIndex(index);

	index = indexWndListBox(hLbSpriteData[1]);
	count = countWndListBox(hLbSpriteData[1]);
	if (index >= count - 2)
		return;

	Frame* f0 = (Frame*)arrayFrame->objectAtIndex(index);
	Frame* f1 = (Frame*)arrayFrame->objectAtIndex(index + 1);
	Frame f;

	memcpy(&f, f0, sizeof(Frame));
	memcpy(f0, f1, sizeof(Frame));
	memcpy(f1, &f, sizeof(Frame));

	setWndListBox(hLbSpriteData[1], index + 1);
}

Frame* genDummyFrame()
{
	Frame* f = new Frame;

	f->t = new Tex;
	f->t->path = new char;
	f->t->path[0] = 0;
	f->t->tex = NULL;
	f->p = iPointZero;
	f->delta = 0.0f;
	f->rtAtt = iRectZero;
	f->rtDef = iRectZero;

	return f;
}

