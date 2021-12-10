//#include "App.h"

#include "Game.h"
#include "SpriteTool.h"

void loadGame()
{

}

void drawGame(float dt)
{
	// 윈도우에서 선택한 거 그림
	drawSprite(dt);
}

void freeGame()
{
	freeSpriteTool();
}


void keyGame(iKeyStat stat, iPoint point, bool leftRight)
{
	keySpriteTool(stat, point);
}




