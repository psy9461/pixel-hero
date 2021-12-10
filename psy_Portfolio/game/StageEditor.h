#pragma once

#include "iStd.h"

#include "iWindow.h"

#include "Common.h"


void loadStageEditor();
void drawStageEditor(float dt);
void freeStageEditor();
void keyStageEditor(iKeyStat stat, iPoint point, bool leftRight);

// -----------------------------------------
// popField
// -----------------------------------------

// 1. 데이터를 넣어주고 load하면서 popField가 초기화
// 2. 데이터를 넣어주지 않으면 popField는 거대한 맵으로 초기화하고 최종적으로 가로와 세로크기를 기록해서 저장
void createPopField(void* data = NULL);
void freePopField();
void showPopField(bool show);
void drawPopField(float dt);
bool keyPopField(iKeyStat stat, iPoint point, bool leftRight);

// -----------------------------------------
// popTiles
// -----------------------------------------
extern Texture*** texTiles;

extern int selectedObj;
extern int selectedTheme;

void createPopTiles();
void freePopTiles();
void showPopTiles(bool show);
void drawPopTiles(float dt);
bool keyPopTiles(iKeyStat stat, iPoint point);

// -----------------------------------------
// popEntity
// -----------------------------------------
extern Texture** texEntity;

void createPopEntity();
void freePopEntity();
void showPopEntity(bool show);
void drawPopEntity(float dt);
bool keyPopEntity(iKeyStat stat, iPoint point);


// -----------------------------------------
// popEditorMenu
// -----------------------------------------
extern struct loadData;
void createPopEditorMenu();
void freePopEditorMenu();
void showPopEditorMenu(bool show);
void drawPopEditorMenu(float dt);
bool keyPopEditorMenu(iKeyStat stat, iPoint point);

void saveStageData();
void loadStageData(char* stageDataPath, loadData* ld);

void drawEntityScale(float dt);


