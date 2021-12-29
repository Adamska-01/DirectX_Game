#pragma once
#include "GameObject.h"
#include "Model.h"


class Projectile : public GameObject
{
private:
	Model* model;
	float speed;
	float damage;
	bool canDestroy = false;

	float currentTime = 0.0f;
	float timeToDestroy = 5.0f;

public:
	Projectile(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
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