#pragma once
#include <dinput.h>
#include <dxerr.h>
 

class Input
{
protected:
	static IDirectInput8* pDirectInput;

public:
	Input() = default;
	Input(HINSTANCE _hInst);
	virtual ~Input() = default;

	virtual HRESULT ReadInputStates() = 0;

private:
	HRESULT initInput(HINSTANCE _hInst);
};