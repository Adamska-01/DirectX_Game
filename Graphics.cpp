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

    //Create Z buffer
    if (FAILED(CreateZBuffer(hWnd)))
    {
        DXTRACE_MSG("Failed to create Z buffer");
        return;
    }

    //Create Blend states
    if (FAILED(CreateBlendStates()))
    {
        DXTRACE_MSG("Failed to create Blend states");
        return;
    }
    
    //Create ViewPort
    CreateViewPort(hWnd);
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
    //Clear back buffer
    const float colour[] = { _r, _g, _b, 1.0f };
    pImmediateContext->ClearRenderTargetView(pBackBufferRTView, colour);

    //Clear the Z buffer
    pImmediateContext->ClearDepthStencilView(pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

HRESULT Graphics::CreateZBuffer(HWND hWnd)
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    //Create a Z buffer texture 
    D3D11_TEXTURE2D_DESC tex2DDesc;
    ZeroMemory(&tex2DDesc, sizeof(tex2DDesc));

    DXGI_SWAP_CHAIN_DESC sd;
    pSwapChain->GetDesc(&sd); //Get antialiasing informations
    tex2DDesc.Width = width;
    tex2DDesc.Height = height;
    tex2DDesc.ArraySize = 1;
    tex2DDesc.MipLevels = 1;
    tex2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24 bits of each pixel 
    tex2DDesc.SampleDesc.Count = sd.SampleDesc.Count;
    tex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex2DDesc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Texture2D* pZBufferTexture;
    hr = pDevice->CreateTexture2D(&tex2DDesc, NULL, &pZBufferTexture); 
    if (FAILED(hr)) return hr;

    //Create the Z buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));

    dsvDesc.Format = tex2DDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = pDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &pZBuffer);
    if (FAILED(hr)) return hr;
    pZBufferTexture->Release();

    //Set the render target view 
    pImmediateContext->OMSetRenderTargets(1, &pBackBufferRTView, pZBuffer);

    return hr;
}

HRESULT Graphics::CreateBlendStates()
{
    HRESULT hr = S_OK;

    D3D11_BLEND_DESC bsdesc;
    bsdesc.RenderTarget[0].BlendEnable = TRUE;
    bsdesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bsdesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsdesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsdesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsdesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsdesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bsdesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.IndependentBlendEnable = FALSE;
    bsdesc.AlphaToCoverageEnable = FALSE;
    hr = pDevice->CreateBlendState(&bsdesc, &pAlphaBlendEnable);

    return hr;
}

void Graphics::CreateViewPort(HWND hWnd)
{
    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    //Set the viewport desc
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = .0f;
    viewport.MaxDepth = 1.0f;

    pImmediateContext->RSSetViewports(1, &viewport);
}
