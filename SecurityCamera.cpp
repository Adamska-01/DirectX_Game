#include "SecurityCamera.h"
#include "CollisionHandler.h"
#include <algorithm>

SecurityCamera::SecurityCamera(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
	:
	model(new Model(_gfx, _device, _immContext))
{
	health = 100.0f;
	rangeToDamage = 15.0f;
}

SecurityCamera::~SecurityCamera()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void SecurityCamera::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
	model->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
}

void SecurityCamera::UpdateLogic(float dt, Player* p)
{
	float distanceFromPlayer = sqrt(pow((p->GetCamera()->GetPositionFloat3().x - pos.x), 2) + pow((p->GetCamera()->GetPositionFloat3().y - pos.y), 2) + pow((p->GetCamera()->GetPositionFloat3().z - pos.z), 2));
	if (distanceFromPlayer <= rangeToDamage)
	{
		//TODO: deal damage to player
	}

	CheckCollisionAndDamage(p->GetProjectiles());
} 

void SecurityCamera::CheckCollisionAndDamage(std::vector<Projectile*> const& _projectiles)
{
	CalculateBoundingSphereWorldPos();
	int length = _projectiles.size();
	for (int i = 0; i < length; i++)
	{
		_projectiles[i]->CalculateBoundingSphereWorldPos();
		if (CollisionHandler::SphereToSphereCollision(sphere, _projectiles[i]->sphere))
		{
			DealDamageToSelf(_projectiles[i]->GetDamage());
			_projectiles[i]->SetDestruction(true);
			modColour = true;
		}
	}
}

void SecurityCamera::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection)
{
	if (modColour)
	{
		XMVECTOR colourMod = XMVectorSet(15.0f, 0.0f, 0.0f, 0.0f);
		model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod);
	}
	else
	{
		XMVECTOR colourMod = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod);
	}
}

void SecurityCamera::Draw()
{
	model->Draw();
}

void SecurityCamera::DealDamageToSelf(float _dmg)
{
	health -= _dmg;
	std::clamp(health, 0.0f, 100.0f);
}

Model* SecurityCamera::GetModel()
{
	return model;
}

bool SecurityCamera::IsDead()
{
	return false;
}

void SecurityCamera::SetStartPos(float _x, float _y, float _z)
{
	startPos.x = _x;
	startPos.y = _y;
	startPos.z = _z;
}
