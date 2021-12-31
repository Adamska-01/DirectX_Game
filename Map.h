#pragma once
#include "IOFunctions.h"
#include "MapBrick.h"
#include "Guard.h"
#include "SecurityCamera.h"

class Guard;
class Player;
class SecurityCamera;

class Map
{
private:  //Subsystem stuff
	Graphics* gfx = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pImmContext = nullptr;
private:
	//Map layout
	std::vector<std::string> gridMap; 
	//Bricks and other things in the map 
	std::vector<MapBrick*> bricks; 
	std::vector<Guard*> guards;
	std::vector<SecurityCamera*> secCamera;

	//offset values 
	int offset{ 0 };  
	int width{ 0 }, height{ 0 }, bricksNumber{ 0 }; 

public:
	Map(std::string filePath, Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _context);
	~Map();
	  
	void LoadMap(std::map<Constants::Models, ObjFileModel*>& models, Player* p);

	void UpdateLogic(float dt, Player* p);
	void Draw(XMMATRIX _view, XMMATRIX _projection, AmbientLight* _ambLight, DirectionalLight* _dirLight, PointLight* _ptLight);

private:
	void Clean(); 
	void PlaceGroundFloor(int _row, int _column, std::map<Constants::Models, ObjFileModel*>& models);
public:
	//Getters
	int GetBrickNumber();
	std::vector<MapBrick*>& GetBricks();
	std::vector<SecurityCamera*>& GetCameras();
};


