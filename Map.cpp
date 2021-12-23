#include "Map.h"

Map::Map(std::string filePath, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	//Loads map from file
	IOFunctions::LoadMapFromFile(filePath, gridMap);

	width = gridMap[0].length();
	height = gridMap.size();
	bricksNumber = width * height;

	gfx = _gfx;
	pDevice = _device;
	pImmContext = _context;
}

Map::~Map()
{
	Clean();
}

void Map::LoadMap(std::map<Constants::Models, ObjFileModel*>& models)
{
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++)
		{
			switch (gridMap[row][column])
			{
			case '0':
				bricks.push_back(new MapBrick(gfx, pDevice, pImmContext));
				bricks.back()->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::floor2TX);
				bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetModel());
				bricks.back()->Scale(3.0f,3.0f,3.0f);
				bricks.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), 0.0f, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				bricks.back()->CalculateBoundingBoxWorldPos();
					break;
			default:
				break;
			}
		}
}
 
void Map::Draw(XMMATRIX _view, XMMATRIX _projection)
{   
	for (int i = 0; i < bricksNumber; i++)
	{
		bricks[i]->GetBrick()->UpdateConstantBf(_view, _projection, bricks[i]->GetPositionVector(), bricks[i]->GetRotationVector(), bricks[i]->GetScaleVector());
		bricks[i]->Draw();
	}
}

void Map::Clean()
{
}

int Map::GetBrickNumber()
{
	return bricksNumber;
}

std::vector<MapBrick*>& Map::GetBricks()
{
	return bricks;
}
