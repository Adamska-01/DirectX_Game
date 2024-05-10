#include "Player.h"
#include "CollisionHandler.h"
#include "CollisionData.h"
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

	maxRotX = 1.1f;
	minRotX = -1.1f;
}

Player::~Player()
{
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
	if (projectiles.size() > 0)
	{
		for (unsigned int i = 0; i < projectiles.size(); i++)
		{
			delete projectiles[i];
			projectiles[i] = nullptr;
		}
		projectiles.clear();
	}
}

void Player::UpdateLogic(float dt)
{ 
	//Fire rate
	currentTime += dt;

	//Movement
	auto movementVector = XMVectorZero();
	if (keyboard->IsKeyPressed(DIK_W))
	{
		movementVector += camera->GetForwardVector();
	}
	if (keyboard->IsKeyPressed(DIK_A))
	{
		movementVector += camera->GetLeftVector();
	}
	if (keyboard->IsKeyPressed(DIK_S))
	{
		movementVector += camera->GetBackwardVector();
	}
	if (keyboard->IsKeyPressed(DIK_D))
	{
		movementVector += camera->GetRightVector();
	}

	Move(XMVector3Normalize(movementVector), dt);

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
	if (camera->GetRotationFloat3().x > maxRotX)
		camera->SetRotation(maxRotX, camera->GetRotationFloat3().y, camera->GetRotationFloat3().z);
	else if (camera->GetRotationFloat3().x < minRotX)
		camera->SetRotation(minRotX, camera->GetRotationFloat3().y, camera->GetRotationFloat3().z);

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

	//Respawn
	if (IsDead())
	{
		health = 100.0; 
		camera->SetPosition(startPos.x, startPos.y, startPos.z);
		camera->SetRotation(0.0f, 0.0f, 0.0f);
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

void Player::Move(const XMVECTOR& direction, float dt)
{
	if (XMVector3Equal(direction, XMVectorZero()))
		return; 

	auto movement = direction * dt * speed;

	auto newPos = camera->GetPositionVector() + movement;

	// Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		auto intersection = CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box);
		if (intersection.isColliding)
		{
			return;
		}
	}
	
	camera->AdjustPosition(movement);
}

void Player::Up(float dt)
{
	auto movement = XMVectorSet(0.0f, 0.9f * dt * jumpSpeed, 0.0f, 1.0f);
	
	auto newPos = camera->GetPositionVector() + XMVectorSet(0.0f, 0.9f * dt * jumpSpeed, 0.0f, 1.0f);

	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		auto intersection = CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box);
		if (intersection.isColliding)
		{
			velocity.y = 0.0f; //stop jump
			return;
		}
	}

	//No collisions, move
	camera->AdjustPosition(movement);
}

void Player::Down(float dt)
{
	auto movement = XMVectorSet(0.0f, -0.9f * dt * jumpSpeed, 0.0f, 1.0f);

	auto newPos = camera->GetPositionVector() + movement;

	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		auto intersection = CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box);
		if (intersection.isColliding)
		{
			velocity.y = 0.0f; //stop jump
			return;
		}
	}

	//No collisions, move
	camera->AdjustPosition(movement);
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
	health = std::clamp(health, 0.0f, 100.0f);
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

float Player::GetHealth()
{
	return health;
}

std::vector<Projectile*> Player::GetProjectiles()
{
	return projectiles;
}
