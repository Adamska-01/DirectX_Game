#include "Player.h"
#include "CollisionHandler.h"

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

	startPos = map->GetBricks()[10]->GetPositionFloat3();
	camera->SetPosition(startPos.x, startPos.y + 40.0f, startPos.z);

	health = 100.0f;
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
		Forward();
	}
	if (keyboard->IsKeyPressed(DIK_A))
	{
		Left();
	}
	if (keyboard->IsKeyPressed(DIK_S))
	{
		BackWards();
	}
	if (keyboard->IsKeyPressed(DIK_D))
	{
		Right();
	}
	//Jump
	if (keyboard->IsKeyPressed(DIK_SPACE) && velocity.y == 0.0f)
	{
		Jump(jumpForce);
	}
	//Camera rotation
	if (mouse->IsRightClickPressed())
	{
		XMFLOAT2 movement = mouse->GetMouseMovement();
		camera->AdjustRotation(0.01f * movement.x, 0.01f * movement.y, 0.0f);
	}
	//Shoot
	if (mouse->IsLeftClickDown() && currentTime >= fireRate)
	{
		projectiles.push_back(new Projectile(gfx, pDevice, pImmContext));
		projectiles.back()->LoadObjModel(projectileModel, Constants::modelVS, Constants::modelPS, Constants::goldTX);
		projectiles.back()->SetPosition(camera->GetPositionVector());
		projectiles.back()->SetRotation(camera->GetRotationVector());
		projectiles.back()->Scale(0.1f, 0.1f, 0.1f);

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


	Gravity(); 
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

void Player::Forward()
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetForwardVector();
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}
	
	//No collisions, move
	camera->AdjustPosition(camera->GetForwardVector());
}

void Player::BackWards()
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetBackwardVector();
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}

	//No collisions, move
	camera->AdjustPosition(camera->GetBackwardVector());
}

void Player::Left()
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetLeftVector();
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}

	//No collisions, move
	camera->AdjustPosition(camera->GetLeftVector());
}

void Player::Right()
{
	XMVECTOR newPos = camera->GetPositionVector() + camera->GetRightVector();
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
			return;
	}

	//No collisions, move
	camera->AdjustPosition(camera->GetRightVector());
}

void Player::Up()
{
	XMVECTOR newPos = camera->GetPositionVector() + XMVectorSet(0.0f, 0.2f, 0.0f, 1.0f);
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
		{
			velocity.y = 0.0f;
			return;
		}
	}

	//No collisions, move
	camera->AdjustPosition(0.0f, 0.2f, 0.0f);
}

void Player::Down()
{
	XMVECTOR newPos = camera->GetPositionVector() + XMVectorSet(0.0f, -0.2f, 0.0f, 1.0f);
	//Calculate new bounding sphere
	camera->CalculateBoundingSphereWorldPos(newPos);

	int length = map->GetBrickNumber();
	for (int i = 0; i < length; i++)
	{
		if (CollisionHandler::SphereToBoxCollision(camera->sphere, map->GetBricks()[i]->box))
		{
			velocity.y = 0.0f;
			return;
		}
	}

	//No collisions, move
	camera->AdjustPosition(0.0f, -0.2f, 0.0f);
}

void Player::Jump(float jumpforce)
{
	velocity.y = jumpforce;
}

void Player::Gravity()
{
	velocity.y -= gravity;

	if (velocity.y > 0)
	{
		Up();
	}
	else
	{
		Down();
	}
}

bool Player::IsDead()
{
	return health <= 0; 
}

Camera* Player::GetCamera()
{
	return camera;
}

int Player::GetHealth()
{
	return health;
}
