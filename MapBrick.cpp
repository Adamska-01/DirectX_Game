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

void MapBrick::CalculateBoundingBoxWorldPos()
{
	XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	XMMATRIX pos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);

	XMMATRIX world = scale * rot * pos;
	
	//min
	box.minBoundV = XMLoadFloat3(&box.minBound);
	box.minBoundV = XMVector3Transform(box.minBoundV, world);
	//max
	box.maxBoundV = XMLoadFloat3(&box.maxBound);
	box.maxBoundV = XMVector3Transform(box.maxBoundV, world);
}

Model* MapBrick::GetBrick()
{
	return brick;
}
