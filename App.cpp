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
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::floorTX);
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::floor2TX);
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::enemyTX);
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::goldTX);

    //Create models 
    skybox = new Skybox(wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    skybox->LoadObjModel(models[Constants::Models::CUBE], Constants::skyboxVS, Constants::skyboxPS, Constants::skyboxTX);
    skybox->Scale(3.0f, 3.0f, 3.0f);

    //Init input
    keyboard = new Keyboard(wnd->GetHINST(), wnd->GetHWND());
    mouse = new Mouse(wnd->GetHINST(), wnd->GetHWND());
    
    //Load map
    map = new Map("Assets/Map.txt", wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    map->LoadMap(models);
    
    player = new Player(map, keyboard, mouse); 
    player->GetCamera()->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

    framerateText = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    crosshair = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    health = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    cameraDestroyed = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
}

App::~App()
{
    if (map != nullptr)
    {
        delete map;
        map = nullptr;
    }
    if (framerateText != nullptr)
    {
        delete framerateText;
        framerateText = nullptr;
    }
    if (crosshair != nullptr)
    {
        delete crosshair;
        crosshair = nullptr;
    }
    if (health != nullptr)
    {
        delete health;
        health = nullptr;
    }
    if (cameraDestroyed != nullptr)
    {
        delete cameraDestroyed;
        cameraDestroyed = nullptr;
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
    player->GetCamera()->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

    //Move Camera
    float dt = 1.0f;
    player->UpdateLogic();
    map->UpdateLogic(FrameTimer::DeltaTime(), player);
       
    skybox->SetPosition(player->GetCamera()->GetPositionVector());
    skybox->UpdateConstantBF(player->GetCamera()->GetViewMatrix(), player->GetCamera()->GetProjetionMatrix());
     
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
    
    //Render map and all objects inside 
    map->Draw(player->GetCamera()->GetViewMatrix(), player->GetCamera()->GetProjetionMatrix());
    
    //Render Text  
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->rastStateCullNone);
    wnd->GetGraphics()->pImmediateContext->OMSetBlendState(wnd->GetGraphics()->pAlphaBlendEnable, 0, 0xffffffff);
    framerateText->AddText("FPS " + std::to_string(FrameTimer::Frames()), -1.0f, 1.0f, 0.05f);
    framerateText->RenderText();                    

    crosshair->AddText(" ", 0.0f, 0.0f, 0.08f);
    crosshair->RenderText();        

    health->AddText("Health " + std::to_string(player->GetHealth()), -1.0f, -0.8f, 0.09f);
    health->RenderText();

    cameraDestroyed->AddText("Cameras to destroy " + std::to_string(player->GetHealth()), -0.3f, 1.0f, 0.06f);
    cameraDestroyed->RenderText();                    
    wnd->GetGraphics()->pImmediateContext->OMSetBlendState(wnd->GetGraphics()->pAlphaBlendDisable, 0, 0xffffffff);


    //------Finish draw------ 
    wnd->GetGraphics()->RenderFrame();

    timer.EndClock();
    timer.DelayByFrameTime();
}