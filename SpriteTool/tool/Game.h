#pragma once

#include "iStd.h"

void loadGame();
void drawGame(float dt);
void freeGame();
void keyGame(iKeyStat stat, iPoint p, bool leftRight = false);

extern iPopup* popExit;