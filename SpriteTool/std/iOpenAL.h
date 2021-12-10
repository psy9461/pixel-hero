#pragma once

#include "iType.h"

#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32.lib")

class iOpenAL
{
public:
	iOpenAL(int audioNum);
	virtual ~iOpenAL();

	void initBuffer(int idx, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	void play(int idx, bool repeat);
	void pause(int idx);
	void stop(int idx);
	void volume(int idx, float vol);

public:
	ALuint* gBuffer;
	ALuint* gSource;
	int bufCount;
};

struct AudioInfo
{
	const char* fileName;
	bool repeat;
	float volume;
};

void loadAudio(AudioInfo* ai, int aiNum);
void freeAudio();

void pauseAudio();
void resumeAudio();

void audioPlay(int index);
void audioStop(int index);
void audioStop();
void audioVolume(float bgm, float eff, int effNum);

