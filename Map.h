#pragma once
#include "IOFunctions.h"
#include "MapBrick.h"
#include "Guard.h"


class Map
{
private:  
	//Grids
	std::vector<std::string> gridMap; 
	std::vector<MapBrick*> bricks; 
	Guard* guard = nullptr;

	int offset{ 0 };  
	int width{ 0 }, height{ 0 }, bricksNumber{ 0 };

	Graphics* gfx = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pImmContext = nullptr;
	 
public:
	Map(std::string filePath, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _context);
	~Map();
	  
	void LoadMap(std::map<Constants::Models, ObjFileModel*>& models);

	void UpdateLogic(float dt, Player* p);
	void Draw(XMMATRIX _view, XMMATRIX _projection);

	void Clean(); 

public:
	int GetBrickNumber();
	std::vector<MapBrick*>& GetBricks();
};


