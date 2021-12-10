#include "Loading.h"

int gameState = gs_intro, toGameState;
MethodLoad methodFree;
MethodLoad methodLoad;
float loadingDt;
bool loading = false;


#define _loadingDt 0.5f

static Texture* texLoading;

void setLoading(int gameState, MethodLoad free, MethodLoad load)
{
	if (loadingDt)
		return;

	toGameState = gameState;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.000001f;

	loading = true;
	//texLoading = createImage("assets/loading/loading.jpg");
}

#define DELAY_DT 1.0f
void drawLoading(float dt)
{
	if (loadingDt == 0.0f)
		return;

	float a = 1.0f;

	if (loadingDt < _loadingDt)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt)
			loadingDt = _loadingDt;

		a = loadingDt / _loadingDt; // 점점 어두워진다.
	}
	else if (loadingDt == _loadingDt)
	{
		loadingDt = _loadingDt + 0.000001f;

		if (methodFree)
			methodFree();
		gameState = gs_loading;

		audioStop();////////////////////////////////////////////

#if CHECK_TEXTURE
		printf("texture_num : %d\n", texture_num);
#endif
		a = 1.0f;
	}
	else if (loadingDt < _loadingDt + DELAY_DT)
	{
		loadingDt += dt;
		if (loadingDt >= _loadingDt + DELAY_DT)
		{
			if (methodLoad)
				methodLoad();
			gameState = toGameState;

			loadingDt = _loadingDt + DELAY_DT + 0.000001f;
		}

		a = 1.0f;
	}
	else if (loadingDt < _loadingDt * 2 + DELAY_DT)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt * 2 + DELAY_DT)
		{
			loadingDt = 0.0f;
			//freeImage(texLoading);
			return;
		}

		a = 1.0f - (loadingDt - DELAY_DT - _loadingDt) / _loadingDt; // 점점 밝아진다.
	}

	setRGBA(0, 0, 0, a);
	fillRect(0, 0, devSize.width, devSize.height);
	//drawImage(texLoading, devSize.width / 2, devSize.height / 2,
	//	VCENTER | HCENTER,
	//	0, 0, texLoading->width, texLoading->height,
	//	1.0f, 1.0f, 2, 0);
	setRGBA(1, 1, 1, 1);
}
