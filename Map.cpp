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

void Map::LoadMap(std::map<Constants::Models, ObjFileModel*>& models, Player* p)
{
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++)
		{
			switch (gridMap[row][column])
			{
			case '0':
				PlaceGroundFloor(row, column, models);
				break;
			case '3':
				//Place ground floor
				PlaceGroundFloor(row, column, models);

				//Place wall
				for (int i = 1; i <= 3; i++)
				{
					bricks.push_back(new MapBrick(gfx, pDevice, pImmContext));
					bricks.back()->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::wallTX);
					bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetVertexBuffer());
					bricks.back()->Scale(3.0f, 3.0f, 3.0f);
					bricks.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), i * bricks.back()->GetScaleFloat3().y * (bricks.back()->box.maxBound.y - bricks.back()->box.minBound.y), column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
					bricks.back()->CalculateBoundingBoxWorldPos();
					bricksNumber++;
				}
				break;
			case 'N':
				//Place ground floor
				PlaceGroundFloor(row, column, models);

				//Place Guard
				guards.push_back(new Guard(gfx, pDevice, pImmContext));
				guards.back()->LoadObjModel(models[Constants::Models::SPHERE], Constants::modelVS, Constants::modelPS, Constants::enemyTX);
				guards.back()->sphere.CalculateModelCentrePoint(guards.back()->GetModel()->GetVertexBuffer());
				guards.back()->sphere.CalculateBoundingSphereRadius(guards.back()->GetModel()->GetVertexBuffer(), guards.back()->GetScaleFloat3().x);
				guards.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), bricks.back()->GetScaleFloat3().y * guards.back()->sphere.radius, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				guards.back()->SetStartPos(guards.back()->GetPositionFloat3().x, guards.back()->GetPositionFloat3().y, guards.back()->GetPositionFloat3().z);
				break;
			case 'A': 
				//Place ground floor
				PlaceGroundFloor(row, column, models);

				//Place camera
				secCamera.push_back(new SecurityCamera(gfx, pDevice, pImmContext));
				secCamera.back()->LoadObjModel(models[Constants::Models::CAMERA], Constants::reflectVS, Constants::reflectPS, Constants::skyboxTX);
				secCamera.back()->sphere.CalculateModelCentrePoint(secCamera.back()->GetModel()->GetVertexBuffer());
				secCamera.back()->sphere.CalculateBoundingSphereRadius(secCamera.back()->GetModel()->GetVertexBuffer(), secCamera.back()->GetScaleFloat3().x);
				secCamera.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), bricks.back()->GetScaleFloat3().y * secCamera.back()->sphere.radius, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				secCamera.back()->SetStartPos(secCamera.back()->GetPositionFloat3().x, secCamera.back()->GetPositionFloat3().y, secCamera.back()->GetPositionFloat3().z);
				secCamera.back()->SetRotation(-30.0f, -45.0f, 0.0f);
				break;
			case 'B':
				//Place ground floor
				PlaceGroundFloor(row, column, models);

				//Place camera
				secCamera.push_back(new SecurityCamera(gfx, pDevice, pImmContext));
				secCamera.back()->LoadObjModel(models[Constants::Models::CAMERA], Constants::reflectVS, Constants::reflectPS, Constants::skyboxTX);
				secCamera.back()->sphere.CalculateModelCentrePoint(secCamera.back()->GetModel()->GetVertexBuffer());
				secCamera.back()->sphere.CalculateBoundingSphereRadius(secCamera.back()->GetModel()->GetVertexBuffer(), secCamera.back()->GetScaleFloat3().x);
				secCamera.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), bricks.back()->GetScaleFloat3().y * secCamera.back()->sphere.radius, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				secCamera.back()->SetStartPos(secCamera.back()->GetPositionFloat3().x, secCamera.back()->GetPositionFloat3().y, secCamera.back()->GetPositionFloat3().z);
				secCamera.back()->SetRotation(-30.0f, 0.0f, 0.0f);
				break;
			case 'C':
				//Place ground floor
				PlaceGroundFloor(row, column, models);

				//Place camera
				secCamera.push_back(new SecurityCamera(gfx, pDevice, pImmContext));
				secCamera.back()->LoadObjModel(models[Constants::Models::CAMERA], Constants::reflectVS, Constants::reflectPS, Constants::skyboxTX);
				secCamera.back()->sphere.CalculateModelCentrePoint(secCamera.back()->GetModel()->GetVertexBuffer());
				secCamera.back()->sphere.CalculateBoundingSphereRadius(secCamera.back()->GetModel()->GetVertexBuffer(), secCamera.back()->GetScaleFloat3().x);
				secCamera.back()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), bricks.back()->GetScaleFloat3().y * secCamera.back()->sphere.radius, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
				secCamera.back()->SetStartPos(secCamera.back()->GetPositionFloat3().x, secCamera.back()->GetPositionFloat3().y, secCamera.back()->GetPositionFloat3().z);
				secCamera.back()->SetRotation(-30.0f, 45.0f, 0.0f);
				break;
			case 'P':
				//Place ground floor
				PlaceGroundFloor(row, column, models);

				//Set player position
				p->GetCamera()->SetPosition(row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), bricks.back()->GetScaleFloat3().y * p->GetCamera()->sphere.radius, column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z + 0.5f));
				p->SetStartPos(p->GetCamera()->GetPositionFloat3().x, p->GetCamera()->GetPositionFloat3().y, p->GetCamera()->GetPositionFloat3().z);
				break;
			default:
				break;
			}
		}
}
 
void Map::UpdateLogic(float dt, Player* p)
{
	int lengthGuards = guards.size();
	for (int i = 0; i < lengthGuards; i++)
	{
		guards[i]->UpdateLogic(dt, p, this);
	}

	int lengthCameras = secCamera.size();
	for (int i = 0; i < lengthCameras; i++)
	{
		secCamera[i]->UpdateLogic(dt, p);
	}
}

void Map::Draw(XMMATRIX _view, XMMATRIX _projection, AmbientLight* _ambLight, DirectionalLight* _dirLight, PointLight* _ptLight)
{   
	for (int i = 0; i < bricksNumber; i++)
	{
		XMVECTOR a = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		bricks[i]->GetBrick()->UpdateConstantBf(_view, _projection, bricks[i]->GetPositionVector(), bricks[i]->GetRotationVector(), bricks[i]->GetScaleVector(), a, _ambLight, _dirLight, _ptLight);
		bricks[i]->Draw();
	}

	//Render guard
	int lengthGuards = guards.size();
	for (int i = 0; i < lengthGuards; i++)
	{
		guards[i]->UpdateConstantBF(_view, _projection, _ambLight, _dirLight, _ptLight);
		guards[i]->Draw();
	}

	//Render security cameras
	int lengthCameras = secCamera.size();
	for (int i = 0; i < lengthCameras; i++)
	{
		secCamera[i]->UpdateConstantBF(_view, _projection);
		secCamera[i]->Draw(); 
	}
}

void Map::Clean()
{
	for (int i = 0; i < bricksNumber; i++)
	{
		delete bricks[i];
		bricks[i] = nullptr;
	}
	bricks.clear();

	int lengthGuards = guards.size();
	for (int i = 0; i < lengthGuards; i++)
	{
		delete guards[i];
		guards[i] = nullptr;
	}
	guards.clear(); 
}

void Map::PlaceGroundFloor(int _row, int _column, std::map<Constants::Models, ObjFileModel*>& models)
{
	bricks.push_back(new MapBrick(gfx, pDevice, pImmContext));
	bricks.back()->LoadObjModel(models[Constants::Models::CUBE], Constants::modelVS, Constants::modelPS, Constants::floor2TX);
	bricks.back()->box.CalculateMinAndMax(bricks.back()->GetBrick()->GetVertexBuffer());
	bricks.back()->Scale(3.0f, 3.0f, 3.0f);
	bricks.back()->SetPosition(_row * bricks.back()->GetScaleFloat3().x * (bricks.back()->box.maxBound.x - bricks.back()->box.minBound.x), 0.0f, _column * bricks.back()->GetScaleFloat3().z * (bricks.back()->box.maxBound.z - bricks.back()->box.minBound.z));
	bricks.back()->CalculateBoundingBoxWorldPos();
	bricksNumber++;
}

int Map::GetBrickNumber()
{
	return bricksNumber;
}

std::vector<MapBrick*>& Map::GetBricks()
{
	return bricks;
}

std::vector<SecurityCamera*>& Map::GetCameras()
{
	return secCamera;
}
 