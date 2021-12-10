#include "iFPS.h"

#include "iWindow.h"
#include "iStd.h"

iFPS* iFPS::instance()
{
	static iFPS* ins = new iFPS();
	return ins;
}

iFPS::iFPS()
{
	now = GetTickCount();
#ifdef DISPLAY_FPS
	framesPerSec = 0;
	tickDt = 0.0f;
	num = 0;
#endif
	lastDt = 0.0f;

	ingSlowTime = false;
	_slowDelta = 0.0f;
	slowDelta = 0.0f;
	_slowDelay = 0.0f;
	slowDelay = 0.0f;
}

iFPS::~iFPS()
{
}

float iFPS::update()
{
	unsigned long n = GetTickCount();
	float dt = (n - now) / 1000.f;
#ifdef DISPLAY_FPS
	tickDt += dt;
	num++;
	if (tickDt > 0.3f)
	{
		framesPerSec = num / tickDt;

		tickDt = 0.0f;
		num = 0;
	}
#endif
	now = n;
	lastDt = dt;
	if (ingSlowTime)
	{
		if (slowDelta < _slowDelta)
		{
			// 느려지고(dt => 0.0f)
			//float delta = linear(slowDelta / _slowDelta, dt, 0.0f);
			float delta = dt * (1 - _sin(90 * slowDelta / _slowDelta));
			//float delta = dt * (1 - _sin(90 * _sin(90 * slowDelta / _slowDelta)));

			slowDelta += dt;
			if (slowDelta > _slowDelta)
				slowDelta = _slowDelta;

			dt = delta;
		}
		else if (slowDelta == _slowDelta)
		{
			// 멈춤

			slowDelay += dt;
			if (slowDelay > _slowDelay)
				slowDelta += 0.000001f;

			dt = 0.0f;
		}
		else if (slowDelta < _slowDelta * 2)
		{
			// 빨리지고(0.0f => dt)
			//float delta = linear((slowDelta - _slowDelta) / _slowDelta, 0.0f, dt);
			float r = (slowDelta - _slowDelta) / _slowDelta;
			float delta = dt * r * r;
			//float delta = dt * r * r * r;

			slowDelta += dt;

			dt = delta;
		}
		else
		{
			ingSlowTime = false;
		}
	}
	return dt;
}

void iFPS::setSlowTime(float delta, float delay)
{
	if (ingSlowTime)
		return;

	ingSlowTime = true;
	_slowDelta = delta;
	slowDelta = 0.0f;
	_slowDelay = delay;
	slowDelay = 0.0f;
}

