#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Player.h" 

class Player;
class Projectile;
class Map;

class Guard : public GameObject
{
private:
	enum class States
	{
		ATTACK,
		PATROLLING,
		GOTOSTART
	};
private:
	Model* model;
	float health;
	float speed;
	States state;
	bool modColour;

	XMFLOAT3 startPos;
	float minAlertDistance;

	//change dir timer stuff
	float intervalPatrol;
	float intervalAttack;
	float intervalColourMod;
	float intervalToRespawn;
	float currentTimeRespawn;
	float currentTimePatrol;
	float currentTimeAttack;
	float currentTimeColourMod;

public:
	Guard(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~Guard();
	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);

	void UpdateLogic(float dt, Player* p, Map* _map);
private:
	void AssignState(Player* p);
	void DealDamageToSelf(float _dmg);
public:
	void CheckCollisionAndDamage(std::vector<Projectile*>const & _projectiles);
	void UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection, AmbientLight* _ambLight, DirectionalLight* _dirLight, PointLight* _pointLight);
	void Draw();

	bool IsDead();

	//Getters
	Model* GetModel();

	//Setters
	void SetStartPos(float _x, float _y, float _z);
};