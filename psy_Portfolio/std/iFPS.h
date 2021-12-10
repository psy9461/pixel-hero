#pragma once

#define DISPLAY_FPS

class iFPS
{
public:
	static iFPS* instance();
private:
	iFPS();
public:
	virtual ~iFPS();

	float update();

	void setSlowTime(float delta, float delay);

public:
	unsigned long now;
#ifdef DISPLAY_FPS
	int framesPerSec;
	float tickDt;
	int num;
#endif // DISPLAY_FPS
	float lastDt;

	bool ingSlowTime;
	float slowDelta, _slowDelta;
	float slowDelay, _slowDelay;
};
