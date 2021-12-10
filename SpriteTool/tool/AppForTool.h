#pragma once

#pragma once

#include "iWindow.h"
#include "iStd.h"

// 더블 버퍼링
// 개발 사이즈(후면버퍼, 백버퍼)
// 해상도 가용화면(viewport)==실제 사이즈(전면버퍼, 프론티어버퍼)

// 개발 사이즈 100x50
// 단말 사이즈 200x120	200x100(높이 위 아래 10암전처리)

// devSize (100, 50)
// viewport (0, 10, 200, 100)

// 

void goFullscreen();
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);

extern iPoint cursor;
void loadCursor();
void freeCursor();
void drawCursor(float dt);
bool updateCursor(bool inClient);
