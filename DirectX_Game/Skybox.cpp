#include "Skybox.h"

Skybox::Skybox(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
	:
	cube(new Model(_gfx, _device, _immContext))
{ }

void Skybox::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
	cube->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
}

Skybox::~Skybox()
{
	if (cube != nullptr)
	{
		delete cube;
		cube = nullptr;
	}
}

void Skybox::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection)
{
	XMVECTOR a = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	cube->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, a);
}

void Skybox::Draw()
{
	cube->Draw();
}
