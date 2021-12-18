#include "App.h"

App::App()
	:
	wnd(new Window(800, 600, "DirectX_Game"))
{ }

App::~App()
{
    if (wnd != nullptr)
    {
        delete wnd;
        wnd = nullptr;
    }
}

int App::Loop()
{ 
    while (true)
    {
        if (const auto ecode = Window::ProcessMessages())   //Looks for messages and return if QUIT
        {
            return *ecode;
        } 

        Update(); 
    } 
}

void App::Update()
{
    timer.StartClock();

    i += 0.02f;
    const float c = sin(i) / 2.0f + 0.5f;
    
    wnd->GetGraphics()->ClearFrame(c, c, 1.0f);
    wnd->GetGraphics()->RenderFrame();

    timer.EndClock();
    timer.DelayByFrameTime();
}
