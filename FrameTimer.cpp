#include "FrameTimer.h"
#include <thread>

using namespace std::chrono;

float FrameTimer::deltaTime = 0;
int FrameTimer::currentFPS = 0;

FrameTimer::FrameTimer()
{
	start = system_clock::now();
	end = system_clock::now(); 

	countedFrames = counterDelay = 0;
}

void FrameTimer::CalculateFPS()
{
	countedFrames++;
	counterDelay += workTime.count();
	if (counterDelay >= 1000.0f)
	{
		deltaTime = (1.0f / countedFrames);
		currentFPS = countedFrames - 1;

		counterDelay = 0;
		countedFrames = 0;
	}
}

void FrameTimer::StartClock()
{
	start = system_clock::now();
}

void FrameTimer::EndClock()
{
	end = system_clock::now();
}

void FrameTimer::DelayByFrameTime()
{
	workTime = end - start;;
	CalculateFPS();

	if (workTime.count() < FRAMETIME)
	{
		duration<float, std::milli> delta_ms(FRAMETIME - workTime.count());
		auto delta_ms_duration = duration_cast<std::chrono::milliseconds>(delta_ms);

		counterDelay += delta_ms_duration.count(); //For FPS calculation

		std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
	}
}