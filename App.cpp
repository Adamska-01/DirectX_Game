#include "App.h"
#include "CollisionHandler.h"
#include "GameManager.h"

App::App()
	:
	wnd(new Window(1280, 720, "DirectX_Game")),
    currentWidth(wnd->GetWidth()),
    currentHeight(wnd->GetHeight())
{ 
    //Create models
    models[Constants::Models::CUBE] = new ObjFileModel(Constants::cube, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    models[Constants::Models::SPHERE] = new ObjFileModel(Constants::sphere, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    models[Constants::Models::CAMERA] = new ObjFileModel(Constants::camera, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    
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
    
    //Game stuff
    map = new Map("Assets/Map.txt", wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);

    player = new Player(map, keyboard, mouse, wnd->GetGraphics(), wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext, models[Constants::Models::SPHERE]);
    player->GetCamera()->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);
    
    //Load map
    map->LoadMap(models, player);

    //UI
    framerateText = new Text2D(" ", wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    crosshair = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    healthText = new Text2D(Constants::font1, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    cameraDestroyedText = new Text2D(Constants::font2, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);
    victoryText = new Text2D(Constants::font2, wnd->GetGraphics()->pDevice, wnd->GetGraphics()->pImmediateContext);

    //Lights
    ambientLight = new AmbientLight();
    ambientLight->SetColour(0.3f, 0.3f, 0.3f, 1.0f); //Grey;
    directionalLight = new DirectionalLight();
    directionalLight->SetColour(0.84f, 0.84f, 0.82f, 0.0f); //Sun colour
    directionalLight->SetDirection(-0.5f, -0.5f, 1.0f);
    pointLight = new PointLight();
    pointLight->SetColour(0.0f, 5.0f, 0.0f, 1.0f);
    pointLight->SetAttenuation(XMFLOAT3(0.0f, 1.0f, 0.0f));
    pointLight->SetRange(5.0f);
    pointLight->SetPosition(player->GetCamera()->GetPositionFloat3().x, player->GetCamera()->GetPositionFloat3().y, player->GetCamera()->GetPositionFloat3().z + 40.0f);
    pointLightStartPos = pointLight->GetPositionFloat3();
    defaultValue = 50.0f;

    //Determines vicctory state
    GameManager::GameManager(map->GetCameras().size());
}

App::~App()
{ 
    //UI
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
    if (healthText != nullptr)
    {
        delete healthText;
        healthText = nullptr;
    }
    if (cameraDestroyedText != nullptr)
    {
        delete cameraDestroyedText;
        cameraDestroyedText = nullptr;
    }
    if (victoryText != nullptr)
    {
        delete victoryText;
        victoryText = nullptr;
    }
    //Player
    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }
    //Skybox
    if (skybox != nullptr)
    {
        delete skybox;
        skybox = nullptr;
    }
    //Map
    if (map != nullptr)
    {
        delete map;
        map = nullptr;
    }
    //Lights
    if (ambientLight != nullptr)
    {
        delete ambientLight;
        ambientLight = nullptr;
    }
    if (directionalLight != nullptr)
    {
        delete directionalLight;
        directionalLight = nullptr;
    }
    if (pointLight != nullptr)
    {
        delete pointLight;
        pointLight = nullptr;
    }
    //Models
    std::map<Constants::Models, ObjFileModel*>::iterator it;
    for (it = models.begin(); it != models.end(); it++)
    { 
        delete it->second;
        it->second = nullptr;
    }
    //Bindables
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
        //Looks for messages and return optional if QUIT
        if (const auto ecode = Window::ProcessMessages())   
        {
            return *ecode;
        } 

        //Game loop
        UpdateLogic(); 
        UpdateRender();


        if (EndLoop()) //Exit Game
        {
            PostQuitMessage(0);
            return 0;
        }
    } 
}


void App::UpdateLogic()
{
    //Start timer
    timer.StartClock(); 

    //Update input
    keyboard->ReadInputStates();
    mouse->ReadInputStates(); 

    //Set projection values 
    if (currentWidth != wnd->GetWidth() || currentHeight != wnd->GetHeight())
    {
        player->GetCamera()->SetProjectionValues(90.0f, static_cast<float>(wnd->GetWidth()) / static_cast<float>(wnd->GetHeight()), 0.01f, 1000.0f);

        currentWidth = wnd->GetWidth();
        currentHeight = wnd->GetHeight();
    }

    GameManager::SetCamerasNumber(map->GetCameras().size());
    
    //------------------------------UPDATE ENTITIES------------------------------ 
    if (!GameManager::HasWon())
    {
        player->UpdateLogic(FrameTimer::DeltaTime());
        skybox->SetPosition(player->GetCamera()->GetPositionVector());

        map->UpdateLogic(FrameTimer::DeltaTime(), player); 
    }
}

void App::UpdateRender()
{
    //Rotate directional light
    i += 6.0f * FrameTimer::DeltaTime();
    if (i > 360.0f)
        i = 0.0f;
    directionalLight->SetRotation(i, 0.0f, i);

    //Move point light around
    XMFLOAT3 newPos = pointLightStartPos;
    newPos.x = sin(FrameTimer::Time() / 1000.0f);
    pointLight->SetPosition(newPos.x *defaultValue + pointLightStartPos.x, newPos.y, newPos.z);

    wnd->GetGraphics()->ClearFrame(0.0f, 0.0f, 0.0f);
    //---------------------------------RENDER HERE---------------------------------  

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

    //Render UI  
    wnd->GetGraphics()->pImmediateContext->RSSetState(wnd->GetGraphics()->rastStateCullNone);
    wnd->GetGraphics()->pImmediateContext->OMSetBlendState(wnd->GetGraphics()->pAlphaBlendEnable, 0, 0xffffffff);
    
    framerateText->AddText("FPS: " + std::to_string(FrameTimer::Frames()), -1.0f, 1.0f, 0.05f);
    framerateText->RenderText();                    

    crosshair->AddText(".", -0.0413f, 0.066f, 0.08f);
    crosshair->RenderText();        

    healthText->AddText("Health: " + std::to_string(player->GetHealth()).substr(0, std::to_string(player->GetHealth()).find(".") + 3) + "%", -1.0f, -0.9f, 0.08f);
    healthText->RenderText();

    cameraDestroyedText->AddText("Destroyed cameras: " + std::to_string(3 - GameManager::GetCamerasNumber()) + "/3", -0.315f, 1.0f, 0.06f);
    cameraDestroyedText->RenderText();                    
    
    //Victory Text
    if (GameManager::HasWon())
    {
        victoryText->AddText("You Have Won!", -0.6f, 0.06f, 0.1f);
        victoryText->AddText("(Press ESC to exit)", -0.95f, -0.05f, 0.1f);
        victoryText->RenderText(); 
    }

    wnd->GetGraphics()->pImmediateContext->OMSetBlendState(wnd->GetGraphics()->pAlphaBlendDisable, 0, 0xffffffff);
     
    //---------------------------------END RENDER---------------------------------  
    wnd->GetGraphics()->RenderFrame();

    //Update timer
    timer.EndClock();
    timer.DelayByFrameTime();
}

std::optional<int> App::EndLoop()
{
    if (GameManager::HasWon() && keyboard->IsKeyDown(DIK_ESCAPE))
    {
        return WM_QUIT;
    }

    //return an empty optional if not quiting 
    return {};
}
