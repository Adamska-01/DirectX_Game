#pragma once
#include "Window.h" 
#include "FrameTimer.h"
#include "objfilemodel.h" 
#include "PixelShader.h"
#include "VertexShader.h"
#include "Textures.h"
#include "Skybox.h"
#include "Map.h"
#include "Player.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Text2D.h"
#include <map>


class App
{
public:
	App();
	~App();

	int Loop();  //Message loop
private:
	void UpdateLogic();
	void UpdateRender();

private:
	//Subsystems
	Window* wnd; int currentWidth, currentHeight;
	std::map<Constants::Models, ObjFileModel*> models;
	Keyboard* keyboard;
	Mouse* mouse; 
	//FPS counter and dt
	FrameTimer timer;
	 
	Player* player;
	Map* map;

	Skybox* skybox;

	//UI
	Text2D* framerateText;
	Text2D* crosshair;
	Text2D* healthText;
	Text2D* cameraDestroyedText;

	//Lights
	AmbientLight* ambientLight;
	DirectionalLight* directionalLight; float i = 0.0f;
	PointLight* pointLight; XMFLOAT3 pointLightStartPos; float defaultValue;   
};