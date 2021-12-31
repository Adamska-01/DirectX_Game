#include "Player.h"
#include "CollisionHandler.h"
#include <algorithm>

Player::Player(Map* _map, Keyboard* kbd, Mouse* ms, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext, ObjFileModel* prjModel)
	:
	camera(new Camera()),
	map(_map),
	keyboard(kbd),
	mouse(ms),
	gfx(_gfx),
	pDevice(_device),
	pImmContext(_immContext),
	projectileModel(prjModel)
{ 
	camera->sphere.centre = camera->GetPositionFloat3();
	camera->sphere.radius = 4.0f; 

	health = 100.0f;
	speed = speedValue = 15.0f;
	speedBoost = 10.0f;
	jumpSpeed = 19.0f;
	cameraSpeed = 0.1f;

	jumpForce = 90.0f;
	gravity = 6.5f;

	fireRate = 0.2f;
	currentTime = 0.0f;
}

Player::~Player()
{
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
}

void Player::UpdateLogic(float dt)
{ 
	//Fire rate
	currentTime += dt;

	//Movement
	if (keyboard->IsKeyPressed(DIK_W))
	{
		Forward(dt);
	}
	if (keyboard->IsKeyPressed(DIK_A))
	{
		Left(dt);
	}
	if (keyboard->IsKeyPressed(DIK_S))
	{
		BackWards(dt);
	}
	if (keyboard->IsKeyPressed(DIK_D))
	{
		Right(dt);
	}
	//Jump
	if (keyboard->IsKeyDown(DIK_SPACE) && velocity.y == 0.0f)
	{
		Jump(jumpForce);
	}
	//Run
	if (keyboard->IsKeyPressed(DIK_LSHIFT))
	{
		speed = speedValue + speedBoost;
	}
	if (keyboard->IsKeyUp(DIK_LSHIFT))
	{
		speed = speedValue;
	}
	//Camera rotation 
	XMFLOAT2 movement = mouse->GetMouseMovement();
	camera->AdjustRotation(cameraSpeed * movement.x * dt, cameraSpeed * movement.y * dt, 0.0f); 
	//Shoot
	if (mouse->IsLeftClickDown() && currentTime >= fireRate)
	{
		//Instantiate a new projctile and give it a position
		projectiles.push_back(new Projectile(gfx, pDevice, pImmContext));
		projectiles.back()->LoadObjModel(projectileModel, Constants::modelVS, Constants::modelPS, Constants::goldTX);
		projectiles.back()->Scale(0.1f, 0.1f, 0.1f);
		projectiles.back()->sphere.CalculateModelCentrePoint(projectileModel);
		projectiles.back()->sphere.CalculateBoundingSphereRadius(projectileModel, projectiles.back()->GetScaleFloat3().x);
		projectiles.back()->SetPosition(camera->GetPositionVector());
		projectiles.back()->SetRotation(camera->GetRotationVector());

		currentTime = 0.0f;
	}


	//projectiles update 
	int length = projectiles.size();
	for (int i = 0; i < length; i++)
	{ 
		projectiles[i]->UpdateLogic(dt); 
	} 
	for (int i = 0; i < length; i++) //Destroy projectiles
	{
		if (projectiles[i]->CanDestroy())
		{
			delete projectiles[i];
			projectiles[i] = nullptr;
			projectiles.erase(projectiles.begin() + i);
			--length;
		}
	} 


	Gravity(dt); 
}
 
void Player::Draw(XMMATRIX _view, XMMATRIX _projection)
{
	int length = projectiles.size();
	for (int i = 0; i < length; i++)
	{
		projectiles[i]->UpdateConstantBF(_view, _projection);
		projectiles[i]->Draw();
	}
}

void Player::Forward(float dt)
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetForwardVector() * dt * speed;
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}
	
	//No collisions, move
	camera->AdjustPosition(camera->GetForwardVector() * dt * speed);
}

void Player::BackWards(float dt)
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetBackwardVector() * dt * speed;
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}

	//No collisions, move
	camera->AdjustPosition(camera->GetBackwardVector() * dt * speed);
}

void Player::Left(float dt)
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetLeftVector() * dt * speed;
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}

	//No collisions, move
	camera->AdjustPosition(camera->GetLeftVector() * dt * speed);
}

void Player::Right(float dt)
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetRightVector() * dt * speed;
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}

	//No collisions, move
	camera->AdjustPosition(camera->GetRightVector() * dt * speed);
}

void Player::Up(float dt)
{
	XMVECTOR newPos = camera->GetPositionVector() + XMVectorSet(0.0f, 0.9f * dt * jumpSpeed, 0.0f, 1.0f);
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
		{
			velocity.y = 0.0f; //stop jump
			return;
		}
	}

	//No collisions, move
	camera->AdjustPosition(0.0f, 0.9f * dt * jumpSpeed, 0.0f);
}

void Player::Down(float dt)
{
	XMVECTOR newPos = camera->GetPositionVector() + XMVectorSet(0.0f, -0.9f * dt * jumpSpeed, 0.0f, 1.0f);
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
		{
			velocity.y = 0.0f; //Stop player from drowning to the ground
			return;
		}
	}

	//No collisions, move
	camera->AdjustPosition(0.0f, -0.9f * dt * jumpSpeed, 0.0f);
}

void Player::Jump(float jumpforce)
{
	velocity.y = jumpforce;
}

void Player::Gravity(float dt)
{
	velocity.y -= gravity;

	if (velocity.y > 0)
	{
		Up(dt);
	}
	else
	{
		Down(dt);
	}
}

bool Player::IsDead()
{
	return health <= 0; 
}

void Player::DealDamageToSelf(float _dmg)
{
	health -= _dmg;
	std::clamp(health, 0.0f, 100.0f);
}

void Player::SetStartPos(float _x, float _y, float _z)
{
	startPos.x = _x;
	startPos.y = _y;
	startPos.z = _z;
}

Camera* Player::GetCamera()
{
	return camera;
}

int Player::GetHealth()
{
	return health;
}

std::vector<Projectile*> Player::GetProjectiles()
{
	return projectiles;
}
