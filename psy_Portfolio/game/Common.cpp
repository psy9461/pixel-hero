#include "Common.h"

#include "Entity.h" 
#include "Loading.h"

#include "Intro.h"
#include "ProcData.h"
#include "iSort.h"
#include "Tilemap.h"
#include "Entity.h"

Entity*** entPool;
Entity** entity;
int entNum;

Projectile*** projPool;
Projectile** projectile;
int projNum;


Hero* hero;
//iSort* sort;
Texture*** tileImgs;

void loadEntpool()
{
    entPool = new Entity * *[MAX_CHAR - 1];
    for (int i = 0; i < MAX_CHAR - 1; i++)
    {
        entPool[i] = new Entity * [40];
        memset(entPool[i], 0x00, sizeof(Entity*) * 40);
        for (int j = 0; j < 40; j++)
        {
            if (i < 4)
                entPool[i][j] = new Walkable(i);
            else
                break;
        }
    }

    entity = new Entity * [200];
    entNum = 0;
}

void freeEntpool()
{
    for (int i = 0; i < MAX_CHAR - 1; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (i < 4)
            {
                if (entPool[i][j])
                    delete entPool[i][j];
            }
            else
                break;
        }   
        delete entPool[i];
    }
    delete entPool;

    delete entity;
}

void loadProjpool()
{
    projPool = new Projectile * *[MAX_PROJ];
    for (int i = 0; i < MAX_PROJ; i++)
    {
        projPool[i] = new Projectile * [50];
        memset(projPool[i], 0x00, sizeof(Projectile*) * 50);
        for (int j = 0; j < 50; j++)
        {
            if (i == 0)
                projPool[i][j] = new Projectile(i);
            else
                break;
        }
    }

    projectile = new Projectile * [50];
}

void freeProjpool()
{
    for (int i = 0; i < MAX_PROJ; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (i != 0) break;
            if (projPool[i][j])
                delete projPool[i][j];
        }
        delete projPool[i];
    }
    delete projPool;

    delete projectile;
}
void loadCommon()
{
    loadEntpool();
    loadProjpool();

    tileImgs = new Texture * *[KIND_TILE];
    for (int i = 0; i < KIND_TILE; i++)
    {
        TileData td = _tileData[i];
        int cnt = td.tileCnt;
        tileImgs[i] = new Texture * [cnt];
        for (int j = 0; j < cnt; j++)
        {
            tileImgs[i][j] = createImage("assets/tiles/%s/%d.png", td.path, j);
        }
    }


	//sort = new iSort(100);
	createPopCommonMenu();
    createPopOptionProc();
    createPopStatus();


	loadHero();
	
	setTexture(CLAMP, NEAREST);
}

void freeCommon()
{
    freeEntpool();
    freeProjpool();

    for (int i = 0; i < KIND_TILE; i++)
    {
        for (int j = 0; j < _tileData[i].tileCnt; j++)
        {
            freeImage(tileImgs[i][j]);
        }
        delete tileImgs[i];
    }
    delete tileImgs;

	//delete sort;
	freePopCommonMenu();
    freePopOptionProc();
    freePopStatus();
    freeSpriteChar();
    freeSpriteProj();
	freeHero();
}

// -----------------------------------------
// popCommonMenu
// -----------------------------------------
iPopup* popCommonMenu = NULL;
iImage** imgCommonMenuBtn;

const char* strCommonMenu[5] = { "돌아가기", "다시하기", "설정", "메인화면", "게임종료"};
bool toggleCommonMenu = false;

void drawPopCommonMenuBefore(iPopup* pop, float dt, float rate);

MethodLoad methodCommonMenuLoad = NULL;
MethodLoad methodCommonMenuFree = NULL;

void setCommonMethodLoadFree(void* mLoad, void* mFree)
{
	methodCommonMenuLoad = (MethodLoad)mLoad;
	methodCommonMenuFree = (MethodLoad)mFree;
}

void createPopCommonMenu()
{
//	methodCommonMenuFree = (MethodLoad)mFree;
//
	if (popCommonMenu != NULL)
		return;

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
	g->fillRect(devSize.width/2 - 100, devSize.height/2 - 200, 200, 400, 20);

	tex = g->getTexture();
	img->addObject(tex);
	img->position = iPointZero;
	pop->addObject(img);
	freeImage(tex);

	imgCommonMenuBtn = new iImage * [5];
	size = iSizeMake(100, 40);
	for (int i = 0; i < 5; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)	setRGBA(1, 1, 1, 1);
			else		setRGBA(0, 0, 0, 1);
			g->fillRect(0, 0, size.width, size.height, 15);

			setRGBA(0, 0, 0, 1);
            setLineWidth(3);
			g->drawRect(0, 0, size.width, size.height, 15);
			setRGBA(1, 1, 1, 1);

			setStringSize(20);
			if (j == 0)	setStringRGBA(0, 0, 0, 1);
			else		setStringRGBA(1, 1, 1, 1);
            setStringNameKor;
			setStringBorder(1);
			setStringBorderRGBA(0, 0, 0, 1);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strCommonMenu[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width/2 - 50, 202 + 70 * i);
		pop->addObject(img);
		imgCommonMenuBtn[i] = img;
	}

	pop->methodBefore = drawPopCommonMenuBefore;
	popCommonMenu = pop;
}

void freePopCommonMenu()
{
	delete popCommonMenu;
	popCommonMenu = NULL;
	delete imgCommonMenuBtn;
}

void showPopCommonMenu(bool show)
{
	popCommonMenu->show(show);
}

void drawPopCommonMenuBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 5; i++)
		imgCommonMenuBtn[i]->setTexObject(i == popCommonMenu->selected);
}

void drawPopCommonMenu(float dt)
{
	popCommonMenu->paint(dt);
}

bool keyPopCommonMenu(iKeyStat stat, iPoint point)
{
	if (popCommonMenu->bShow == false)
		return false;

	int i, j = -1;

	switch (stat) {

	case iKeyStatBegan:
		if (popCommonMenu->selected == -1)
			break;

		if (popCommonMenu->selected == 0)
		{
			showPopCommonMenu(false);
			toggleCommonMenu = false;
		}
		else if (popCommonMenu->selected == 1)
		{

			setLoading(gs_stage1A, methodCommonMenuFree, methodCommonMenuLoad);
		}
		else if (popCommonMenu->selected == 2)
		{
			// 설정창
            showPopCommonMenu(false);
            showPopOptionProc(true);
		}
		else if (popCommonMenu->selected == 3)
		{
			// 메인메뉴로 돌아가겠냐는 확인 팝업 추가
            toggleCommonMenu = false;
			setLoading(gs_intro, methodCommonMenuFree, loadIntro);
		}
		else
		{
			showPopExit(true);
		}

	case iKeyStatMoved:
		for (int i = 0; i < 5; i++)
		{
			if (containPoint(point, imgCommonMenuBtn[i]->touchRect(popCommonMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popCommonMenu->selected != j)
		{
			// aduio
			popCommonMenu->selected = j;
		}
		break;

	case iKeyStatEnded:

		break;
	}

	return true;
}


// ===================================
// popOption
// ===================================
#include "App.h"

iPopup* popOptionProc;
iImage** imgOptionProcBtn;
iImage** imgVolumeProc;

int volumeForProc;
void drawpopOptionProcBefore(iPopup* pop, float dt, float rate);
void methodPopCommonClose(iPopup* pop);

void createPopOptionProc()
{
    iImage* img;
    Texture* tex;
    int i, j;

    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(480, 320);
    g->init(size);

    setRGBA(1, 1, 1, 0.7f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(40);
    //setStringRGBA(0, 0, 0, 1);
    setStringBorder(2);
    //setStringBorderRGBA(1, 0, 1, 1);
    setStringNameKor;
    setStringYellow;
    setStringBorderBrown;
    g->drawString(size.width / 2, 50, VCENTER | HCENTER, "옵션");
    setStringSize(30);
    g->drawString(130, 100, TOP | RIGHT, "음량");
    g->drawString(130, 170, TOP | RIGHT, "화면크기");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    const char* strBtn[5] = {
        "-", "+", "전체화면", "창모드", "나가기"
    };

    imgOptionProcBtn = new iImage * [5];
    for (i = 0; i < 5; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            if (i < 2)
            {
                size = iSizeMake(25, 25);
                g->init(size);

                if (j == 0) setRGBA(0, 0, 1, 1);
                else        setRGBA(0, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 5);
                setRGBA(1, 1, 1, 1);

                setStringSize(20);
                setStringRGBA(1, 1, 1, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 0, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
            else if (i < 4)
            {
                size = iSizeMake(100, 50);
                g->init(size);

                if (j == 0) setRGBA(1, 1, 1, 1);
                else        setRGBA(0, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);

                setStringSize(25);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 0, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
            else
            {
                size = iSizeMake(80, 50);
                g->init(size);

                if (j == 0) setRGBA(0, 0, 1, 1);
                else        setRGBA(0, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);

                setStringSize(25);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 0, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
        }

        if (i < 2)
            img->position = iPointMake(160 + 260 * i, 100);
        else if (i < 4)
            img->position = iPointMake(150 + 150 * (i - 2), 160);
        else
            img->position = iPointMake(200, 240);
        pop->addObject(img);

        imgOptionProcBtn[i] = img;
    }

    imgVolumeProc = new iImage * [10];
    for (int i = 0; i < 10; i++)
    {
        img = new iImage();
        for (int j = 0; j < 2; j++)
        {
            size = iSizeMake(10, 40 * (float)(i + 1) / 7);
            g->init(size);

            if (j == 0) setRGBA(1, 1, 1, 1);
            else        setRGBA(0, 0, 1, 1);
            g->fillRect(0, 0, size.width, size.height, 2);
            setRGBA(1, 1, 1, 1);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = iPointMake(200 + 20 * i, 100 + (40 - size.height));
        pop->addObject(img);

        imgVolumeProc[i] = img;
    }

    pop->style = iPopupRotate;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 480) / 2, (devSize.height - 320) / 2);
    pop->methodBefore = drawpopOptionProcBefore;
    pop->methodClose = methodPopCommonClose;
    popOptionProc = pop;

    volumeForProc = 3;
    setStringNameEng;
}
void freePopOptionProc()
{
    delete popOptionProc;
    delete imgOptionProcBtn;
}

void drawpopOptionProcBefore(iPopup* pop, float dt, float rate)
{
    setRGBA(0, 0, 0, 0.7f * rate);
    fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);

    for (int i = 0; i < 5; i++)
    {
        imgOptionProcBtn[i]->setTexObject(i == popOptionProc->selected);
    }

    for (int i = 0; i < 10; i++)
    {
        if (i < volumeForProc)
            imgVolumeProc[i]->setTexObject(1);
        else
            imgVolumeProc[i]->setTexObject(0);
    }

    if (isFullscreen)
    {
        imgOptionProcBtn[2]->setTexObject(1);
    }
    else
    {
        imgOptionProcBtn[3]->setTexObject(1);
    }
}

void drawPopOptionProc(float dt)
{
    popOptionProc->paint(dt);
}

void methodPopCommonClose(iPopup* pop)
{
    showPopCommonMenu(true);
}

void showPopOptionProc(bool show)
{
    popOptionProc->show(show);
    if (show)
    {

    }
}

bool keyPopOptionProc(iKeyStat stat, iPoint point)
{
    if (popOptionProc->bShow == false)
        return false;
    if (popOptionProc->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popOptionProc->selected == -1)
            break;

        if (popOptionProc->selected == 0)
        {
            // 사운드 줄이기
            volumeForProc--;
            if (volumeForProc < 0)
                volumeForProc == 0;
            audioVolume(0.1f * volumeForProc, 0.1f * volumeForProc, 1);
        }
        else if (popOptionProc->selected == 1)
        {
            // 사운드 올리기
            volumeForProc++;
            if (volumeForProc > 10)
                volumeForProc == 10;
            audioVolume(0.1f * volumeForProc, 0.1f * volumeForProc, 1);
        }
        else if (popOptionProc->selected == 2)
        {
            // 전체화면
            goFullscreen(true);
        }
        else if (popOptionProc->selected == 3)
        {
            // 창모드
            goFullscreen(false);
        }
        else if (popOptionProc->selected == 4)
        {
            // 볼륨 크기
            showPopOptionProc(false);
        }
        else if (popOptionProc->selected > 4 && popOptionProc->selected < 15)
        {

        }

        break;
    case iKeyStatMoved:
        for (i = 0; i < 5; i++)
        {
            if (containPoint(point, imgOptionProcBtn[i]->touchRect(popOptionProc->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popOptionProc->selected != j)
        {
            audioPlay(0);
            popOptionProc->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;

    }
    return true;

}



// ===================================
// popStatus
// ===================================
iPopup* popStatus;
iImage** imgPopstatus;
Texture* texHpBar;
iStrTex* stHp;
#define hpMax 100.0f
#define hpDelta 40.0f
void drawPopStatusAfter(iPopup* pop, float dt, float rate);

float strLineHeight;
float _strLineHeight;
void createPopStatus()
{
    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();

    imgPopstatus = new iImage * [2];
    
    for (int i = 0; i < 2; i++)
    {
        igImage* igImg1;
        igImage* igImg2;
        iImage* img = new iImage();
        if (i == 0)
        {
            iSize size = iSizeMake(100, 100);
            g->init(size);
            igImg1 = g->createIgImage("assets/UI/frames/Frame2.png");
            float scaleX = size.width / igImg1->GetWidth();
            float scaleY = size.height / igImg1->GetHeight();
            g->drawIgImage(igImg1, 0, 0, TOP | LEFT, 0, 0, igImg1->GetWidth(), igImg1->GetHeight(),
                scaleX, scaleY, 2, 0);
            igImg2 = g->createIgImage("assets/adventurer-profile.png");
            g->drawIgImage(igImg2, size.width / 2, size.height - 25, BOTTOM | HCENTER, 0, 0, igImg1->GetWidth(), igImg1->GetHeight(),
                2.0, 2.0, 2, 0);

            Texture* tex = g->getTexture();

            img->addObject(tex);
            freeImage(tex);
            img->position = iPointZero;
        }
        else
        {
            iSize size = iSizeMake(200, 25);
            g->init(size);
            setRGBA(0.4, 0.15, 0.13, 1);
            setRGBA(0.4, 0.1, 0.1, 1);
            setLineWidth(3);
            g->drawRect(0, 0, size.width, size.height, 10);
            g->fillRect(0, 0, size.width, size.height, 10);
            Texture* tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
            img->position = iPointMake(110, 37);

            setRGBA(1, 0, 0, 1);
            g->init(size);
            g->fillRect(0, 0, size.width, size.height, 10);
            texHpBar = g->getTexture();
            setRGBA(1, 1, 1, 1);
        }
        pop->addObject(img);

        imgPopstatus[i] = img;
    }
    
    pop->openPoint = iPointMake(20, 600);
    pop->closePoint = iPointMake(20, 600);
    pop->methodAfter = drawPopStatusAfter;
    popStatus = pop;

    stHp = new iStrTex(NULL);

    _strLineHeight = getStringLineHeight();
}

void freePopStatus()
{
    delete imgPopstatus;
    delete popStatus;
    freeImage(texHpBar);
    delete stHp;
}


void drawPopStatusAfter(iPopup* pop, float dt, float rate)
{
    //
    float hpRate = (float)hero->hp/(float)hero->maxHp;
    float hpWidth = 200 * hpRate;
    
    texHpBar->width -= hpDelta * dt;
    if (texHpBar->width < hpWidth)
        texHpBar->width = hpWidth;
    if (texHpBar->width < 0)
        texHpBar->width = 0;
    //drawImage(texHpBar, 24, 24, TOP | LEFT, 0, 0,
    drawImage(texHpBar, popStatus->closePoint.x + 110, popStatus->closePoint.y + 37, TOP | LEFT, 0, 0,
        texHpBar->width, texHpBar->height,
        1, 1, 0, 0);

    strLineHeight = getStringLineHeight();
    if (strLineHeight != _strLineHeight)
        setStringLineHeight(_strLineHeight);
    setStringNameEng;
    setStringRGBA(1, 1, 1, 1);
    setStringSize(20);
    setStringBorderRGBA(1, 1, 1, 1);
    setStringBorder(0.5);
    stHp->paint(popStatus->closePoint.x + 110 + 100, popStatus->closePoint.y + 37 + 35, BOTTOM | HCENTER, "%d / %d", hero->hp, hero->maxHp);
}

void drawPopStatus(float dt)
{
    popStatus->paint(dt);
}

void showPopStatus(bool show)
{
    popStatus->show(show);
}

