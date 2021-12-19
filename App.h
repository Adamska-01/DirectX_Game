#pragma once
#include "Window.h" 
#include "FrameTimer.h"
#include "objfilemodel.h"
#include "Constants.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Textures.h"
#include "Model.h"
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
	void Update();

private:
	Window* wnd; 
	float i = 0.0f;

	std::map<Constants::Models, ObjFileModel*> models;
	Model* crate;
	Camera* player;
	Keyboard* keyboard;
	Mouse* mouse;
	ID3D11DepthStencilState* pDepthWriteSolid;
	ID3D11RasterizerState* pRasterSolid;

	FrameTimer timer;
};