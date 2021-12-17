#include "Window.h"
#include "resource.h"

//static init
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
    :
    hInst(GetModuleHandle(nullptr)) //Get the handle to the instance (no need to pass it through the entry point)
{ 
    //Register a window class  
    WNDCLASSEX wcex = { 0 };    //init all elements to 0
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetInstance();
    wcex.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(WindowIcon), IMAGE_ICON, 16, 16, 0));
    wcex.hIconSm = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr; //No background (DX will handle that)
    wcex.lpszMenuName = nullptr;  
    wcex.lpszClassName = GetName();  
    wcex.lpfnWndProc = HandleMsgSetup; //Name of the function that processes all windows messages   

    RegisterClassEx(&wcex);  
}

Window::WindowClass::~WindowClass()
{ 
    UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName()
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance()
{
    return wndClass.hInst;
}


//Window stuff
Window::Window(int _width, int _height, std::string _name)
    :
    width(width),
    height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = _width + wr.left;
    wr.top = 100;
    wr.bottom = _height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    //Create window and get hWnd
    hWnd = CreateWindow(
        WindowClass::GetName(),                   //Class name
        _name.c_str(),                            //title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Style
        CW_USEDEFAULT,                            //Default x position
        CW_USEDEFAULT,                            //Default y position  
        wr.right - wr.left,                       //Width
        wr.bottom - wr.top,                       //Height
        nullptr,                                  //no parent
        nullptr,                                  //no menu
        WindowClass::GetInstance(),               //wndClassInst
        this                                      //Custom param needed for setting the wndProc
    );

    //Display window
    ShowWindow(hWnd, SW_SHOWDEFAULT);

    //Init Graphics 
    gfx = new Graphics(hWnd);
}

Window::~Window()
{
    if (gfx != nullptr)
    {
        delete gfx;
        gfx = nullptr;
    }
    DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessages()
{ 
    //Check if there is a message and returns an empty optional, otherwise the quit message

    MSG msg;
    // while queue has messages, remove and dispatch them (but do not block on empty queue)
    while (PeekMessage(	
        &msg,			 
        nullptr, 		
        0, 				 
        0,
        PM_REMOVE))		//Messages are removed from the queue after processing by PeekMessage
    {
        // check for quit because peekmessage does not signal this via return val
        if (msg.message == WM_QUIT)
        {
            // return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
            return (int)msg.wParam;
        }

        //TranslateMessage will post auxilliary WM_CHAR messages from key msgs
        //translates virtual-key messages into characters (WM_CHAR) (basically sends a WM_CHAR messages when pressing a key, so it can also be omitted if not using that message)
        TranslateMessage(&msg);		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage

        //Pass msg to the WndProc (in our case the default one "DefWindowProc" (which we can extend later on...)
        //DefWindowProc doesn't know whether we want to quit the whole application or we just close the window, so when closing the app will still run
        DispatchMessage(&msg);		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
    }

    // return empty optional when not quitting app
    return {};
}

Graphics* Window::GetGraphics()
{
    return gfx;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    //Use create parameter passed in from CreateWindowA() to store window class pointer at WinAPI side
    if (_msg == WM_NCCREATE) //Called before the creation of a window (and lParam contains info about the window being created (this))
    {
        //Extract ptr to window class from creation data 
        const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(_lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams); //lpCreateParams is a pointer to window (as we passed it in CreateWindowA()
        //Set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        //Set a new address for the window procedure (now HandleMsgSecond will be called instead of HandleMsgSetup)
        SetWindowLongPtr(_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgSecond));
        //Forward message to window instance handler
        return pWnd->HandleMsg(_hWnd, _msg, _wParam, _lParam);
    }

    //if we get message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}

//Adaptor from WIN32 call convention to C++ call convention 
LRESULT WINAPI Window::HandleMsgSecond(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    //Retrieve ptr to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
    //Forward message to window clas handler
    return pWnd->HandleMsg(_hWnd, _msg, _wParam, _lParam);
}

LRESULT Window::HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    switch (_msg)
    {
    case WM_DESTROY:    //Provides a WM_QUIT that will end the loop
        PostQuitMessage(0);
        return 0;
    }

    //Process any window messages
    return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}
