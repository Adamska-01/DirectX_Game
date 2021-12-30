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
#include "Keyboard.h"
#include "Mouse.h"
#include "Text2D.h"
#include <map>


class App
{
public:
	App();
	~App();

	//Message loop
	int Loop();  
private:
	void UpdateLogic();
	void UpdateRender();

private:
	Window* wnd; 
	float i = 0.0f;

	std::map<Constants::Models, ObjFileModel*> models;
	Skybox* skybox;
	Map* map;
	Player* player;

	Text2D* framerateText;
	Text2D* crosshair;
	Text2D* health;
	Text2D* cameraDestroyed;

	//Lights
	AmbientLight* ambientLight;
	DirectionalLight* directionalLight;

	Keyboard* keyboard;
	Mouse* mouse;

	FrameTimer timer;
};