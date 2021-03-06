#pragma once
#include <string> 

namespace Constants
{
	//Vertex shaders
	static std::string modelVS = "ModelVS";
	static std::string skyboxVS = "SkyBoxVS";
	static std::string reflectVS = "ReflectVS";

	//Pixel shaders 
	static std::string modelPS = "ModelPS";
	static std::string skyboxPS = "SkyBoxPS";
	static std::string reflectPS = "ReflectPS";

	//Textures
	static std::string skyboxTX = "Assets/Textures/City.dds";
	static std::string crateTX = "Assets/Textures/BoxTexture.bmp";
	static std::string floorTX = "Assets/Textures/Floor.tif";
	static std::string floor2TX = "Assets/Textures/Floor2.tif";
	static std::string enemyTX = "Assets/Textures/enemy.png";
	static std::string goldTX = "Assets/Textures/Gold.tif";
	static std::string wallTX = "Assets/Textures/wall.tif";

	//Fonts
	static std::string font1 = "Assets/Textures/font1.png";
	static std::string font2 = "Assets/Textures/font1.bmp";

	//Models 
	static enum class Models
	{
		CUBE, SPHERE, CAMERA
	};
	//Models paths
	static std::string cube = "Assets/Models/cube.obj";
	static std::string sphere = "Assets/Models/Sphere.obj";
	static std::string camera = "Assets/Models/camera.obj";
	
	//Input Layouts
	static D3D11_INPUT_ELEMENT_DESC ilModel[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};  
} 
 