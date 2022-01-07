#pragma once
#include "Input.h"
#include <DirectXMath.h> 
#define XM_NO_INTRINSICS_   //Use less optimal but more compatible code 
#define XM_NO_ALLIGNMENT_


class Mouse : Input
{
private:
	enum class ClickStates
	{
		DOWN, PRESSED, UP, NOTPRESSED
	};
private:
	IDirectInputDevice8* pMouseDevice;
	DIMOUSESTATE mouseStates;
	ClickStates clickStates[4];

public:
	Mouse(HINSTANCE _hInst, HWND _hWnd);
	~Mouse() override;

	HRESULT ReadInputStates() override;

	//Check for mouse input
	bool IsLeftClickDown();
	bool IsLeftClickPressed();
	bool IsLeftClickUp(); 
	bool IsRightClickDown();
	bool IsRightClickPressed();
	bool IsRightClickUp();
	DirectX::XMFLOAT2 GetMouseMovement();

private:
	HRESULT InitMouse(HINSTANCE _hInst, HWND _hWnd);
};