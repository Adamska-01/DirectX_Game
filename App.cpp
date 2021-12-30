#include "App.h"
#include "CollisionHandler.h"

App::App()
	:
	wnd(new Window(800, 600, "DirectX_Game"))
{ 
    //Create models
    models[Constants::Models::CUBE] = new ObjFileModel("Assets/Models/cube.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    models[Constants::Models::SPHERE] = new ObjFileModel("Assets/Models/Sphere.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    models[Constants::Models::CAMERA] = new ObjFileModel("Assets/Models/camera.obj", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    
    //Create shaders
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::modelVS, Constants::ilModel, ARRAYSIZE(Constants::ilModel));
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::skyboxVS, Constants::ilModel, ARRAYSIZE(Constants::ilModel));
    VertexShader::GetInstance()->SetShaderAndIL(wnd->GetGraphics(), Constants::reflectVS, Constants::ilModel, ARRAYSIZE(Constants::ilModel));
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
    Textures::GetInstance()->SetTexture(wnd->GetGraphics(), Constants::wallTX);

    //Create models 
    skybox = new Skybox(wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    skybox->LoadObjModel(models[Constants::Models::CUBE], Constants::skyboxVS, Constants::skyboxPS, Constants::skyboxTX);
    skybox->Scale(3.0f, 3.0f, 3.0f);

    //Init input
    keyboard = new Keyboard(wnd->GetHINST(), wnd->GetHWND());
    mouse = new Mouse(wnd->GetHINST(), wnd->GetHWND());
    
    
    //Load map
    map = new Map("Assets/Map.txt", wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);

    player = new Player(map, keyboard, mouse, wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext, models[Constants::Models::SPHERE]);
    player->GetCamera()->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);
    
    map->LoadMap(models, player);

    framerateText = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    crosshair = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    health = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    cameraDestroyed = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);

    //Lights
    ambientLight = new AmbientLight();
    ambientLight->SetColour(0.3f, 0.3f, 0.3f, 1.0f); //Grey;
    directionalLight = new DirectionalLight();
    directionalLight->SetColour(0.98f, 0.84f, 0.11f, 0.0f); //Sun colour
    directionalLight->SetDirection(0.5f, 0.5f, -1.0f);
    pointLight = new PointLight();
    pointLight->SetColour(0.0f, 15.0f, 0.0f, 1.0f);
    pointLight->SetAttenuation(XMFLOAT3(0.0f, 1.0f, 0.0f));
    pointLight->SetRange(5.0f);
    pointLight->SetPosition(player->GetCamera()->GetPositionFloat3().x, player->GetCamera()->GetPositionFloat3().y, player->GetCamera()->GetPositionFloat3().z + 40.0f);
    pointLightStartPos = pointLight->GetPositionFloat3();
    defaultValue = 50.0f;
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
    player->UpdateLogic(FrameTimer::DeltaTime());
    map->UpdateLogic(FrameTimer::DeltaTime(), player);
       
    skybox->SetPosition(player->GetCamera()->GetPositionVector());

    wnd->GetGraphics()->RenderFrame(); 
}

void App::UpdateRender()
{
    i += 10.0f * FrameTimer::DeltaTime();
    if (i > 360.0f)
        i = 0.0f;
    directionalLight->SetRotation(i, 0.0f, 0.0f);


    XMFLOAT3 newPos = pointLightStartPos;
    newPos.x = sin(FrameTimer::Time() / 1000.0f);
    pointLight->SetPosition(newPos.x *defaultValue + pointLightStartPos.x, newPos.y, newPos.z);

    wnd->GetGraphics()->ClearFrame(0.0f, 0.0f, 0.0f);
    //-------Draw here-------

    //Skybox
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->pRasterSkyBox);
    wnd->GetGraphics()->pImmediateContext->OMSetDepthStencilState(wnd->GetGraphics()->pDepthWriteSkyBox, 0);
    skybox->UpdateConstantBF(player->GetCamera()->GetViewMatrix(), player->GetCamera()->GetProjetionMatrix());
    skybox->Draw();
    wnd->GetGraphics()->pImmediateContext->OMSetDepthStencilState(wnd->GetGraphics()->pDepthWriteSolid, 0);
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->pRasterSolid);
    
    //Render map and all objects inside 
    map->Draw(player->GetCamera()->GetViewMatrix(), player->GetCamera()->GetProjetionMatrix(), ambientLight, directionalLight, pointLight); 
    player->Draw(player->GetCamera()->GetViewMatrix(), player->GetCamera()->GetProjetionMatrix());

    //Render Text  
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->rastStateCullNone);
    wnd->GetGraphics()->pImmediateContext->OMSetBlendState(wnd->GetGraphics()->pAlphaBlendEnable, 0, 0xffffffff);
    framerateText->AddText("FPS " + std::to_string(FrameTimer::Frames()), -1.0f, 1.0f, 0.05f);
    framerateText->RenderText();                    

    crosshair->AddText(" ", -0.04f, 0.04f, 0.08f);
    crosshair->RenderText();        

    health->AddText("Health " + std::to_string(player->GetHealth()), -1.0f, -0.8f, 0.09f);
    health->RenderText();

    cameraDestroyed->AddText("Cameras to destroy " + std::to_string(map->GetCameras().size()), -0.3f, 1.0f, 0.06f);
    cameraDestroyed->RenderText();                    
    wnd->GetGraphics()->pImmediateContext->OMSetBlendState(wnd->GetGraphics()->pAlphaBlendDisable, 0, 0xffffffff);


    //------Finish draw------ 
    wnd->GetGraphics()->RenderFrame();

    timer.EndClock();
    timer.DelayByFrameTime();
}