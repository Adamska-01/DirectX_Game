#pragma once
#include "GameObject.h"
#include "Model.h"


class MapBrick : public GameObject
{
private:
	//Grids
	Model* brick; 

public:
	MapBrick(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext);
	void LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture);

	~MapBrick();
	   
	void Draw(); 
	 
	Model* GetBrick();
};