#pragma once
#include <Windows.h>
#include <string>
#include <optional>
#include "Graphics.h"

class Window
{
private:
	//Singlet manages registration/cleanup of window class
	class WindowClass
	{ 
	private:
		WindowClass(); 
		~WindowClass(); 
	private:
		static WindowClass wndClass;
		static constexpr const char* wndClassName = "DirectX_Game";
		HINSTANCE hInst; //Pointer to current inatance of the application 

	public:
		static const char* GetName();
		static HINSTANCE GetInstance();
	}; 

public:
	Window(int _width, int _height, std::string _name);
	~Window();

	static std::optional<int> ProcessMessages();	//static because should process msgs for all windows

	//Getters
	int GetWidth();
	int GetHeight();

public: 
	Graphics* GetGraphics();

private: //Enables wnd msg as a member function 
	static LRESULT CALLBACK HandleMsgSetup(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	static LRESULT CALLBACK HandleMsgSecond(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	LRESULT HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	int width;
	int height;
	HWND hWnd;

	Graphics* gfx;
};

 