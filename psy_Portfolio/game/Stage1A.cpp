#include "Stage1A.h"

#include "Entity.h"

#include "TileMap.h"

#include "Loading.h"
#include "Intro.h"
//#include "ProcData.h"
//#define TEST_SORT

iStrTex* strGameover;
float gameoverDt;
float alpha;
float strAlpha;

bool gameover;
bool clear;

Texture* msg;
void loadStage1A()
{
#if 0
	freeEntpool();
	entPool = NULL;
	freeProjpool();
	projPool = NULL;
	loadEntpool();
	loadProjpool();
#endif

	if (hero)
	{
		delete hero;
		hero = new Hero();
	}


	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(900, 100);
	g->init(size);
	const char* strNameBefore = getStringName();
	setStringNameKor;
	setStringRGBA(1, 1, 1, 1);
	setStringSize(50);
	setStringBorder(3);
	setStringBorderRGBA(0, 0, 0, 1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "필드에 남은 몬스터를 모두 처치하세요");

	msg = g->getTexture();

	setStringName(strNameBefore);
	audioPlay(2);
	setCommonMethodLoadFree(loadStage1A, freeStage1A);
	loadMap(1, "A");
	loadEnemy();
	showPopStatus(true);

	
	strGameover = new iStrTex(NULL);

	gameoverDt = 0.0f;
	alpha = 0.0f;
	strAlpha = 0.0f;
	gameover = false;
	clear = false;
}



float msgDt;
void drawStage1A(float dt)
{
	float _dt = dt;
	if (toggleCommonMenu)
		dt = 0;

	iPoint off = drawMap();

	if (hero->hp <= 0)
	{
		audioStop();
		alpha += dt / 2;
		if (alpha >= 1)
			alpha = 1;
		setRGBA(0, 0, 0, alpha);
		fillRect(0, 0, devSize.width, devSize.height);
		setRGBA(1, 1, 1, 1);

		if (alpha == 1)
		{
			gameoverDt += dt;
			if (gameoverDt >= 1)
			{
				if (gameover == false)
				{
					audioPlay(3);
					freeEntpool();
					entPool = NULL;
					freeProjpool();
					projPool = NULL;
					gameover = true;
				}

				setStringNameEng;
				setStringLineHeight(100);
				setStringRGBA(1, 0, 0, 1);
				setStringSize(80);
				setStringBorder(5);
				setStringBorderRGBA(0, 0, 1, 1);
				strGameover->paint(devSize.width / 2, devSize.height / 2, VCENTER | HCENTER,
					"Game Over");
			}

			if (gameoverDt >= 5.0f)
			{
				gameoverDt = 5.0f;
				setStringLineHeight(strLineHeight);
				setLoading(gs_intro, freeStage1A, loadIntro);
			}
		}

		return;
	}

	drawEnemy(dt, off);
	drawHero(dt, off);

#if 0
	fbo->bind(texMinimap);
	setRGBA(0, 1, 0, 1);
	fillRect((hero->pos.x - hero->rtCol.size.width / 2) / 10.f, (hero->pos.y - hero->rtCol.size.height) / 10.0f,
		hero->rtCol.size.width / 10.0f, hero->rtCol.size.height / 10.0f);
	setRGBA(1, 0, 0, 1);
	for (int i = 0; i < entNum; i++)
	{
		Enemy* e = (Enemy*)entity[i];
	
		fillRect((e->pos.x - e->rtCol.size.width / 2) / 10.f, (e->pos.y - e->rtCol.size.height) / 10.0f,
			e->rtCol.size.width / 10.0f, e->rtCol.size.height / 10.0f);
	}
	setRGBA(1, 1, 1, 1);
	fbo->unbind();
	
	Texture* t = texMinimap;
	setGLBlend(GLBlendMultiplied);
	drawImage(t, devSize.width, devSize.height, BOTTOM | RIGHT, 0, 0, t->width, t->height, 1.0f, 1.0f, 2, 0, REVERSE_HEIGHT);
	setGLBlend(GLBlendAlpha);
#endif

	off += hero->pos;

	drawPopStatus(dt);
	drawPopCommonMenu(_dt);
	drawPopOptionProc(_dt);

	if (entNum == 0)
	{
		audioStop();
		alpha += dt / 2;
		if (alpha >= 1)
			alpha = 1;
		setRGBA(0, 0, 0, alpha);
		fillRect(0, 0, devSize.width, devSize.height);
		setRGBA(1, 1, 1, 1);

		if (alpha == 1)
		{

			for (int i = 0; i < entNum; i++)
			{
				Enemy* e = (Enemy*)entity[i];
				e->be = EnemyBehaveIdle;
				e->alive = false;
			}
			entNum = 0;

			gameoverDt += dt;
			if (gameoverDt >= 1)
			{
				if (clear == false)
				{
					audioPlay(4);
					freeEntpool();
					entPool = NULL;
					freeProjpool();
					projPool = NULL;
					clear = true;
				}

				setStringNameEng;
				setStringLineHeight(100);
				setStringRGBA(0, 1, 0, 1);
				setStringSize(70);
				setStringBorder(1);
				setStringBorderRGBA(0, 1, 0, 1);
				strGameover->paint(devSize.width / 2, devSize.height / 2, VCENTER | HCENTER,
					"Clear~!");
			}

			if (gameoverDt >= 5.0f)
			{
				gameoverDt = 5.0f;
				setStringLineHeight(strLineHeight);
				setLoading(gs_intro, freeStage1A, loadIntro);
			}
		}

		return;
	}
	else if (boss->hp <= 0)
	{
		msgDt += dt;
		if (msgDt < 2.0f) return;

		float a = _sin(180 * msgDt / 2);
		if (a < 0)
			a *= -1;
		setRGBA(1, 1, 1, a);
		drawImage(msg, devSize.width / 2, 100, VCENTER | HCENTER);
		setRGBA(1, 1, 1, 1);
	}



	if (keyDown & keysESC)
	{
		if (popExit)
		{
			if (popExit->bShow == true) return;
		}
		else if (popOptionProc->bShow) return;

		showPopCommonMenu(toggleCommonMenu = !toggleCommonMenu);
	}
}
void freeStage1A()
{
	freeMap();
	freeEnemy();
	delete strGameover;
}


void keyStage1A(iKeyStat stat, iPoint point)
{
	if (keyPopOptionProc(stat, point)||
		keyPopCommonMenu(stat, point))
		return;
}


#ifdef TEST_SORT
iSort* 
;
Entity** ent;

sort = new iSort();
ent = new Entity * [100];

sort->init();

ent[0] = hero;
sort->add(hero->pos.y);
for (int i = 0; i < enemyNum; i++)
{
	sort->add(enemy[i]->pos.y);
	ent[1 + i] = enemy[i];
}

sort->update();

for (int i = 0; i < sort->sdNum; i++)
	ent[sort->get(i)]->paint(dt);

delete sort;
delete ent;

#endif

