#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <dxerr.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	void ClearFrame(float _r, float _g, float _b);
	void RenderFrame();

private:
	HRESULT CreateSwapChain(HWND hWnd);
	HRESULT CreateBackBuffer();

private: 
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr;
	ID3D11RenderTargetView* pBackBufferRTView = nullptr;

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
};