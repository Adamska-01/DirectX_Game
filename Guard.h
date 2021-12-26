#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Player.h"


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

	XMFLOAT3 startPos;
	float minAlertDistance;

	//change dir timer stuff
	float intervalPatrol;
	float intervalAttack;
	float currentTimePatrol;
	float currentTimeAttack;

public:
	Guard(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~Guard();
	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);

	void UpdateLogic(float dt, Player* p);
	void AssignState(Player* p);

	void UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection);
	void Draw();

	bool IsDead();
};