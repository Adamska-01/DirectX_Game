#include "MapBrick.h"

MapBrick::MapBrick(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
	:
	brick(new Model(_gfx, _device, _immContext))
{ }

void MapBrick::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
	brick->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
}

MapBrick::~MapBrick()
{
	if (brick != nullptr)
	{
		delete brick;
		brick = nullptr;
	}
}

void MapBrick::Draw()
{
	brick->Draw();
} 

Model* MapBrick::GetBrick()
{
	return brick;
}
