#pragma once
#include "objfilemodel.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Textures.h"
#include "Graphics.h"	


class Model
{
private:
	struct MODEL_CONSTANT_BUFFER
	{
		XMMATRIX worldViewProjection;	 //64 bytes
		XMMATRIX worldView;				 //64 bytes
		XMVECTOR colourModifier;		 //16 bytes
		XMVECTOR directionalLightVector; //16 bytes
		XMVECTOR directionalLightColour; //16 bytes
		XMVECTOR ambientLightColour;     //16 bytes
		XMVECTOR pointLightPos;          //16 bytes
		XMVECTOR pointLightColour;       //16 bytes

		XMFLOAT3 pointLightAttenuation;  //12 bytes
		float range;                     //4 bytes
	}; //Total size = 2 bytes  
private:
	ID3D11Device* pD3DDevice = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr; 
	ID3D11Buffer* pConstantBuffer = nullptr;
	Graphics* gfx;

	ObjFileModel* pObject = nullptr;

	std::string vsShader, psShader, texture;  
private: 
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale; 

public:
	Model(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~Model();

	HRESULT LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);
	void UpdateConstantBf(XMMATRIX _view, XMMATRIX _projection, XMVECTOR _pos, XMVECTOR _rot, XMVECTOR _scale, XMVECTOR _clMod, AmbientLight* _ambLight = nullptr, DirectionalLight* _dirLight = nullptr, PointLight* _pointLight = nullptr);
	void Draw();
private:
	void SetTexture(std::string _texture);
	void SetShaders(std::string _vs, std::string _ps);
public:
	ObjFileModel* GetVertexBuffer();
};