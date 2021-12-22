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
		XMVECTOR directionalLightVector; //16 bytes
		XMVECTOR directionalLightColour; //16 bytes
		XMVECTOR ambientLightColour;     //16 bytes
		XMVECTOR pointLightPos;          //16 bytes
		XMVECTOR pointLightColour;       //16 bytes

		XMFLOAT3 pointLightAttenuation;  //12 bytes
		float range;                     //4 bytes
	}; //Total size = 224 bytes  
private:
	ID3D11Device* pD3DDevice = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr; 
	ID3D11Buffer* pConstantBuffer = nullptr;
	Graphics* gfx;

	std::string vsShader, psShader, texture;  
private:
	ObjFileModel* pObject = nullptr;

	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	  
private: 
	XMFLOAT3 colSphereCentre; 
	float colSphereRadius;

public:
	Model(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~Model();

	HRESULT LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);
	void UpdateConstantBf(XMMATRIX _view, XMMATRIX _projection, XMVECTOR _pos, XMVECTOR _rot, XMVECTOR _scale, AmbientLight* _ambLight = nullptr, DirectionalLight* _dirLight = nullptr, PointLight* _pointLight = nullptr);
	//Sphere collision
	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();
	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius();
	bool CheckCollision(Model* _model);
private:
	void SetTexture(std::string _texture);
	void SetShaders(std::string _vs, std::string _ps);
public:
	void Draw();
 
	ObjFileModel* GetModel();
};