#include "Projectile.h"

Projectile::Projectile(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
	:
	model(new Model(_gfx, _device, _immContext))
{ 
	speed = 20.0f;
}

void Projectile::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
	model->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
}

void Projectile::UpdateLogic(float dt)
{
}

void Projectile::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection)
{
	model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector);
}

void Projectile::Draw()
{
	model->Draw();
}

bool Projectile::CanDestroy()
{
	return canDestroy;
}
