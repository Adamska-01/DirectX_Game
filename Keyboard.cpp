#include "Keyboard.h"
#include <array>


Keyboard::Keyboard(HINSTANCE _hInst, HWND _hWnd)
    :
    Input(_hInst)
{
    if (FAILED(InitKeyboard(_hInst, _hWnd)))
    {
        DXTRACE_MSG("Failed to create input device");
        return;
    } 
}

Keyboard::~Keyboard()
{
    if (pKeyboardDevice != nullptr) pKeyboardDevice->Release();
    if (pDirectInput != nullptr)
    {
        pDirectInput->Release();
        pDirectInput = nullptr;
    }
}

HRESULT Keyboard::ReadInputStates()
{
    HRESULT hr;

    unsigned char prevStates[256];
    std::copy(std::begin(keyboardStates), std::end(keyboardStates), std::begin(prevStates));

    hr = pKeyboardDevice->GetDeviceState(sizeof(keyboardStates), (LPVOID)&keyboardStates);

    //Sometimes the app window loses focus, so reaquire the device;
    if (FAILED(hr))
    {
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
        {
            pKeyboardDevice->Acquire();
        }
    }

    //Assign key states
    int length = sizeof(keyboardStates) / sizeof(keyboardStates[0]);
    for (int i = 0; i < length; i++)
    {
        if (keyboardStates[i] && !prevStates[i])
            keystates[i] = keyStates::DOWN;
        else if (keyboardStates[i] && prevStates[i])
            keystates[i] = keyStates::PRESSED;
        else if (!keyboardStates[i] && prevStates[i])
            keystates[i] = keyStates::UP;
        else
            keystates[i] = keyStates::NOTPRESSED;
    }

    return hr;
}

bool Keyboard::IsKeyDown(unsigned char _key)
{
    return keystates[_key] == keyStates::DOWN;
}

bool Keyboard::IsKeyPressed(unsigned char _key)
{
    return keystates[_key] == keyStates::PRESSED;
}

bool Keyboard::IsKeyUp(unsigned char _key)
{
    return keystates[_key] == keyStates::UP;
}

HRESULT Keyboard::InitKeyboard(HINSTANCE _hInst, HWND _hWnd) 
{  
    HRESULT hr = S_OK;

    ZeroMemory(keyboardStates, sizeof(keyboardStates));

    //Create device 
    hr = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboardDevice, NULL);
    if (FAILED(hr)) return hr;

    //Tell DirectInput how to interpret the device (keyboard)
    hr = pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr)) return hr;

    //Tell device how ti interact with other Apps  (DISCL: only if window is active, and app can share the device with other apps)
    hr = pKeyboardDevice->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) return hr;

    //Ask the device to start providing input for the App
    hr = pKeyboardDevice->Acquire();
    if (FAILED(hr)) return hr;

    return hr;
}
