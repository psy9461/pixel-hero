#pragma once

#include "iStd.h"

#define gs_intro	0
#define gs_stage1A	1

#define gs_stageedit 100
#define gs_loading	0xff

extern int gameState;
extern bool loading;

typedef void (*MethodLoad)();
void setLoading(int gameState, MethodLoad free, MethodLoad load);
void drawLoading(float dt);
