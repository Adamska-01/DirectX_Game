#pragma once
#include "GameObject.h"
#include "Model.h"


class Projectile : public GameObject
{
private:
	Model* model;

	//Stats
	float speed;
	float damage;
	bool canDestroy = false;

	//Timer stuff
	float currentTime;
	float timeToDestroy;

public:
	Projectile(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	~Projectile();

	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);

	void UpdateLogic(float dt);
	void UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection);
	void Draw();

	//Getters 
	bool CanDestroy();
	float GetDamage();

	//Setters
	void SetDestruction(bool _value);
};