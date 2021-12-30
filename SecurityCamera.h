#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Player.h" 

class Player;

class SecurityCamera : public GameObject
{
private:
	Model* model;
	float health;
	float rangeToDamage;
	XMFLOAT3 startPos;
	bool modColour;

public:
	SecurityCamera(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~SecurityCamera();
	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);

	void UpdateLogic(float dt, Player* p);
	void CheckCollisionAndDamage(std::vector<Projectile*>const& _projectiles);
	void UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection);
	void Draw();
public: 
	

private:
	void DealDamageToSelf(float _dmg);

public:
	//Getters
	bool IsDead();

	//Setters
	void SetStartPos(float _x, float _y, float _z);
};