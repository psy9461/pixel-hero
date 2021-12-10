#pragma once

#include "iStd.h"
#include "Common.h"

void createIntroScene();
void loadIntro();
void freeIntro();
void drawIntro(float dt);
void keyIntro(iKeyStat stat, iPoint point);

// ===================================
// popTitle
// ===================================
void createPopTitle();
void freePopTitle();
void drawPopTitle(float dt);
void showPopTitle(bool show);

// ===================================
// popMenu
// ===================================
void createPopMenu();
void freePopMenu();
void drawPopMenu(float dt);
void showPopMenu(bool show);
bool keyPopMenu(iKeyStat stat, iPoint point);

// ===================================
// popNewGame
// ===================================
void createPopNewGame();
void freePopNewGame();
void drawPopNewGame(float dt);
void showPopNewGame(bool show);
bool keyPopNewGame(iKeyStat stat, iPoint point);

// ===================================
// popStageEditor
// ===================================
void createPopStageEditor();
void freePopStageEditor();
void drawPopStageEditor(float dt);
void showPopStageEditor(bool show);
bool keyPopStageEditor(iKeyStat stat, iPoint point);


// ===================================
// popOption
// ===================================
void createPopOption();
void freePopOption();
void drawPopOption(float dt);
void showPopOption(bool show);
bool keyPopOption(iKeyStat stat, iPoint point);
// ===========================================
// popExit
// ===========================================
extern iPopup* popExit;
void createPopExit();
void freePopExit();
void drawPopExit(float dt);
void showPopExit(bool show);
bool keyPopExit(iKeyStat stat, iPoint point);



