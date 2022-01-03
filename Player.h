#pragma once
#include "Camera.h"
#include "Map.h"
#include "Projectile.h"
#include "Keyboard.h"
#include "Mouse.h"

class Map;
 
class Player
{
private: //SubSystem stuff
	Graphics* gfx;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmContext;
	
	Keyboard* keyboard;
	Mouse* mouse;

	ObjFileModel* projectileModel;
private:
	Camera* camera;
	Map* map;
	std::vector<Projectile*> projectiles; //Holds the projectiles 

	XMFLOAT3 startPos;
	XMFLOAT3 velocity;

	//Stats and skills
	float health;
	float speed, speedValue, speedBoost;
	float jumpSpeed;
	float jumpForce;
	float gravity;
	float cameraSpeed;

	//Camera rotation
	float maxRotX, minRotX;

	//Timer stuff for shooting
	float fireRate;
	float currentTime;

public:
	Player(Map* _map, Keyboard* kbd, Mouse* ms, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext, ObjFileModel* prjModel);
	~Player();

	void UpdateLogic(float dt); 
	void Draw(XMMATRIX _view, XMMATRIX _projection);

private:
	//Movement + collisions with map
	void Forward(float dt);
	void BackWards(float dt);
	void Left(float dt);
	void Right(float dt);
	void Up(float dt);
	void Down(float dt);
	void Jump(float jumpforce);

	void Gravity(float dt); 
public:
	void DealDamageToSelf(float _dmg);  

	//Getters
	bool IsDead();
	Camera* GetCamera();
	float GetHealth();
	std::vector<Projectile*> GetProjectiles();
	
	//Setters
	void SetStartPos(float _x, float _y, float _z);
};
