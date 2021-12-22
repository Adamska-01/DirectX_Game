#include "App.h"
#include "CollisionHandler.h"

App::App()
	:
	wnd(new Window(800, 600, "DirectX_Game"))
{ 
    //Create models
    models[Constants::Models::CUBE] = new ObjFileModel("Assets/Models/cube.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    models[Constants::Models::SPHERE] = new ObjFileModel("Assets/Models/Sphere.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    
    //Create shaders
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::modelVS, Constants::ilModel, ARRAYSIZE(Constants::ilModel));
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::skyboxVS, Constants::ilModel, ARRAYSIZE(Constants::ilModel));
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::reflectVS, Constants::ilReflect, ARRAYSIZE(Constants::ilReflect));
    PixelShader::GetInstance()->SetShader(wnd->GetGraphics(), Constants::modelPS);
    PixelShader::GetInstance()->SetShader(wnd->GetGraphics(), Constants::skyboxPS);
    PixelShader::GetInstance()->SetShader(wnd->GetGraphics(), Constants::reflectPS);

    //Create textures
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::skyboxTX);
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::crateTX);

    //Create models 
    skybox = new Skybox(wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    skybox->LoadObjModel(models[Constants::Models::CUBE], Constants::skyboxVS, Constants::skyboxPS, Constants::skyboxTX);
    skybox->Scale(3.0f, 3.0f, 3.0f);

    player = new Camera();
    player->SetPosition(0.0f, 0.0f, -2.0f);
     
    player->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

    //Load map
    map = new Map("Assets/Map.txt", wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    map->LoadMap(models);

    //Init input
    keyboard = new Keyboard(wnd->GetHINST(), wnd->GetHWND());
    mouse = new Mouse(wnd->GetHINST(), wnd->GetHWND());
}

App::~App()
{
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
    if (wnd != nullptr)
    {
        delete wnd;
        wnd = nullptr;
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

        UpdateLogic(); 
        UpdateRender();
    } 
}


void App::UpdateLogic()
{
    timer.StartClock(); 

    keyboard->ReadInputStates();
    mouse->ReadInputStates(); 
    player->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

    //Move Camera
    float dt = 1.0f;
    if (keyboard->IsKeyPressed(DIK_W))
    {
        player->AdjustPosition(player->GetForwardVector() * dt); 
    }
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
       
    skybox->SetPosition(player->GetPositionVector());
    skybox->UpdateConstantBF(player->GetViewMatrix(), player->GetProjetionMatrix());
     
    wnd->GetGraphics()->RenderFrame(); 
}

void App::UpdateRender()
{
    i += 0.1f;
    float c = sin(i) / 2 + 0.5f;
    wnd->GetGraphics()->ClearFrame(c, c, 0.0f);
    //-------Draw here-------

    //Skybox
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->pRasterSkyBox);
    wnd->GetGraphics()->pImmediateContext->OMSetDepthStencilState(wnd->GetGraphics()->pDepthWriteSkyBox, 0);
    skybox->Draw();
    wnd->GetGraphics()->pImmediateContext->OMSetDepthStencilState(wnd->GetGraphics()->pDepthWriteSolid, 0);
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->pRasterSolid);
    map->Draw(player->GetViewMatrix(), player->GetProjetionMatrix());

    //------Finish draw------ 
    wnd->GetGraphics()->RenderFrame();

    timer.EndClock();
    timer.DelayByFrameTime();
}