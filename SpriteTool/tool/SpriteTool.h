#pragma once

#include "iWindow.h"
#include "SpriteData.h"
#include "Sprite.h"

extern iRect rtOpenGL;

void loadSpriteTool();
void freeSpriteTool();
void drawSprite(float dt);
void updateSpriteTool(WPARAM wParam, LPARAM lParam);
void keySpriteTool(iKeyStat stat, iPoint point);

void loadSpriteFile();
void _loadSpriteFile(const char* path);
void saveSpriteFile();

void addState();
void delState();
void udtState();
void upState();
void downState();

void addFrame();
void delFrame();
void upFrame();
void downFrame();

Frame* genDummyFrame();

Frame* getCurrFrame(bool bCreateTex);
void loadFrameImg();
void _loadFrameImg(const char* path);



