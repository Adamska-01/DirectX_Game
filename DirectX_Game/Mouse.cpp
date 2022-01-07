#include "Mouse.h"

Mouse::Mouse(HINSTANCE _hInst, HWND _hWnd)
{
    if (FAILED(InitMouse(_hInst, _hWnd)))
    {
        DXTRACE_MSG("Failed to create input device");
        return;
    }
}

Mouse::~Mouse()
{
    if (pMouseDevice != nullptr) pMouseDevice->Release();
}

HRESULT Mouse::ReadInputStates()
{
    HRESULT hr = S_OK;

    bool prevStates[] = { (bool)mouseStates.rgbButtons[0], (bool)mouseStates.rgbButtons[1] };

    hr = pMouseDevice->GetDeviceState(sizeof(mouseStates), (LPVOID)&mouseStates);

    //Sometimes the app window loses focus, so reaquire the device;
    if (FAILED(hr))
    {
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
        {
            pMouseDevice->Acquire();
        }
    }

    //Assign key states
    int length = sizeof(clickStates) / sizeof(clickStates[0]);
    for (int i = 0; i < length; i++)
    {
        if (mouseStates.rgbButtons[i] && !prevStates[i])
            clickStates[i] = ClickStates::DOWN;
        else if (mouseStates.rgbButtons[i] && prevStates[i])
            clickStates[i] = ClickStates::PRESSED;
        else if (!mouseStates.rgbButtons[i] && prevStates[i])
            clickStates[i] = ClickStates::UP;
        else
            clickStates[i] = ClickStates::NOTPRESSED;
    }

    return hr;
}

bool Mouse::IsLeftClickDown()
{
    return clickStates[0] == ClickStates::DOWN;
}

bool Mouse::IsLeftClickPressed()
{
    return clickStates[0] == ClickStates::PRESSED;
}

bool Mouse::IsLeftClickUp()
{
    return clickStates[0] == ClickStates::UP;
}

bool Mouse::IsRightClickDown()
{
    return clickStates[1] == ClickStates::DOWN;
}

bool Mouse::IsRightClickPressed()
{
    return clickStates[1] == ClickStates::PRESSED;
}

bool Mouse::IsRightClickUp()
{
    return clickStates[1] == ClickStates::UP;
}

DirectX::XMFLOAT2 Mouse::GetMouseMovement()
{
    return DirectX::XMFLOAT2((float)mouseStates.lY, (float)mouseStates.lX);
}

HRESULT Mouse::InitMouse(HINSTANCE _hInst, HWND _hWnd)
{
    HRESULT hr = S_OK;

    //Create device 
    hr = pDirectInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);
    if (FAILED(hr)) return hr;

    //Tell DirectInput how to interpret the device (keyboard)
    hr = pMouseDevice->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr)) return hr;

    //Tell device how ti interact with other Apps  (DISCL: only if window is active, and app can share the device with other apps)
    hr = pMouseDevice->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if (FAILED(hr)) return hr;

    //Ask the device to start providing input for the App
    hr = pMouseDevice->Acquire();
    if (FAILED(hr)) return hr;

    return hr;
}
