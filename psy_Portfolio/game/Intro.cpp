#include "Loading.h"
#include "Intro.h"
#include "Stage1A.h"
#include "StageEditor.h"
#include "ProcData.h"
#include "Entity.h"

Texture* texPress;
float pressDt;
bool isPress;

void createIntroScene()
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(480, 55);
    g->init(size);
    setStringSize(50);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(3);
    setStringBorderRGBA(1, 1, 1, 1);
    setStringName("assets/CraftPixNet Survival Kit.otf");
    g->drawString(size.width / 2, 27, VCENTER | HCENTER, "Press Any Key");
    texPress = g->getTexture();
    pressDt = 0.0f;
    isPress = false;
}

Texture* texIntroBg;
float introBgScaleX, introBgScaleY;
void loadIntro()
{
    if (entPool == NULL)
        loadEntpool();
    if (projPool == NULL)
        loadProjpool();

    audioPlay(1);

    //texIntroBg = createImage("assets/background/introBG.jpg");
    texIntroBg = createImage("assets/background/introBG2.png");

    introBgScaleX = devSize.width / texIntroBg->width;
    introBgScaleY = devSize.height / texIntroBg->height;

    createIntroScene();
    createPopTitle();
    createPopMenu();
    setStringNameKor;
    createPopNewGame();
    createPopStageEditor();
    createPopOption();
    setStringNameEng;
    //createPopExit();

    showPopTitle(true);
}

void freeIntro()
{
    freeImage(texIntroBg);
    freeImage(texPress);
    freePopTitle();
    freePopMenu();
    freePopNewGame();
    freePopStageEditor();
    freePopOption();
    //freePopExit();
}

void drawIntro(float dt)
{
    //setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	//clearRect();
    //setRGBA(1, 1, 1, 1);

    drawImage(texIntroBg, 0, 0, TOP | LEFT, 0, 0, texIntroBg->width, texIntroBg->height,
        introBgScaleX, introBgScaleY, 2, 0);
    drawPopTitle(dt);

    pressDt += dt;
    if (pressDt < 3.0f)
        return;
    
    float a = _sin(180 * pressDt);
    if (a < 0)
        a *= -1;
    
    setRGBA(1, 1, 1, a);
    if(!isPress)
        drawImage(texPress, devSize.width / 2, devSize.height * 0.7f, VCENTER | HCENTER);
    setRGBA(1, 1, 1, 1);
    
    drawPopMenu(dt);
    drawPopNewGame(dt);
    drawPopStageEditor(dt);
    drawPopOption(dt);

    setRGBA(0, 0, 1, 0);
    drawRect(200, 200, 200, 200);
    fillRect(500, 200, 200, 200);
    setRGBA(1, 1, 1, 1);
 }

void keyIntro(iKeyStat stat, iPoint point)
{
    if (keyPopMenu(stat, point)         ||
        keyPopNewGame(stat, point)      ||
        keyPopStageEditor(stat, point)  ||
        keyPopOption(stat, point))
 //       keyPopExit(stat, point))
        return;

    switch (stat)
    {
    case iKeyStatBegan:
        if (pressDt < 3.0f)
            break;
        if(!isPress)
        {
            isPress = true;
            showPopMenu(true);
        }
        break;

    case iKeyStatMoved:
        break;
    case iKeyStatEnded:
        break;
    }
}

// ===================================
// popTitle
// ===================================
iPopup* popTitle;

void createPopTitle()
{
    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();
    igImage* igImg = g->createIgImage("assets/UI/logo/Logo.png");
    iSize size = iSizeMake(igImg->GetWidth() * 2.5f, igImg->GetHeight() * 2.5f);
    g->init(size);
    g->drawIgImage(igImg, 0, 0, TOP | LEFT, 0, 0, igImg->GetWidth(), igImg->GetHeight(),
        2.5f, 2.5f, 2, 0);
    setStringSize(70);
    setStringRGBA(yellowMain.r, yellowMain.g, yellowMain.b, yellowMain.a);
    setStringBorder(3);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height/2 - 15, VCENTER | HCENTER, "Pixel Hero");

    Texture* tex= g->getTexture();
    iImage* img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    img->position = iPointZero;
    pop->addObject(img);

    pop->_aniDt = 2.0f;
    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width / 2 - size.width / 2, -size.height);
    pop->closePoint = iPointMake(devSize.width / 2 - size.width / 2, 60);

    popTitle = pop;
}
void freePopTitle()
{
    delete popTitle;
}
void drawPopTitle(float dt)
{
    popTitle->paint(dt);
}

void showPopTitle(bool show)
{
    popTitle->show(show);
    if (show)
    {

    }
    else
    {

    }
}


// ===================================
// popMenu
// ===================================
iPopup* popMenu;
iImage** imgBtn;
float playTime;
int indexCloseMenu;

void drawPopMenuBefore(iPopup* pop, float dt, float rate);

void popMenuClose(iPopup* pop)
{
    if (indexCloseMenu == -1)
        return;

    void (*MethodShow[4])(bool show) = {
        showPopNewGame,
        showPopStageEditor,
        showPopOption,
        showPopExit
    };

    MethodShow[indexCloseMenu](true);
}

void createPopMenu()
{
    //iImage* img;
    Texture* tex;
    int i, j, k;

    const char* strBtn[4] = {
        "New Game",
        "Map Editor",
        "Option",
        "Exit"
    };

    iPopup* pop = new iPopup();

    imgBtn = new iImage * [4];
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(160, 50);
    for (i = 0; i < 4; i++) 
    {
        iImage* img = new iImage();
        for (j = 0; j < 2; j++)
        {
            igImage* igImg;
            if (j == 0) igImg = g->createIgImage("assets/UI/bars/Button3.png");
            else        igImg = g->createIgImage("assets/UI/bars/Button4.png");
            
            float scaleX = size.width / igImg->GetWidth();
            float scaleY = size.height / igImg->GetHeight();

            g->init(size);
            g->drawIgImage(igImg, 0, 0, TOP | LEFT, 0, 0, igImg->GetWidth(), igImg->GetHeight(),
                scaleX, scaleY, 2, 0);

            setStringSize(25);
            setStringYellow;
            setStringBorder(4);
            setStringBorderBrown;
            g->drawString(size.width / 2, size.height / 2 - 3, VCENTER | HCENTER, strBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = iPointMake(0, 65 * i);
        pop->addObject(img);

        imgBtn[i] = img;

    }
    indexCloseMenu = -1;

    pop->style = iPopupNone;
    pop->openPoint = iPointMake(devSize.width / 2 - 75, devSize.height / 2 - 10);
    pop->closePoint = iPointMake(devSize.width / 2 - 75, devSize.height / 2 - 10);
    pop->methodBefore = drawPopMenuBefore;
    pop->methodClose = popMenuClose;
    popMenu = pop;
}
void freePopMenu()
{
    delete popMenu;
    delete imgBtn;
}

void drawPopMenuBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 4; i++)
        imgBtn[i]->setTexObject(i == popMenu->selected);
}

void drawPopMenu(float dt)
{
    playTime += dt;

    popMenu->paint(dt);
}

void showPopMenu(bool show)
{
    popMenu->show(show);
    if (show)
    {

    }
    else
    {

    }
}

void closePopMenuCommon(iPopup* pop)
{
    showPopMenu(true);
}

bool keyPopMenu(iKeyStat stat, iPoint point)
{
    if (popMenu->bShow == false)
        return false;
    if (popMenu->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popMenu->selected == -1)
            break;

        if (popMenu->selected == 0)
        {
            indexCloseMenu = 0;
            showPopMenu(false);
        }
        else if (popMenu->selected == 1)
        {
            indexCloseMenu = 1;
            showPopMenu(false);
        }
        else if (popMenu->selected == 2)
        {
            indexCloseMenu = 2;
            showPopMenu(false);
        }
        else
        {
            indexCloseMenu = 3;
            showPopExit(true);
            showPopMenu(false);
        }

        break;
    case iKeyStatMoved:
        for (i = 0; i < 4; i++)
        {
            if (containPoint(point, imgBtn[i]->touchRect(popMenu->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popMenu->selected != j)
        {
            audioPlay(0);
            popMenu->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;

    }
    return true;
}

// ===================================
// popNewGame
// ===================================
iPopup* popNewGame;
iImage** imgNewGameBtn;
void drawPopNewGameBefore(iPopup* pop, float dt, float rate);

void createPopNewGame()
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
    setStringYellow;
    setStringBorder(2);
    setStringBorderBrown;
    //setStringBorderRGBA(1, 0, 1, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "새 게임을\n시작하시겠습니까?");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    const char* strBtn[2] = {
        "예", "아니오"
    };

    imgNewGameBtn = new iImage * [2];
    for (i = 0; i < 2; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            size = iSizeMake(150, 40);
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

        img->position = iPointMake(70 + 180 * i, 250);
        pop->addObject(img);

        imgNewGameBtn[i] = img;
    }

    pop->style = iPopupRotate;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 480) / 2, (devSize.height - 320) / 2);
    pop->methodBefore = drawPopNewGameBefore;
    pop->methodClose = closePopMenuCommon;
    popNewGame = pop;
}
void freePopNewGame()
{
    delete popNewGame;
    delete imgNewGameBtn;
}

void drawPopNewGameBefore(iPopup* pop, float dt, float rate)
{
    setRGBA(0, 0, 0, 0.7f * rate);
    fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);

    for (int i = 0; i < 2; i++)
    {
        imgNewGameBtn[i]->setTexObject(i == popNewGame->selected);
    }
}

void drawPopNewGame(float dt)
{
    popNewGame->paint(dt);
}
void showPopNewGame(bool show)
{
    popNewGame->show(show);
    if (show)
    {

    }
}
bool keyPopNewGame(iKeyStat stat, iPoint point)
{
    if (popNewGame->bShow == false)
        return false;
    if (popNewGame->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popNewGame->selected == -1)
            break;

        if (popNewGame->selected == 0)
        {
            hero->hp = hero->maxHp;
            setLoading(gs_stage1A, freeIntro, loadStage1A);
            printf("확인\n");
        }
        else// if (popExit->selected == 1)
        {
            printf("아니오\n");
            showPopNewGame(false);
        }

        break;
    case iKeyStatMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(point, imgNewGameBtn[i]->touchRect(popNewGame->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popNewGame->selected != j)
        {
            audioPlay(0);
            popNewGame->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;

    }
    return true;
}

// ===================================
// popStageEditor
// ===================================
iPopup* popStageEditor;
iImage** imgStageEditorBtn;
void drawPopStageEditorBefore(iPopup* pop, float dt, float rate);

void createPopStageEditor()
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
    setStringYellow;
    setStringBorderBrown;
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "맵 에디터를\n실행하시겠습니까?");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    const char* strBtn[2] = {
        "예", "아니오"
    };

    imgStageEditorBtn = new iImage * [2];
    for (i = 0; i < 2; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            size = iSizeMake(150, 40);
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

        img->position = iPointMake(70 + 180 * i, 250);
        pop->addObject(img);

        imgStageEditorBtn[i] = img;
    }

    pop->style = iPopupRotate;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 480) / 2, (devSize.height - 320) / 2);
    pop->methodBefore = drawPopStageEditorBefore;
    pop->methodClose = closePopMenuCommon;
    popStageEditor = pop;
}
void freePopStageEditor()
{
    delete popStageEditor;
    delete imgStageEditorBtn;
}

void drawPopStageEditorBefore(iPopup* pop, float dt, float rate)
{
    setRGBA(0, 0, 0, 0.7f * rate);
    fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);

    for (int i = 0; i < 2; i++)
    {
        imgStageEditorBtn[i]->setTexObject(i == popStageEditor->selected);
    }
}
void drawPopStageEditor(float dt)
{
    popStageEditor->paint(dt);
}
void showPopStageEditor(bool show)
{
    popStageEditor->show(show);
    if (show)
    {

    }
}
bool keyPopStageEditor(iKeyStat stat, iPoint point)
{
    if (popStageEditor->bShow == false)
        return false;
    if (popStageEditor->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popStageEditor->selected == -1)
            break;

        if (popStageEditor->selected == 0)
        {
            setLoading(gs_stageedit, freeIntro, loadStageEditor);
            printf("확인\n");
        }
        else// if (popExit->selected == 1)
        {
            printf("아니오\n");
            showPopStageEditor(false);
        }

        break;
    case iKeyStatMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(point, imgStageEditorBtn[i]->touchRect(popStageEditor->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popStageEditor->selected != j)
        {
            audioPlay(0);
            popStageEditor->selected = j;
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

iPopup* popOption;
iImage** imgOptionBtn;
iImage** imgVolume;

int volume;
void drawPopOptionBefore(iPopup* pop, float dt, float rate);

void createPopOption()
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

    imgOptionBtn = new iImage * [5];
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
                size = iSizeMake(100, 50);
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
            img->position = iPointMake(190, 240);
        pop->addObject(img);

        imgOptionBtn[i] = img;
    }

    imgVolume = new iImage * [10];
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

        imgVolume[i] = img;
    }

    pop->style = iPopupRotate;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 480) / 2, (devSize.height - 320) / 2);
    pop->methodBefore = drawPopOptionBefore;
    pop->methodClose = closePopMenuCommon;
    popOption = pop;

    volume = 3;
}
void freePopOption()
{
    delete popOption;
    delete imgOptionBtn;
}

void drawPopOptionBefore(iPopup* pop, float dt, float rate)
{
    setRGBA(0, 0, 0, 0.7f * rate);
    fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);

    for (int i = 0; i < 5; i++)
    {
        imgOptionBtn[i]->setTexObject(i == popOption->selected);
    }

    for (int i = 0; i < 10; i++)
    {
        if (i < volume)
            imgVolume[i]->setTexObject(1);
        else
            imgVolume[i]->setTexObject(0);
    }

    if (isFullscreen)
    {
        imgOptionBtn[2]->setTexObject(1);
    }
    else
    {
        imgOptionBtn[3]->setTexObject(1);
    }
}

void drawPopOption(float dt)
{
    popOption->paint(dt);
}
void showPopOption(bool show)
{
    popOption->show(show);
    if (show)
    {

    }
}

bool keyPopOption(iKeyStat stat, iPoint point)
{
    if (popOption->bShow == false)
        return false;
    if (popOption->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popOption->selected == -1)
            break;

        if (popOption->selected == 0)
        {
            // 사운드 줄이기
            volume--;
            if (volume < 0)
                volume == 0;
            audioVolume(0.1f * volume, 0.1f * volume, 1);
        }
        else if (popOption->selected == 1)
        {
            // 사운드 올리기
            volume++;
            if (volume > 10)
                volume == 10;
            audioVolume(0.1f * volume, 0.1f * volume, 1);
        }
        else if (popOption->selected == 2)
        {
            // 전체화면
            goFullscreen(true);
        }
        else if (popOption->selected == 3)
        {
            // 창모드
            goFullscreen(false);
        }
        else if (popOption->selected == 4)
        {
            // 볼륨 크기
            showPopOption(false);
        }
        else if (popOption->selected > 4 && popOption->selected < 15)
        {

        }

        break;
    case iKeyStatMoved:
        for (i = 0; i < 5; i++)
        {
            if (containPoint(point, imgOptionBtn[i]->touchRect(popOption->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popOption->selected != j)
        {
            audioPlay(0);
            popOption->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;

    }
    return true;

}



// ===========================================
// popExit
// ===========================================
iPopup* popExit = NULL;
iImage** imgExitBtn;
bool reserveShowPopExit;

void drawPopExitBefore(iPopup* pop, float dt, float rate);
void _showPopExit(bool show);

void createPopExit()
{
    setStringNameKor;
    if (popExit)
        return;

    iImage* img;
    Texture* tex;
    int i, j;

    iPopup* pop = new iPopup();

    //
    // bg
    //
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
    setStringYellow;
    setStringBorderBrown;
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "게임을\n종료하시겠습니까?");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //
    // btn
    //
    const char* strBtn[2] = {
        "예", "아니오"
    };

    imgExitBtn = new iImage * [2];
    for (i = 0; i < 2; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            size = iSizeMake(150, 40);
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

        img->position = iPointMake(70 + 180 * i, 250);
        pop->addObject(img);

        imgExitBtn[i] = img;
    }

    pop->style = iPopupRotate;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 480) / 2, (devSize.height - 320) / 2);
    pop->methodBefore = drawPopExitBefore;
    pop->methodClose = NULL;
    popExit = pop;

    reserveShowPopExit = false;
    setStringNameEng;
}

void freePopExit()
{
    delete popExit;
    delete imgExitBtn;
}

void drawPopExitBefore(iPopup* pop, float dt, float rate)
{
    setRGBA(0, 0, 0, 0.7f * rate);
    fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);

    for (int i = 0; i < 2; i++)
    {
        imgExitBtn[i]->setTexObject(i == popExit->selected);
    }
}

void drawPopExit(float dt)
{
    if (popExit != NULL)
        popExit->paint(dt);

    if (reserveShowPopExit)
    {
        _showPopExit(true);
        reserveShowPopExit = false;
    }
}

void closePopExit0(iPopup* pop)
{
    //freePopExit();
    extern bool runWnd;
    runWnd = false;
}

void closePopExit1(iPopup* pop)
{
    if (popMenu)
        showPopMenu(true);
}

void _showPopExit(bool show)
{
    if (show)
    {
        createPopExit();
    }
    popExit->show(show);
}

void showPopExit(bool show)
{
    if (show)
    {
        reserveShowPopExit = true;
    }
    else
    {
        _showPopExit(false);
    }
}

bool keyPopExit(iKeyStat stat, iPoint point)
{
    if (popExit->bShow == false)
        return false;
    if (popExit->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popExit->selected == -1)
            break;

        if (popExit->selected == 0)
        {
            // 프로그램 종료
            popExit->methodClose = closePopExit0;
        }
        else
        {
            popExit->methodClose = closePopExit1;
        }

        showPopExit(false);

        break;
    case iKeyStatMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(point, imgExitBtn[i]->touchRect(popExit->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popExit->selected != j)
        {
            audioPlay(0);
            popExit->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;

    }
    return true;
}




