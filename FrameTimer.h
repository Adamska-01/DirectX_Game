#pragma once
#include <chrono>


class FrameTimer
{
private:
	static constexpr int FPS = 60;
	static constexpr float FRAMETIME = 1000.0f / FPS; //millisecond
	static int currentFPS;
	static float deltaTime;
	static float time;

	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	std::chrono::duration<float, std::milli> workTime;

	int countedFrames;
	float counterDelay;

private:
	void CalculateFPS();

public:
	FrameTimer();

	void StartClock();
	void EndClock();

	void DelayByFrameTime();

	//dt
	static float DeltaTime() { return deltaTime; }
	static int Frames() { return currentFPS; } 
	static float Time() { return time; }
};