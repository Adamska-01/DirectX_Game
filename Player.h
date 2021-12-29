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

	float jumpForce = 28;
	float gravity = 1.5f;

	float fireRate = 0.2f;
	float currentTime = 0.0f;

private: 
	float health;

public:
	Player(Map* _map, Keyboard* kbd, Mouse* ms, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext, ObjFileModel* prjModel);
	~Player();

	void UpdateLogic(float dt); 
	void Draw(XMMATRIX _view, XMMATRIX _projection);

	void Forward();
	void BackWards();
	void Left();
	void Right();
	void Up();
	void Down();
	void Jump(float jumpforce);

	void Gravity();

	bool IsDead();

public:
	Camera* GetCamera();
	int GetHealth();
	std::vector<Projectile*> GetProjectiles();
};
