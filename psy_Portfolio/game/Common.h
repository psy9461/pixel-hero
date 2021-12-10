#pragma once

#include "iStd.h"

#define gravityForce	500.0f
#define gravityMax		500.0f
#define jumpForce		-350.0f

struct Entity;
extern Entity*** entPool;
extern Entity** entity;
extern int entNum;

struct Projectile;
extern Projectile*** projPool;
extern Projectile** projectile;
extern int projNum;

struct Hero;
extern Hero* hero;

extern float strLineHeight;

void loadEntpool();
void freeEntpool();
void loadProjpool();
void freeProjpool();


void loadCommon();
void freeCommon();

// -----------------------------------------
// popCommonMenu
// -----------------------------------------
struct iPopup;
extern iPopup* popCommonMenu;
extern bool toggleCommonMenu;

void createPopCommonMenu();
void freePopCommonMenu();
void showPopCommonMenu(bool show);
void drawPopCommonMenu(float dt);
bool keyPopCommonMenu(iKeyStat stat, iPoint point);

void setCommonMethodLoadFree(void* mLoad, void* mFree);

// ===================================
// popOption
// ===================================
extern iPopup* popOptionProc;
void createPopOptionProc();
void freePopOptionProc();
void drawPopOptionProc(float dt);
void showPopOptionProc(bool show);
bool keyPopOptionProc(iKeyStat stat, iPoint point);

// ===================================
// popStatus
// ===================================
void createPopStatus();
void freePopStatus();
void drawPopStatus(float dt);
void showPopStatus(bool show);
bool keyPopStatus(iKeyStat stat, iPoint point);



