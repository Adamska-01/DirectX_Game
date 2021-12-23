#pragma once
#include "Camera.h"
#include "Map.h"
#include "Keyboard.h"
#include "Mouse.h"


class Player
{
private:
	Camera* camera;
	Map* map;
	Keyboard* keyboard;
	Mouse* mouse;

	XMFLOAT3 startPos;
	XMFLOAT3 velocity;
	bool isDead = false;

	float jumpForce = 28;
	float gravity = 1.5f;

public:
	Player(Map* _map, Keyboard* kbd, Mouse* ms);
	~Player();

	void UpdateLogic();
	void Forward();
	void BackWards();
	void Left();
	void Right();
	void Up();
	void Down();
	void Jump(float jumpforce);

	void Gravity();

public:
	Camera* GetCamera();
};
