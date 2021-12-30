#include <windows.h> 
#include "App.h"

 
int WINAPI WinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, 
    _In_ int nCmdShow)
{
    App app; 

    return app.Loop(); 
} 