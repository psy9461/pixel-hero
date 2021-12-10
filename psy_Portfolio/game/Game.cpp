//#include "App.h"

#include "Game.h"

#include "Loading.h"

#include "Intro.h"

#include "Common.h"
#include "Stage1A.h"
#include "Entity.h"
#include "Tilemap.h"

//#define TEST
#include "StageEditor.h"

void loadGame()
{
	AudioInfo ai[5] = {
		{"assets/soundtrack/btn.wav", false, 0.5f},
		{"assets/soundtrack/introBGM.wav", true, 0.3f},
		{"assets/soundtrack/Stage1BGM.wav", true, 0.3f},
		{"assets/soundtrack/gameover.wav", false, 0.5f},
		{"assets/soundtrack/clear.wav", false, 0.5f}
	};
	loadAudio(ai, 5);
	audioVolume(0.3f, 0.3f, 1);
	//audioPlay(2);

	setStringNameEng;


#if 0
	gameState = gs_stageedit;
	loadStageEditor();
	return;
#endif
	loadCommon();

	loadIntro();
	gameState = gs_intro;
}

#ifdef TEST
enum HeroBehave
{
	HeroBehaveWait = 0,

};

class THero
{
public:
	THero();
	virtual ~THero();

	void paint(float dt);

public:
	iSize size;

	iPoint position;
	float jumpDt;
	int jumpNum;
};


#define hero_move 200   // 이동속도
#define hero_bottom 300   // 주인공 떨어지는 바닥
#define hero_jumpDt 0.3f
#define hero_fall 200   // 떨어지는 속도
#define hero_jumping 100 + hero_jumpDt * hero_fall // 점프량

THero::THero()
{
	size = iSizeMake(40, 60);
	position = iPointMake(devSize.width / 2, hero_bottom);

	jumpDt = hero_jumpDt;
	jumpNum = 0;
}
THero::~THero()
{

}

void THero::paint(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	setRGBA(1, 1, 1, 1);
	drawLine(0, hero_bottom, devSize.width, hero_bottom);

	setRGBA(1, 0, 1, 1);
	fillRect(position.x - size.width / 2,
		position.y - size.height,
		size.width, size.height);
	setRGBA(1, 1, 1, 1);


	if (keyStat & keysLeft)
		position.x -= hero_move * dt;
	else if (keyStat & keysRight)
		position.x += hero_move * dt;

	if (keyDown & keysSpace)
	{
		if (jumpNum < 2)// 더블점프까지
		{
			jumpDt = 0.0f;
			jumpNum++;
		}
	}

	if (jumpDt < hero_jumpDt)
	{
		float jumpPrev = jumpDt;
		jumpDt += dt;
		if (jumpDt > hero_jumpDt)
			jumpDt = hero_jumpDt;

		position.y -= linear(jumpDt / hero_jumpDt, 0, hero_jumping)
			- linear(jumpPrev / hero_jumpDt, 0, hero_jumping);
	}

	if (position.y < hero_bottom)
	{
		position.y += hero_fall * dt;
		if (position.y >= hero_bottom)
		{
			position.y = hero_bottom;

			jumpNum = 0;
			jumpDt = hero_jumpDt;
		}
	}
}
#endif

void drawGame(float dt)
{
	// drawLine +
	// be, frame, p, tex, rtAtt, rtDef
	switch (gameState)
	{
		case gs_intro:		drawIntro(dt);	break;
		case gs_stage1A:	drawStage1A(dt);	break;
		case gs_stageedit:  drawStageEditor(dt); break;
	}
	drawPopExit(dt);

	drawLoading(dt);
#ifdef TEST
	static THero* hero = new THero();
	hero->paint(dt);
#endif
}

void freeGame()
{
	switch (gameState) {
	case gs_intro:		freeIntro();		break;
	case gs_stage1A:	freeStage1A();		break;
	case gs_stageedit:	freeStageEditor();	break;
	//case gs_end:		freeHero();		break;
	}
	freePopExit();
	freeCommon();
}


void keyGame(iKeyStat stat, iPoint point, bool leftRight)
{
	if (popExit)
	{
		if (keyPopExit(stat, point))
			return;
	}

	switch (gameState) {
	case gs_intro:		keyIntro(stat, point);			break;
	case gs_stage1A:	keyStage1A(stat, point);		break;
	case gs_stageedit:	keyStageEditor(stat, point, leftRight);	break;
	}
}




