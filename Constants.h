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
	static std::string skyboxTX = "Assets/Textures/HaloSkyBox.dds";
	static std::string crateTX = "Assets/Textures/BoxTexture.bmp";

	//Models 
	static enum class Models
	{
		CUBE, SPHERE
	};
	
	//Input Layouts
	static D3D11_INPUT_ELEMENT_DESC ilModel[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	}; 
	static D3D11_INPUT_ELEMENT_DESC ilReflect[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
} 