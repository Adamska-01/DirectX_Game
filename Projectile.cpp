#include "Projectile.h"

Projectile::Projectile(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
	:
	model(new Model(_gfx, _device, _immContext))
{ 
	speed = 85.0f;
	damage = 20.0f;

	currentTime = 0.0f;
	timeToDestroy = 5.0f;
}

Projectile::~Projectile()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void Projectile::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
	model->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
} 

void Projectile::UpdateLogic(float dt)
{
	//Movement (forward)
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	AdjustPosition(GetForwardVector() * dt * speed);
	
	//Set destructiona after some time
	currentTime += dt;
	if (currentTime >= timeToDestroy)
	{
		canDestroy = true;
	}
}

void Projectile::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection)
{
	XMVECTOR colourMod = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod);
}

void Projectile::Draw()
{
	model->Draw();
}

bool Projectile::CanDestroy()
{
	return canDestroy;
}

float Projectile::GetDamage()
{
	return damage;
}

void Projectile::SetDestruction(bool _value)
{
	canDestroy = _value;
}
