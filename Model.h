#pragma once
#include "objfilemodel.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"

class Model
{
private:
	ID3D11Device* pD3DDevice = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr;
	ID3D11VertexShader* pVshader = nullptr;
	ID3D11PixelShader* pPshader = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;
	ID3D11Buffer* pConstantBuffer = nullptr;

	//Textures
	ID3D11ShaderResourceView* pTexture = nullptr;
	ID3D11SamplerState* pSampler = nullptr;

	struct MODEL_CONSTANT_BUFFER
	{
		XMMATRIX worldViewProjection;	 //64 bytes
		XMMATRIX worldView;	 //64 bytes
		XMVECTOR directionalLightVector; //16 bytes
		XMVECTOR directionalLightColour; //16 bytes
		XMVECTOR ambientLightColour;     //16 bytes
		XMVECTOR pointLightPos;          //16 bytes
		XMVECTOR pointLightColour;       //16 bytes

		XMFLOAT3 pointLightAttenuation;  //12 bytes
		float range;                     //4 bytes
	}; //Total size = 224 bytes 

private:
	ObjFileModel* pObject = nullptr;

	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	//Look at
	float dx, dy, dz;

private: 
	XMFLOAT3 colSphereCentre; 
	float colSphereRadius;

public:
	Model(ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~Model();

	HRESULT LoadObjModel(std::string _filename = "", std::string _VSshader = "ModelVS", std::string _PSshader = "ModelPS");
	HRESULT AddTexture(std::string _filename);
	void UpdateConstantBf(XMMATRIX _view, XMMATRIX _projection, AmbientLight* _ambLight = nullptr, DirectionalLight* _dirLight = nullptr, PointLight* _pointLight = nullptr);
	void LookAt_XYZ(float _x, float _y, float _z);
	//Sphere collision
	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();
	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius();
	bool CheckCollision(Model* _model);

	void Draw();

	//Transformations
	void MoveForwardY(float _speed);
	void MoveForwardXY(float _speed);
	void MoveBackwardsXY(float _speed);
	void Translate(const XMVECTOR& _pos);
	void Translate(float _x, float _y, float _z);
	void Rotate(const XMVECTOR& _rot);
	void Rotate(float _x, float _y, float _z);

	//Setters
	void SetPosition(const XMVECTOR& _pos);
	void SetPosition(float _x, float _y, float _z);
	void SetRotation(const XMVECTOR& _rot);
	void SetRotation(float _x, float _y, float _z);
	void Scale(const XMVECTOR& _scale);
	void Scale(float _x, float _y, float _z);
	void SetModel(ObjFileModel* _obj);
	//Getters
	const XMFLOAT3& GetPosition();
	const XMFLOAT3& GetRotation();
	const XMFLOAT3& GetScale();
	ObjFileModel* GetModel();
};