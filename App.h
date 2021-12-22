#pragma once
#include "Window.h" 
#include "FrameTimer.h"
#include "objfilemodel.h" 
#include "PixelShader.h"
#include "VertexShader.h"
#include "Textures.h"
#include "Skybox.h"
#include "Map.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
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
	Camera* player;
	Keyboard* keyboard;
	Mouse* mouse;
	ID3D11DepthStencilState* pDepthWriteSolid;
	ID3D11RasterizerState* pRasterSolid;

	FrameTimer timer;
};