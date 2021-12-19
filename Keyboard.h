#pragma once
#include "Input.h"

class Keyboard : Input
{
private:
	enum class keyStates
	{
		DOWN, PRESSED, UP, NOTPRESSED
	};
private:
	IDirectInputDevice8* pKeyboardDevice;
	unsigned char keyboardStates[256];
	keyStates keystates[256];

public:
	Keyboard(HINSTANCE _hInst, HWND _hWnd);
	~Keyboard() override;

	HRESULT ReadInputStates() override;

	bool IsKeyDown(unsigned char _key);
	bool IsKeyPressed(unsigned char _key);
	bool IsKeyUp(unsigned char _key);

private:
	HRESULT InitKeyboard(HINSTANCE _hInst, HWND _hWnd);
};