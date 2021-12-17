#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
{
    //Create swap chain 
    if (FAILED(CreateSwapChain(hWnd)))
    {
        DXTRACE_MSG("Failed to create Swap chain");
        return;
    }

    //Create back buffer
    if (FAILED(CreateBackBuffer()))
    {
        DXTRACE_MSG("Failed to create back buffer");
        return;
    }
}

Graphics::~Graphics()
{
    if (pImmediateContext != nullptr) pImmediateContext->Release();
    if (pSwapChain != nullptr) pSwapChain->Release();
    if (pBackBufferRTView != nullptr) pBackBufferRTView->Release();
    if (pDevice != nullptr) pDevice->Release();
}

void Graphics::ClearFrame(float _r, float _g, float _b)
{
    const float colour[] = { _r, _g, _b, 1.0f };
    pImmediateContext->ClearRenderTargetView(pBackBufferRTView, colour);
}

void Graphics::RenderFrame()
{
    pSwapChain->Present(0, 0);
}

HRESULT Graphics::CreateSwapChain(HWND hWnd)
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG   //Enables the devices debugging capabilities
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    //Possible driver types
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,   //Comment to test D3D 11.0 on hardware that doesn't support it 
        D3D_DRIVER_TYPE_WARP,       //Comment to use reference device
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    //Swap chain descriptor
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));    //Is the standard DirectX way of initialising structures. 
    sd.BufferCount = 1;     //Number of back buffers in the swap chain
    sd.BufferDesc.Width = width;    //width and height of the renderable area calculated earlier
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //Format of the buffers
    sd.BufferDesc.RefreshRate.Numerator = 0;   //Refresh rate (in this case 60/1)
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //Use the surface or resource as an output render target
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;    //No antialiasing
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    //This loop goes through each of the specified devices and attempts to create an
    //associated swap chain based on provided feature levels and the swap chain description
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,   //Graphics adapter (NULL == default)
            driverType,     //Drivers to create
            NULL,
            createDeviceFlags,
            featureLevels,
            numFeatureLevels,
            D3D11_SDK_VERSION,  //Always use this for DX apps
            &sd,        //Poiter to the swap chain descriptor
            &pSwapChain,    //pp to the swap chain object (used to access the swap chain)
            &pDevice,    //pp to the device object. represents the physical hardware
            &featureLevel,  //Flag of the highest feature level that was found During device creation
            &pImmediateContext   //pp to the context object. represents the portion of the device that handles graphics rendering 
        );

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr)) return hr;

    return hr;
}

HRESULT Graphics::CreateBackBuffer()
{
    HRESULT hr = S_OK;

    ID3D11Resource* pBackBuffer = nullptr;
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;
    
    //Use back buffer texture pointer to create the renedr target view
    hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pBackBufferRTView);
    if (FAILED(hr)) return hr;

    //Clean up
    pBackBuffer->Release();

    return hr;
}
