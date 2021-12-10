#include "Loading.h"

#include "Intro.h"
#include "StageEditor.h"
#include "Tilemap.h"
#include "ProcData.h"

struct loadData
{
	int tNum;
	int tmX;
	int tmY;
	int tmW;

	Tile* tm;
};

const char* strName;
void loadStageEditor()
{
	strName = getStringName();
	setStringName("assets/UI/font/BMJUA_ttf.ttf");
	createPopEditorMenu();
	createPopField();
	createPopTiles();
	createPopEntity();

	showPopEditorMenu(false);
	showPopField(true);
	showPopTiles(true);
	showPopEntity(true);
}

char* stageDataPath = NULL;
bool toggleMenu = false;
void drawStageEditor(float dt)
{
	setTexture(CLAMP, NEAREST);
	drawPopField(dt);
	drawPopTiles(dt);
	drawPopEditorMenu(dt);
	drawPopEntity(dt);

	if (keyDown & keysESC)
	{
		showPopEditorMenu(toggleMenu = !toggleMenu);
	}

	// 여기로 로직을 옮기기
	if (stageDataPath)
	{
		loadData* ld = new loadData;
		freePopField();
		loadStageData(stageDataPath, ld);

		createPopField(ld);
		delete ld;
		stageDataPath = NULL;
	}

	drawEntityScale(dt);
	setTexture(CLAMP, LINEAR);
}

void freeStageEditor()
{
	freePopField();
	freePopTiles();
	freePopEditorMenu();
	freePopEntity();
}

void keyStageEditor(iKeyStat stat, iPoint point, bool leftRight)
{
	if (keyPopEditorMenu(stat, point)	||
		keyPopTiles(stat, point)		||
		keyPopEntity(stat, point)		||
		keyPopField(stat, point, leftRight)
		)
		return;
}

// -----------------------------------------
// popField
// -----------------------------------------
iPopup* popField;

Tile* tm;
int tmX, tmY, tNum;
int selectedField;
iPoint offField;
iSize _sizeMap;

Texture* texFieldFbo;

void createPopField(void* data)
{
	iPopup* pop = new iPopup();
	popField = pop;

	int i;
	tmX = 80;
	tmY = 54;
	tNum = tmX * tmY;
	if (data == NULL)
	{
		tm = new Tile[tNum];
		for (i = 0; i < tNum; i++)
		{
			Tile* t = &tm[i];
			t->value = -1;
			t->ti = { None, true };
		}
	}
	else
	{
		loadData* ld = (loadData*)data;
		tm = ld->tm;
		ld = NULL;
	}

	_sizeMap = iSizeMake(tmX * TILEWIDTH, tmY * TILEHEIGHT);
	selectedField = -1;
	offField = iPointZero;
	texFieldFbo = createTexture(_sizeMap.width / 10.f, _sizeMap.height / 10.f);
}

void freePopField()
{
	delete popField;
	delete tm;
	freeImage(texFieldFbo);
}
void showPopField(bool show)
{
	popField->show(show);
}

bool toggleMap = true;
void drawPopField(float dt)
{
	popField->paint(dt);

	fbo->bind(texFieldFbo);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fbo->unbind();

	Texture* tex;
	for (int j = 0; j < tmY; j++)
	{
		for (int i = 0; i < tmX; i++)
		{
			Tile* t = &tm[tmX * j + i];
			if (t->value == -1) continue;

			float x = offField.x + TILEWIDTH * i;
			float y = offField.y + TILEHEIGHT * j;

			if (t->value % THEME_INTERVAL  < 100)
			{
				// # to do.. 수정해야됨
				//tex = texTiles[selectedTheme][t->value];
				tex = texTiles[t->value / THEME_INTERVAL][t->value % THEME_INTERVAL];

				drawImage(tex, x, y, TOP | LEFT);
				fbo->bind(texFieldFbo);
				drawImage(tex, TILEWIDTH * i / 10.f, TILEHEIGHT * j / 10.f, TOP | LEFT, 0, 0, tex->width, tex->height, 0.1, 0.1, 2, 0);
				fbo->unbind();
			}
			else
			{
				tex = texEntity[(t->value % THEME_INTERVAL) - 100];
				float scale = (float)t->level / 2;
				if (scale == 0)
					scale = 0.3;
//				drawImage(tex, x + 25, y + 49, BOTTOM | HCENTER,
//					0, 0, tex->width, tex->height, scale, scale, 2, 0);
				drawImage(tex, x + TILEWIDTH / 2, y + TILEHEIGHT - 1, BOTTOM | HCENTER,
					0, 0, tex->width, tex->height, scale, scale, 2, 0);
				fbo->bind(texFieldFbo);
				drawImage(tex, (TILEWIDTH * i + 25) / 10.f, (TILEHEIGHT * j + 49) / 10.f,
					BOTTOM | HCENTER, 0, 0, tex->width, tex->height, 0.1 * scale, 0.1 * scale, 2, 0);
				fbo->unbind();
			}
		}
	}
	iSize _size = devSize;
	fbo->bind(texFieldFbo);
	setRGBA(1, 1, 0, 1);
#if 0
	iSize ds = fbo->size;
	drawRect(-offField.x / 10.f, -offField.y / 10.f, ds.width / 10.f, ds.height / 10.f);
	setRGBA(1, 1, 0, 0.3f);
	fillRect(-offField.x / 10.f, -offField.y / 10.f, ds.width / 10.f, ds.height / 10.f);
#else
	drawRect(-offField.x / 10.f, -offField.y / 10.f, _size.width / 10.f, _size.height / 10.f);
	setRGBA(1, 1, 0, 0.3f);
	fillRect(-offField.x / 10.f, -offField.y / 10.f, _size.width / 10.f, _size.height / 10.f);
	devSize = _size;
#endif
	fbo->unbind();
	setRGBA(1, 1, 1, 1);

	int x = selectedField % tmX;
	int y = selectedField / tmX;

	setRGBA(1, 0, 0, 1);
	drawRect(offField.x + TILEWIDTH * x, offField.y + TILEHEIGHT * y, TILEWIDTH, TILEHEIGHT);
	setRGBA(1, 1, 1, 1);


	if (keyDown & keysM)
		toggleMap = !toggleMap;
	if (toggleMap)
	{
		Texture* t = texFieldFbo;
		setGLBlend(GLBlendMultiplied);
		drawImage(t, devSize.width, devSize.height, BOTTOM | RIGHT, 0, 0, t->width, t->height, 1.0f, 1.0f, 2, 0, REVERSE_HEIGHT);
		setGLBlend(GLBlendAlpha);
	}
}

char _leftRight = 0;
bool scrollField = false;
iPoint prevField;

int startPoint = -1;
bool keyPopField(iKeyStat stat, iPoint point, bool leftRight)
{
	int x = point.x - offField.x; x /= TILEWIDTH;
	int y = point.y - offField.y; y /= TILEHEIGHT;

	switch (stat) {

	case iKeyStatBegan:
		if (keyStat & keysCTRL)
		{
			prevField = point;
			scrollField = true;
			break;
		}
		if (x < tmX && y < tmY)
		{
			Tile* t = &tm[tmX * y + x];
			if (leftRight == false)
			{
				t->value = selectedObj;
				if (t->value < 0) break;

				if (t->value > -1 && t->value < 4)
				{
					t->ti = { Solid, true };
					t->value += selectedTheme * THEME_INTERVAL;
				}
				else if (t->value < 100)
				{
					t->ti = { Solid, false };
					t->value += selectedTheme * THEME_INTERVAL;
				}
				else if (t->value >= 100)
				{
					if (t->value == 100)
					{
						if (startPoint != -1)
						{
							tm[startPoint].value = -1;
						}

						startPoint = tmX * y + x;
					}
					t->level = 2;
					t->ti = { None, true };
				}

				_leftRight = 1;
			}
			else
			{
				t->value = -1;
				t->ti = { None, true };

				_leftRight = 2;
			}
		}
		return true;

	case iKeyStatMoved:
		if (scrollField)
		{
			offField += (point - prevField);
			if (offField.x > 0)
				offField.x = 0;
			else if (offField.x < devSize.width - _sizeMap.width)
				offField.x = devSize.width - _sizeMap.width;
			if (offField.y > 0)
				offField.y = 0;
			else if (offField.y < devSize.height - _sizeMap.height)
				offField.y = devSize.height - _sizeMap.height;

			prevField = point;
			break;
		}
		if (x < tmX && y < tmY)
		{
			// 테마에 따라 분류하고 
			Tile* t = &tm[tmX * y + x];
			if (_leftRight == 1)
			{
				t->value = selectedObj;
				if (t->value > -1 && t->value < 4)
				{
					t->ti = { Solid, true };
					t->value += selectedTheme * THEME_INTERVAL;
				}
				else if (t->value < 100)
				{
					t->ti = { Solid, false }; 
					t->value += selectedTheme * THEME_INTERVAL;
				}
				else if (t->value == 100)
				{
					if (startPoint == tmX * y + x)
						break;

					if (startPoint != -1)
					{
						tm[startPoint].value = -1;
					}

					startPoint = tmX * y + x;
				}
			}
			else if (_leftRight == 2)
			{
				t->value = -1;
				t->ti = { None, true };
			}

			selectedField = tmX * y + x;
			printf("(%d, %d)\n", x, y);
		}
		else
			selectedField = -1;
		break;

	case iKeyStatEnded:
		if (scrollField)
		{
			prevField = point;
			scrollField = false;
			break;
		}
		_leftRight = 0;
		break;
	}

	return false;
}

// -----------------------------------------
// popTiles
// -----------------------------------------
iPopup* popTiles;
iImage*** imgTilesBtn;
iImage* imgTilesOpenCloseBtn;
iImage** imgIndexBtn;
Texture*** texTiles;

int selectedObj;
int selectedTheme = -1;

void drawPopTilesAfter(iPopup* pop, float dt, float rate);
void drawPopTilesBefore(iPopup* pop, float dt, float rate);
void closePopTiles(iPopup* pop);
void setTheme(int index);

void createPopTiles()
{
	int i, j;

	iPopup* pop = new iPopup();

	imgTilesOpenCloseBtn = new iImage();

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(30, 50);

	for (i = 0; i < 2; i++)
	{
		Texture* tex;
		g->init(size);

		setRGBA(0, 0, 0, 0.6);
		g->fillRect(0, 0, size.width, size.height);
		setRGBA(1, 1, 1, 1);

		setStringRGBA(1, 1, 1, 1);
		setStringSize(20);
		setStringBorder(2);
		setStringBorderRGBA(1, 1, 1, 1);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, i < 1 ? "<" : ">");

		tex = g->getTexture();
		imgTilesOpenCloseBtn->addObject(tex);
		freeImage(tex);
	}
	imgTilesOpenCloseBtn->position = iPointMake(-size.width, 0);
	pop->addObject(imgTilesOpenCloseBtn);

	imgIndexBtn = new iImage * [KIND_TILE];
	size = iSizeMake(40, 25);

	for (i = 0; i < KIND_TILE; i++)
	{
		iImage* img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);
			Texture* tex;
			if (j == 0) setRGBA(0, 0, 0, 0.3f);
			else		setRGBA(0, 0, 0, 0.8f);

			g->fillRect(0, 0, size.width, size.height);

			setRGBA(0, 0, 0, 1);
			g->drawRect(0, 0, size.width, size.height);
			setRGBA(1, 1, 1, 1);

			if (j == 0)
			{
				setStringRGBA(0, 0, 0, 1);
				setStringBorderRGBA(1, 1, 1, 1);
			}
			else
			{
				setStringRGBA(1, 1, 1, 1);
				setStringBorderRGBA(1, 1, 1, 1);
			}
			setStringSize(20);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%d", i + 1);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(40 * i, 160);
		pop->addObject(img);
		imgIndexBtn[i] = img;
	}

	imgTilesBtn = new iImage * * [KIND_TILE];
	texTiles = new Texture * * [KIND_TILE];

	for (i = 0; i < KIND_TILE; i++)
	{
		int cnt = _tileData[i].tileCnt;
		imgTilesBtn[i] = new iImage * [cnt];
		texTiles[i] = new Texture * [cnt];
		for (j = 0; j < cnt; j++)
		{
			Texture* t = createImage("assets/tiles/%s/%d.png", _tileData[i].path, j);
			float ts = TILEWIDTH / t->width;
			t->width *= ts;
			t->height *= ts;
			t->potWidth *= ts;
			t->potHeight *= ts;
			texTiles[i][j] = t;
			
			iImage* img = new iImage();
			img->addObject(t);
			freeImage(t);
			img->position = iPointMake(TILEWIDTH * (j % 6), TILEHEIGHT * (j / 6));
			pop->addObject(img);
			imgTilesBtn[i][j] = img;
		}
	}

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width, 0);
	pop->closePoint = iPointMake(devSize.width - 240, 0);
	pop->methodBefore = drawPopTilesBefore;
	pop->methodAfter = drawPopTilesAfter;
	pop->methodClose = closePopTiles;
	popTiles = pop;

	selectedObj = -1;
	setTheme(0);
	imgTilesOpenCloseBtn->setTexObject(1);
}

void freePopTiles()
{
	for (int i = 0; i < KIND_TILE; i++)
	{
		delete imgTilesBtn[i];
		delete texTiles[i];
	}
	delete imgTilesBtn;
	delete texTiles;

	delete imgIndexBtn;
	
	delete popTiles;
}
void showPopTiles(bool show)
{
	popTiles->show(show);
	if (show)
	{
		popTiles->stat = iPopupOpen;
		popTiles->bShow = true;
		popTiles->aniDt = 0.0f;
	}
	popTiles->openPoint = iPointMake(devSize.width, 0);
	popTiles->closePoint = iPointMake(devSize.width - 240, 0);
}

void closePopTiles(iPopup* pop)
{
	pop->openPoint = iPointMake(devSize.width - 240, 0);
	pop->closePoint = iPointMake(devSize.width, 0);
	pop->stat = iPopupProc;
	pop->bShow = true;
}

void drawPopTilesBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < KIND_TILE; i++)
		imgIndexBtn[i]->setTexObject(i == selectedTheme);
}

void drawPopTiles(float dt)
{
	popTiles->paint(dt);
}


void drawPopTilesAfter(iPopup* pop, float dt, float rate)
{
	int i = selectedObj;

	if (i >= 0 && i < 100)
	{
		if (popTiles->stat == iPopupProc)
		{
			setRGBA(1, 0, 0, 0.4f);
			iPoint p = popTiles->closePoint + iPointMake(TILEWIDTH * (i % 6), TILEHEIGHT * (i / 6));
			fillRect(p.x, p.y, TILEWIDTH, TILEHEIGHT);
			setRGBA(1, 1, 1, 1);
		}
	}

	i = popTiles->selected;
	if (i == -1)
		return;

	setRGBA(0, 0, 1, 1);
	iPoint p = popTiles->closePoint + iPointMake(TILEWIDTH * (i % 6), TILEHEIGHT * (i / 6));
	drawRect(p.x, p.y, TILEWIDTH, TILEHEIGHT);
	setRGBA(1, 1, 1, 1);
}

void setTheme(int index)
{
	if (selectedTheme == index)
		return;
	selectedTheme = index;

	iImage** imgs;
	for (int i = 0; i < KIND_TILE; i++)
	{
		imgs = imgTilesBtn[i];
		for (int j = 0; j < _tileData[i].tileCnt; j++)
			imgs[j]->alpha = i == index ? 1.0f : 0.0f;
	}
}

bool tilesOpenClose = true;
bool keyPopTiles(iKeyStat stat, iPoint point)
{
	if (popTiles->bShow == false ||
		containPoint(point, popTiles->touchRect()) == false)
		return false;

	iImage** imgs;
	int i, j = -1;

	switch (stat) {
		
	case iKeyStatBegan:
		if (containPoint(point, imgTilesOpenCloseBtn->touchRect(popTiles->closePoint)))
		{
			tilesOpenClose = !tilesOpenClose;
			imgTilesOpenCloseBtn->setTexObject(tilesOpenClose);
			showPopTiles(tilesOpenClose);
			return true;
		}

		for (int i = 0; i < KIND_TILE; i++)
		{
			if (containPoint(point, imgIndexBtn[i]->touchRect(popTiles->closePoint)))
				setTheme(i);
		}

		i = popTiles->selected;
		if (i == -1) break;

		selectedObj = i;

		return true;//break;
	case iKeyStatMoved:
		imgs = imgTilesBtn[selectedTheme];
		for (i = 0; i < _tileData[selectedTheme].tileCnt; i++)
		{
			if (containPoint(point, imgs[i]->touchRect(popTiles->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (popTiles->selected != j)
		{
			// audioPlay
			popTiles->selected = j;
			return true;
		}
		break;
	case iKeyStatEnded:
		break;
	}

	return true;
}

// -----------------------------------------
// popEntity
// -----------------------------------------
iPopup* popEntity;
iImage* imgEntityOpenCloseBtn;
iImage** imgEntityBtn;
Texture** texEntity;

int selectedEntity;

void drawPopEntityAfter(iPopup* pop, float dt, float rate);
void closePopEntity(iPopup* pop);

void createPopEntity()
{
	iImage* img;
	Texture* tex;
	int i, j;

	iPopup* pop = new iPopup();

	imgEntityOpenCloseBtn = new iImage();

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(30, 50);

	for (i = 0; i < 2; i++)
	{
		g->init(size);

		setRGBA(0, 0, 0, 0.6);
		g->fillRect(0, 0, size.width, size.height);
		setRGBA(1, 1, 1, 1);

		setStringRGBA(1, 1, 1, 1);
		setStringSize(20);
		setStringBorder(2);
		setStringBorderRGBA(1, 1, 1, 1);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, i < 1 ? ">" : "<");

		tex = g->getTexture();
		imgEntityOpenCloseBtn->addObject(tex);
		freeImage(tex);
	}
	imgEntityOpenCloseBtn->position = iPointMake(200, 0);
	pop->addObject(imgEntityOpenCloseBtn);

	imgEntityBtn = new iImage * [MAX_CHAR];
	texEntity = new Texture * [MAX_CHAR];
	for (i = 0; i < MAX_CHAR; i++)
	{
		Texture* t = createImage(pathEntity[i]);
		float ts = TILEWIDTH / t->width;
		t->width *= ts;
		t->height *= ts;
		t->potWidth *= ts;
		t->potHeight *= ts;
		texEntity[i] = t;

		img = new iImage();
		img->addObject(t);
		freeImage(t);
		img->position = iPointMake(TILEWIDTH * (i % 5), TILEHEIGHT * (i / 5));
		pop->addObject(img);
		imgEntityBtn[i] = img;
	}

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(-200, 0);
	pop->closePoint = iPointMake(0, 0);
	pop->methodAfter = drawPopEntityAfter;
	pop->methodClose = closePopEntity;
	popEntity = pop;

	imgEntityOpenCloseBtn->setTexObject(1);
}

void freePopEntity()
{
	delete popEntity;
	delete imgEntityBtn;
	delete texEntity;
}

void showPopEntity(bool show)
{
	popEntity->show(show);	if (show)
	{
		popEntity->stat = iPopupOpen;
		popEntity->bShow = true;
		popEntity->aniDt = 0.0f;
	}
	popEntity->openPoint = iPointMake(-200, 0);
	popEntity->closePoint = iPointMake(0, 0);
}

void closePopEntity(iPopup* pop)
{
	pop->openPoint = iPointMake(0, 0);
	pop->closePoint = iPointMake(-200, 0);
	pop->stat = iPopupProc;
	pop->bShow = true;
}

void drawPopEntityAfter(iPopup* pop, float dt, float rate)
{
	int i = selectedObj;
	if (i >= 100)
	{
		if (popEntity->stat == iPopupProc)
		{
			setRGBA(1, 0, 0, 0.4f);
			iPoint p = popEntity->closePoint + iPointMake(TILEWIDTH * ((i-100) % 5), TILEHEIGHT * ((i - 100) / 5));
			fillRect(p.x, p.y, TILEWIDTH, TILEHEIGHT);
			setRGBA(1, 1, 1, 1);
		}
	}

	i = popEntity->selected;
	if (i == -1)
		return;

	setRGBA(0, 0, 1, 1);
	iPoint p = popEntity->closePoint + iPointMake(TILEWIDTH * (i % 5), TILEHEIGHT * (i / 5));
	drawRect(p.x, p.y, TILEWIDTH, TILEHEIGHT);
	setRGBA(1, 1, 1, 1);
}

void drawPopEntity(float dt)
{
	popEntity->paint(dt);
}

bool entityOpenClose = true;
bool keyPopEntity(iKeyStat stat, iPoint point)
{
	if (popEntity->bShow == false)
		return false;

	int i, j = -1;

	switch (stat) {

	case iKeyStatBegan:
		if (containPoint(point, imgEntityOpenCloseBtn->touchRect(popEntity->closePoint)))
		{
			entityOpenClose = !entityOpenClose;
			imgEntityOpenCloseBtn->setTexObject(entityOpenClose);
			showPopEntity(entityOpenClose);
			return true;
		}

		i = popEntity->selected;
		if (i == -1) break;

		selectedObj = i + 100;

		return true;//break;
	case iKeyStatMoved:
		for (i = 0; i < MAX_CHAR; i++)
		{
			if (containPoint(point, imgEntityBtn[i]->touchRect(popEntity->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (popEntity->selected != j)
		{
			// audioPlay
			popEntity->selected = j;
			return true;
		}
		break;
	case iKeyStatEnded:
		break;
	}

	return false;
}

// -----------------------------------------
//  popEditorMenu
// -----------------------------------------
iPopup* popEditorMenu;
iImage** imgEditorMenuBtn;

const char* strEditorMenu[3] = { "저장하기", "불러오기", "메인메뉴" };

void drawPopEditorMenuBefore(iPopup* pop, float dt, float rate);

void createPopEditorMenu()
{
	iImage* img;
	Texture* tex;
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height);

	iPopup* pop = new iPopup();

	img = new iImage();

	g->init(size);
	setRGBA(0, 0, 0, 0.6f);
	g->fillRect(0, 0, size.width, size.height);

	setRGBA(1, 1, 1, 1);
	g->fillRect(devSize.width / 2 - 100, devSize.height / 2 - 150, 200, 300, 20);

	tex = g->getTexture();
	img->addObject(tex);
	img->position = iPointZero;
	pop->addObject(img);
	freeImage(tex);

	imgEditorMenuBtn = new iImage * [3];
	size = iSizeMake(100, 40);
	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)	setRGBA(1, 1, 1, 1);
			else		setRGBA(0, 0, 0, 1);
			g->fillRect(0, 0, size.width, size.height, 15);

			setRGBA(0, 0, 0, 1);
			g->drawRect(0, 0, size.width, size.height, 15);
			setRGBA(1, 1, 1, 1);

			setStringSize(20);
			if (j == 0)	setStringRGBA(0, 0, 0, 1);
			else		setStringRGBA(1, 1, 1, 1);
			setStringBorder(1);
			setStringBorderRGBA(0, 0, 0, 1);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strEditorMenu[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width / 2 - 50, 270 + 70 * i);
		pop->addObject(img);
		imgEditorMenuBtn[i] = img;
	}

	pop->methodBefore = drawPopEditorMenuBefore;
	popEditorMenu = pop;
}

void freePopEditorMenu()
{
	delete popEditorMenu;
	delete imgEditorMenuBtn;
}

void showPopEditorMenu(bool show)
{
	popEditorMenu->show(show);
}


void drawPopEditorMenuBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 3; i++)
		imgEditorMenuBtn[i]->setTexObject(i == popEditorMenu->selected);
}

void drawPopEditorMenu(float dt)
{
	popEditorMenu->paint(dt);
}

int tmW = 0;
bool keyPopEditorMenu(iKeyStat stat, iPoint point)
{
	if (popEditorMenu->bShow == false)
		return false;

	int i, j = -1;

	switch (stat) {
		
	case iKeyStatBegan:
		if (popEditorMenu->selected == -1)
			break;
		
		if (popEditorMenu->selected == 0)
		{
			// save
			for (int j = 0; j < tmY; j++)
			{
				for (int i = 0; i < tmX; i++)
				{
					int idx = tmX * j + i;
					Tile* t = &tm[idx];

					if (t->value == -1) continue;

					int x = idx % tmX;
					if (x >= tmW)
						tmW = x + 1;
				}
			}
			saveStageData();
		}
		else if (popEditorMenu->selected == 1)
		{
			// load
			stageDataPath = (char*)openFileDlg(true, L"dat Files(*.dat)\0*.dat;.DAT\0", L"\\assets\\stagedata");
		}
		else
		{
			setLoading(gs_intro, freeStageEditor, loadIntro);
			setStringName(strName); // #to do...
		}
		break;
				
	case iKeyStatMoved:
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(point, imgEditorMenuBtn[i]->touchRect(popEditorMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popEditorMenu->selected != j)
		{
			// aduio
			popEditorMenu->selected = j;
		}
		break;

	case iKeyStatEnded:

		break;
	}

	return true;
}

void saveStageData()
{
	const char* path = openFileDlg(false, L"dat Files(*.dat)\0*.dat;.DAT\0", L"\\assets\\stagedata");
	if (path == NULL)
		return;
	
	char strPath[1024];
	int l = strlen(path);


	if (strcmp(&path[l - 4], ".dat") == 0 ||
		strcmp(&path[l - 4], ".DAT") == 0)
		strcpy(strPath, path);
	else
	{
		sprintf(strPath, "%s.dat\0", path);
	}

	FILE* pf = fopen(strPath, "wb");

	fwrite(&tNum, sizeof(int), 1, pf);
	fwrite(&tmX, sizeof(int), 1, pf);
	fwrite(&tmY, sizeof(int), 1, pf);
	fwrite(&tmW, sizeof(int), 1, pf);

	for (int i = 0; i < tNum; i++)
	{
		Tile* t = &tm[i];

		fwrite(t, sizeof(Tile), 1, pf);
	}

	fclose(pf);
}

void loadStageData(char* stageDataPath, loadData* ld)
{
	FILE* pf = fopen(stageDataPath, "rb");
	
	fread(&ld->tNum, sizeof(int), 1, pf);
	fread(&ld->tmX, sizeof(int), 1, pf);
	fread(&ld->tmY, sizeof(int), 1, pf);
	fread(&ld->tmW, sizeof(int), 1, pf);

	ld->tm = new Tile[ld->tNum];

	for (int i = 0; i < ld->tNum; i++)
	{
		fread(&ld->tm[i], sizeof(Tile), 1, pf);
	}
	
	fclose(pf);
}


Tile* selectedTilePrev = NULL;
float selectedTileAlpha = 0.0f;
iPoint selectedTilePosition;
float updateEntityScale(float dt)
{
	dt *= 2;

	if (selectedField == -1)
	{
		selectedTileAlpha -= dt;
		if (selectedTileAlpha < 0.0f)
			selectedTileAlpha = 0.0f;
		return selectedTileAlpha;
	}
	Tile* t = &tm[selectedField];
	if (t->value % THEME_INTERVAL < 100)
	{
		selectedTileAlpha -= dt;
		if (selectedTileAlpha < 0.0f)
			selectedTileAlpha = 0.0f;
		return selectedTileAlpha;
	}

	selectedTilePrev = t;
	selectedTileAlpha += dt;
	if (selectedTileAlpha > 1.0f)
		selectedTileAlpha = 1.0f;

	int x = (selectedField % tmX) * TILEWIDTH;
	int y = (selectedField / tmX) * TILEHEIGHT;

	selectedTilePosition = iPointMake(x + 25 - 75 + offField.x, y + TILEHEIGHT + 10 + offField.y);

	return selectedTileAlpha;
}

void drawEntityScale(float dt)
{
	float a = updateEntityScale(dt);
	if (a == 0.0f)
		return;

	setRGBA(0, 0, 0, a);

	iPoint p = selectedTilePosition;//iPointMake(50, devSize.height - 70);
	float dx = 150;
	int edge = 10;
	fillRect(p.x - edge, p.y, dx + edge * 2, 6, 3); // p.y + 3
	Tile* t = selectedTilePrev;
	float x = p.x + dx * t->level / 4.;// 50 ~ 200
	fillRect(x - 3, p.y + 3 - 10, 6, 20);

	if (keyDown & keysLeft)
	{
		t->level--;
		if (t->level < 0)
			t->level = 0;
	}
	else if (keyDown & keysRight)
	{
		t->level++;
		if (t->level > 4)
			t->level = 4;
	}

}






