#include "Map.h"

Map::Map(std::string filePath, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	//Loads map from file
	IOFunctions::LoadMapFromFile(filePath, gridMap);

	width = gridMap[0].length();
	height = gridMap.size(); 

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
				bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetVertexBuffer());
				bricks.back()->Scale(3.0f,3.0f,3.0f);
				bricks.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), 0.0f, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				bricks.back()->CalculateBoundingBoxWorldPos();
				bricksNumber++;
				break;
			case '3':
				//Place ground floor
				bricks.push_back(new MapBrick(gfx, pDevice, pImmContext));
				bricks.back()->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::floor2TX);
				bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetVertexBuffer());
				bricks.back()->Scale(3.0f, 3.0f, 3.0f);
				bricks.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), 0.0f, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				bricks.back()->CalculateBoundingBoxWorldPos();
				bricksNumber++;

				//Place wall
				for (int i = 1; i <= 3; i++)
				{
					bricks.push_back(new MapBrick(gfx, pDevice, pImmContext));
					bricks.back()->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::floorTX);
					bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetVertexBuffer());
					bricks.back()->Scale(3.0f, 3.0f, 3.0f);
					bricks.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), i * bricks.back()->GetScaleFloat3().y * (bricks.back()->box.maxBound.y - bricks.back()->box.minBound.y), column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
					bricks.back()->CalculateBoundingBoxWorldPos();
					bricksNumber++;
				}
				break;
			case 'N':
				//Place ground floor
				bricks.push_back(new MapBrick(gfx, pDevice, pImmContext));
				bricks.back()->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::floor2TX);
				bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetVertexBuffer());
				bricks.back()->Scale(3.0f, 3.0f, 3.0f);
				bricks.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), 0.0f, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				bricks.back()->CalculateBoundingBoxWorldPos();
				bricksNumber++;

				//Guard
				guard = new Guard(gfx, pDevice, pImmContext);
				guard->LoadObjModel(models[Constants::Models::SPHERE], Constants::modelVS, Constants::modelPS, Constants::enemyTX);
				guard->sphere.CalculateModelCentrePoint(guard->GetModel()->GetVertexBuffer());
				guard->sphere.CalculateBoundingSphereRadius(guard->GetModel()->GetVertexBuffer());
				guard->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), bricks.back()->GetScaleFloat3().y * guard->sphere.radius, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				guard->SetStartPos(guard->GetPositionFloat3().x, guard->GetPositionFloat3().y, guard->GetPositionFloat3().z);
				break;
			default:
				break;
			}
		}
}
 
void Map::UpdateLogic(float dt, Player* p)
{
	guard->UpdateLogic(dt, p);
}

void Map::Draw(XMMATRIX _view, XMMATRIX _projection)
{   
	for (int i = 0; i < bricksNumber; i++)
	{
		bricks[i]->GetBrick()->UpdateConstantBf(_view, _projection, bricks[i]->GetPositionVector(), bricks[i]->GetRotationVector(), bricks[i]->GetScaleVector());
		bricks[i]->Draw();
	}

	//Render guard
	guard->UpdateConstantBF(_view, _projection);
	guard->Draw();
}

void Map::Clean()
{
	for (int i = 0; i < bricksNumber; i++)
	{
		delete bricks[i];
		bricks[i] = nullptr;
	}
	bricks.clear();

	if (guard != nullptr)
	{
		delete guard;
		guard = nullptr;
	}
}

int Map::GetBrickNumber()
{
	return bricksNumber;
}

std::vector<MapBrick*>& Map::GetBricks()
{
	return bricks;
}
 