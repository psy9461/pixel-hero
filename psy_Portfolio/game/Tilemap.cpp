#include "Tilemap.h"

#include "Entity.h"

#include "iStd.h"
#include "ProcData.h"

float tileScale;
float scaleBgX;
float scaleBgY;
Tile* tileMap;
int tileW = TILEWIDTH, tileH = TILEHEIGHT;
int tileX, tileY, maxTileX, tileNum;
iPoint sp;


// ---------------------------------
//   Map
// ---------------------------------

Texture* texBg;
iSize sizeMap;

Texture* texMinimap;
void genEnemy(int kind, int level, int x, int y);

void loadMap(int level, const char* stageIndex)
{
	char stagePath[256];
	sprintf(stagePath, "assets/stagedata/stage%d%s.dat", level, stageIndex);
	FILE* pf = fopen(stagePath, "rb");

	fread(&tileNum, sizeof(int), 1, pf);
	fread(&tileX, sizeof(int), 1, pf);
	fread(&tileY, sizeof(int), 1, pf);
	fread(&maxTileX, sizeof(int), 1, pf);

	tileMap = new Tile[tileNum];
	fread(tileMap, sizeof(Tile), tileNum, pf);

	for (int i = 0; i < tileNum; i++)
	{
		int value = tileMap[i].value % THEME_INTERVAL;
		if (value > 99)// && value < 200)
		{
			int x = (i % tileX) * TILEWIDTH;
			int y = (i / tileX) * TILEHEIGHT;
			if (value == 100)
			{
				sp = iPointMake(x + TILEWIDTH/2, y + TILEHEIGHT - 1);
				hero->pos = sp;
			}
			else
			{
				int enemyLevel = tileMap[i].level;
				int kind = value % 100 -1;
				genEnemy(kind, enemyLevel, x, y);
			}
		}
	}

	sizeMap = iSizeMake(TILEWIDTH * maxTileX, TILEHEIGHT * tileY);
	tileScale = TILEWIDTH / tileImgs[0][0]->width;

	texBg = createImage(bgImgPath[level - 1]);
	scaleBgX = sizeMap.width / texBg->width;
	scaleBgY = sizeMap.height / texBg->height;

	texMinimap = createTexture(sizeMap.width / 10, sizeMap.height / 10);
}

void freeMap()
{
	freeImage(texBg);
	delete tileMap;
	freeImage(texMinimap);
}

iPoint drawMap()
{
	int i, j;

#if 0
	fbo->bind(texMinimap);
	glClearColor(0, 0, 0, 0.6f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fbo->unbind();
#endif

	iPoint off = iPointMake(devSize.width/2, devSize.height/2) - hero->pos;
	if (off.x > 0)
		off.x = 0;
	else if (off.x < devSize.width - sizeMap.width)
		off.x = devSize.width - sizeMap.width;
	if (off.y > 0)
		off.y = 0;
	else if (off.y < devSize.height - sizeMap.height)
		off.y = devSize.height - sizeMap.height;

	setRGBA(1, 1, 1, 1);
	clearRect();
	drawImage(texBg, off.x, off.y, TOP | LEFT, 0, 0, texBg->width, texBg->height, scaleBgX, scaleBgY, 2, 0);

	for (i = 0; i < tileY; i++)
	{
		for (j = 0; j < tileX; j++)
		{
			int value = tileMap[tileX * i + j].value % THEME_INTERVAL;
			int theme = tileMap[tileX * i + j].value / THEME_INTERVAL;
			if (value < 0 || (value > 99 && value < 200)) continue;

			// 타일 크기에 맞게 drawImage
			Texture* tex = tileImgs[theme][value];

			drawImage(tex, tileW * j + off.x, tileH * i + off.y, TOP | LEFT,
				0, 0, tex->width, tex->height, tileScale, tileScale, 2, 0);
		}
	}
#if 0
	fbo->bind(texMinimap);
	for (i = 0; i < tileY; i++)
	{
		for (j = 0; j < tileX; j++)
		{
			int value = tileMap[tileX * i + j].value % THEME_INTERVAL;
			int theme = tileMap[tileX * i + j].value / THEME_INTERVAL;
			if (value < 0 || (value > 99 && value < 200)) continue;
	
			// 타일 크기에 맞게 drawImage
			Texture* tex = tileImgs[theme][value];
			drawImage(tex, TILEWIDTH * j / 10.f, TILEHEIGHT * i / 10.f, TOP | LEFT, 0, 0, tex->width, tex->height,
				0.1 * tileScale, 0.1 * tileScale, 2, 0);
		}
	}
	fbo->unbind();
#endif
	return off;
}

// 풀에서 필드로 끌어오는 함수
Enemy* boss;
void genEnemy(int kind, int level, int x, int y)
{
	for (int i = 0; i < 40; i++)
	{
		Enemy* e = (Enemy*)entPool[kind][i];

		if (kind < 4)
		{
			if (e->alive == false)
			{
				e->alive = true;
				e->hp = e->maxHp;
				e->be = EnemyBehaveIdle;
				e->pos = iPointMake(x + TILEWIDTH / 2, y + TILEHEIGHT - 1);
				e->scale = (float)level / 2.0f;

				entity[entNum] = e;
				entNum++;
				if (kind == 3)
					boss = e;
				break;
			}
		}
		else //if --> 다른 몬스터 추가 시 #to do...
		{
			return;
		}
	}
}



