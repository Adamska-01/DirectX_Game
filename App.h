#pragma once
#include "Window.h" 
#include "FrameTimer.h"


class App
{
public:
	App();
	~App();

	//Message loop
	int Loop();  
private:
	void Update();

private:
	Window* wnd; 
	float i = 0.0f;

	FrameTimer timer;
};