#include "SecurityCamera.h"
#include "CollisionHandler.h"
#include "FrameTimer.h"
#include <algorithm>

SecurityCamera::SecurityCamera(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
	:
	model(new Model(_gfx, _device, _immContext))
{
	//Stats
	health = 100.0f;
	rangeToDamage = 20.0f;
	damagePerSec = 5.0f;
	
	rotRange = .7f;
	startRot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	modColour = false; 

	//Timer stuff (colourMode)
	currentTimeColourMod = 0.0f;
	intervalColourMod = 0.2f;

	time = 0.0f;
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
	//Rotation 
	time += dt;
	float vl = sin(time);
	SetRotation(startRot.x, startRot.y + rotRange * vl, startRot.z);

	//Deal damage to player if too close
	float distanceFromPlayer = (float)sqrt(pow((p->GetCamera()->GetPositionFloat3().x - pos.x), 2) + pow((p->GetCamera()->GetPositionFloat3().y - pos.y), 2) + pow((p->GetCamera()->GetPositionFloat3().z - pos.z), 2));
	if (distanceFromPlayer <= rangeToDamage)
	{
		p->DealDamageToSelf(damagePerSec * dt);
	}

	//Deal damage to self if hit by projectile
	CheckCollisionAndDamage(p->GetProjectiles());

	//Allow to modify the colour if the guard is shot by the player 
	if (modColour)
	{
		currentTimeColourMod += dt;
		if (currentTimeColourMod >= intervalColourMod)
		{
			modColour = false;
			currentTimeColourMod = 0.0f;
		}
	}
} 

void SecurityCamera::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection)
{
	//Set colour mod if hit by projectile
	if (modColour)
	{
		XMVECTOR colourMod = XMVectorSet(2.0f, 1.0f, 1.0f, 1.0f);
		model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod);
	}
	else
	{
		XMVECTOR colourMod = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod);
	}
}

void SecurityCamera::Draw()
{
	model->Draw();
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
			if (IsDead())
				canDestroy = true;
			_projectiles[i]->SetDestruction(true);
			modColour = true;
		}
	}
}


void SecurityCamera::DealDamageToSelf(float _dmg)
{
	health -= _dmg;
	health = std::clamp(health, 0.0f, 100.0f);
}

Model* SecurityCamera::GetModel()
{
	return model;
}

bool SecurityCamera::IsDead()
{
	return health <= 0.0f;
}

bool SecurityCamera::CanDestroy()
{
	return canDestroy;
}

void SecurityCamera::SetStartRot(float _x, float _y, float _z)
{
	startRot.x = _x;
	startRot.y = _y;
	startRot.z = _z;
}
