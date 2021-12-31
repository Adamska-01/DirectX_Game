#pragma once
#include "Camera.h"
#include "Map.h"
#include "Projectile.h"
#include "Keyboard.h"
#include "Mouse.h"

class Map;
 
class Player
{
private:
	Graphics* gfx;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmContext;
private:
	Camera* camera;
	Map* map;
	Keyboard* keyboard;
	Mouse* mouse;
	std::vector<Projectile*> projectiles;
	ObjFileModel* projectileModel;

	XMFLOAT3 startPos;
	XMFLOAT3 velocity;

	float speed;
	float speedValue;
	float speedBoost;
	float jumpSpeed;
	float cameraSpeed;
	float jumpForce = 90.0f;
	float gravity = 6.5f;

	float fireRate = 0.2f;
	float currentTime = 0.0f;

private: 
	float health;

public:
	Player(Map* _map, Keyboard* kbd, Mouse* ms, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext, ObjFileModel* prjModel);
	~Player();

	void UpdateLogic(float dt); 
	void Draw(XMMATRIX _view, XMMATRIX _projection);

	void Forward(float dt);
	void BackWards(float dt);
	void Left(float dt);
	void Right(float dt);
	void Up(float dt);
	void Down(float dt);
	void Jump(float jumpforce);

	void Gravity(float dt);

	bool IsDead();

	void SetStartPos(float _x, float _y, float _z);

public:
	Camera* GetCamera();
	int GetHealth();
	std::vector<Projectile*> GetProjectiles();
};
