#pragma once

#include "iWindow.h"
#include "iStd.h"

extern bool isFullscreen;

void goFullscreen();
void goFullscreen(bool goFull);
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);

extern iPoint cursor;
void loadCursor();
void freeCursor();
void drawCursor(float dt);
bool updateCursor(bool inClient);


