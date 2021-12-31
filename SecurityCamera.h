#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Player.h" 

class Player;
class Projectile;

class SecurityCamera : public GameObject
{
private:
	Model* model;

	//Stats
	float health;
	float rangeToDamage;
	float damagePerSec;
	XMFLOAT3 startRot;
	bool modColour;
	float rotRange;
	bool canDestroy = false; 

	//Timer for colour mode
	float currentTimeColourMod;
	float intervalColourMod;

public:
	SecurityCamera(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~SecurityCamera();

	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);
	void UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection);
	void Draw(); 

	void UpdateLogic(float dt, Player* p);
	void CheckCollisionAndDamage(std::vector<Projectile*>const& _projectiles);
private:
	void DealDamageToSelf(float _dmg);

public:
	//Getters
	Model* GetModel();
	bool IsDead();
	bool CanDestroy();

	//Setters
	void SetStartRot(float _x, float _y, float _z);
};