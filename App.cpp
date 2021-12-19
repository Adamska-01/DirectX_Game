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
    crate->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::crateTX);
    //crate->Scale(3.0f, 3.0f, 3.0f);

    player = new Camera();
    player->SetPosition(0.0f, 0.0f, -2.0f);

    int a = wnd->GetWidth();
    int b = wnd->GetHeight();
    player->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

    //Init input
    keyboard = new Keyboard(wnd->GetHINST(), wnd->GetHWND());
    mouse = new Mouse(wnd->GetHINST(), wnd->GetHWND());
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

    //Release Mouse before Keyboard (keybord releases IDirectInput8*)
    if (mouse != nullptr)
    {
        delete mouse;
        mouse = nullptr;
    }
    if (keyboard != nullptr)
    {
        delete keyboard;
        keyboard = nullptr;
    }
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
    keyboard->ReadInputStates();
    mouse->ReadInputStates();

    timer.StartClock(); 
    i += 0.1f;
    float c = sin(i) / 2 + 0.5f;
    

    //Move Camera
    float dt = 1.0f;
    if (keyboard->IsKeyPressed(DIK_W))
        player->AdjustPosition(player->GetForwardVector() * dt);
    if (keyboard->IsKeyPressed(DIK_A))
        player->AdjustPosition(player->GetLeftVector() * dt);
    if (keyboard->IsKeyPressed(DIK_S))
        player->AdjustPosition(player->GetBackwardVector() * dt);
    if (keyboard->IsKeyPressed(DIK_D))
        player->AdjustPosition(player->GetRightVector() * dt);
    if (keyboard->IsKeyPressed(DIK_SPACE))
        player->AdjustPosition(0.0f, dt, 0.0f);
    if (keyboard->IsKeyPressed(DIK_Z))
        player->AdjustPosition(0.0f, -dt, 0.0f);

    //Camera rotation
    if (mouse->IsRightClickPressed())
    {
        XMFLOAT2 movement = mouse->GetMouseMovement();
        player->AdjustRotation(0.01f * movement.x, 0.01f * movement.y, 0.0f);
    }

    wnd->GetGraphics()->ClearFrame(c, c, 0.0f); 
    player->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

    crate->SetPosition(0.0f, 0.0f, 8.0f);
    crate->Rotate(0.0, 0.1f, 0.0f); 
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
        rdesc.CullMode = D3D11_CULL_BACK;
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
