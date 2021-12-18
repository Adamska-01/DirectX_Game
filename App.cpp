#include "App.h"

App::App()
	:
	wnd(new Window(800, 600, "DirectX_Game"))
{ 
    //Create models
    models[Constants::Models::CUBE] = new ObjFileModel("Assets/Models/cube.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    models[Constants::Models::SPHERE] = new ObjFileModel("Assets/Models/Sphere.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    
    //Create shaders
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::modelVS, Constants::ilModel, ARRAYSIZE(Constants::ilModel));
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::skyboxVS, Constants::ilSkybox, ARRAYSIZE(Constants::ilSkybox));
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::reflectVS, Constants::ilReflect, ARRAYSIZE(Constants::ilReflect));
    PixelShader::GetInstance()->SetShader(wnd->GetGraphics(), Constants::modelPS);
    PixelShader::GetInstance()->SetShader(wnd->GetGraphics(), Constants::skyboxPS);
    PixelShader::GetInstance()->SetShader(wnd->GetGraphics(), Constants::reflectPS);

    //Create textures
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::skyboxTX);
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::crateTX);

    //Create models 
    crate = new Model(wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    crate->LoadObjModel(models[Constants::Models::CUBE], Constants::skyboxVS, Constants::skyboxPS, Constants::skyboxTX);
    crate->Scale(3.0f, 3.0f, 3.0f);

    player = new Camera();
    player->SetPosition(0.0f, 0.0f, -2.0f);

    player->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);
}

App::~App()
{
    if (wnd != nullptr)
    {
        delete wnd;
        wnd = nullptr;
    }
    std::map<Constants::Models, ObjFileModel*>::iterator it;
    for (it = models.begin(); it != models.end(); it++)
    { 
        delete it->second;
        it->second = nullptr;
    }
    VertexShader::GetInstance()->Clean();
    PixelShader::GetInstance()->Clean();
    Textures::GetInstance()->Clean();
    if (pDepthWriteSolid != nullptr) pDepthWriteSolid->Release();
    if (pRasterSolid != nullptr) pRasterSolid->Release();
}

int App::Loop()
{ 
    while (true)
    {
        if (const auto ecode = Window::ProcessMessages())   //Looks for messages and return if QUIT
        {
            return *ecode;
        } 

        Update(); 
    } 
}

void App::Update()
{
    timer.StartClock(); 
    i += 0.1f;
    
    wnd->GetGraphics()->ClearFrame(0.0f, 0.0f, 0.0f);
    player->SetPosition(0.0f, 0.0f, -2.0f);
    //player->AdjustRotation(0.0, 1.0f, 0.0f);
    player->AdjustPosition(player->GetBackwardVector() * 0.1f);
    crate->SetPosition(player->GetPositionVector());
    crate->UpdateConstantBf(player->GetViewMatrix(), player->GetProjetionMatrix(), nullptr, nullptr, nullptr);
    if (pDepthWriteSolid == nullptr && pRasterSolid == nullptr)
    {
        D3D11_DEPTH_STENCIL_DESC dpthStDesc;
        ZeroMemory(&dpthStDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
        dpthStDesc.DepthEnable = true;
        dpthStDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        dpthStDesc.DepthFunc = D3D11_COMPARISON_LESS;
        wnd->GetGraphics()->pDevice->CreateDepthStencilState(&dpthStDesc, &pDepthWriteSolid);
        
        D3D11_RASTERIZER_DESC rdesc;
        ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
        rdesc.FillMode = D3D11_FILL_SOLID;
        rdesc.CullMode = D3D11_CULL_FRONT;
        rdesc.FrontCounterClockwise = false;
        rdesc.DepthBias = false;
        rdesc.DepthBiasClamp = 0;
        rdesc.SlopeScaledDepthBias = 0;
        rdesc.DepthClipEnable = true;
        rdesc.MultisampleEnable = false;
        rdesc.AntialiasedLineEnable = false;
        wnd->GetGraphics()->pDevice->CreateRasterizerState(&rdesc, &pRasterSolid);
    }
    wnd->GetGraphics()->pImmediateContext->OMSetDepthStencilState(pDepthWriteSolid, 0);
    wnd->GetGraphics()->pImmediateContext->RSSetState(pRasterSolid);
    crate->Draw();
    wnd->GetGraphics()->RenderFrame();

    timer.EndClock();
    timer.DelayByFrameTime();
}
