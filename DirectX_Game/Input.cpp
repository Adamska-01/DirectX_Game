#include "Input.h"

IDirectInput8* Input::pDirectInput = nullptr;

Input::Input(HINSTANCE _hInst)
{
    if(pDirectInput == nullptr)
        initInput(_hInst);
}
  
HRESULT Input::initInput(HINSTANCE _hInst)
{
    HRESULT hr = S_OK;

    //Create DirectInput object 
    hr = DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL);
    if (FAILED(hr)) return hr;

    return hr;
}
