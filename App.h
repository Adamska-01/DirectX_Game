#pragma once
#include "Window.h" 


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
	 
};