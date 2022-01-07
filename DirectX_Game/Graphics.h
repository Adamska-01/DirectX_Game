#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <dxerr.h>

class Graphics
{
	friend class Bindable;
	friend class Window;
	friend class App;

public:
	Graphics(HWND hWnd);
	~Graphics();

	void ClearFrame(float _r, float _g, float _b);
	void RenderFrame(); 

private: //Subsystems creation
	HRESULT CreateSwapChain(HWND hWnd);
	HRESULT CreateBackBuffer();
	HRESULT CreateZBuffer(HWND hWnd); 
	void CreateViewPort(HWND hWnd);
private: //Blend and stencil states 
	HRESULT CreateBlendStates();
	HRESULT CreateDepthStencilStates();
	HRESULT CreateRasterizerStates();

private: 
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr;
	ID3D11RenderTargetView* pBackBufferRTView = nullptr;
	ID3D11DepthStencilView* pZBuffer = nullptr;

	//Blend states 
	ID3D11BlendState* pAlphaBlendEnable;
	ID3D11BlendState* pAlphaBlendDisable;
	//Depth stencil states 
	ID3D11DepthStencilState* pDepthWriteSkyBox;
	ID3D11DepthStencilState* pDepthWriteSolid;
	//Rasterier states 
	ID3D11RasterizerState* pRasterSkyBox;
	ID3D11RasterizerState* pRasterSolid;
	ID3D11RasterizerState* rastStateCullNone;
private:
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
};