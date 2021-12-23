#include "Player.h"
#include "CollisionHandler.h"

Player::Player(Map* _map, Keyboard* kbd, Mouse* ms)
	:
	camera(new Camera()),
	map(_map),
	keyboard(kbd),
	mouse(ms)
{ 
	camera->sphere.colSphereCentre = camera->GetPositionFloat3();
	camera->sphere.colSphereRadius = 4.0f;

	startPos = map->GetBricks()[10]->GetPositionFloat3();
	camera->SetPosition(startPos.x, startPos.y + 10.0f, startPos.z);
}

Player::~Player()
{
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
}

void Player::UpdateLogic()
{ 
	//input check
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
	if (keyboard->IsKeyPressed(DIK_SPACE) && velocity.y == 0.0f)
	{
		Jump(jumpForce);
	}

	if (mouse->IsRightClickPressed())
	{
		XMFLOAT2 movement = mouse->GetMouseMovement();
		camera->AdjustRotation(0.01f * movement.x, 0.01f * movement.y, 0.0f);
	}

	Gravity(); 
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

Camera* Player::GetCamera()
{
	return camera;
}
