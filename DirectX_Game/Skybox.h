#pragma once
#include "GameObject.h"
#include "Model.h"


class Skybox : public GameObject
{
private:
	Model* cube;

public:
	Skybox(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);

	~Skybox();

	void UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection);
	void Draw();
};