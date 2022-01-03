#pragma once
#include "Map.h"


class GameManager
{ 
private: 
	static int cameras; 
	
public:
	GameManager(int _camNumber);

	static bool HasWon();


	//Setters
	static void SetCamerasNumber(int _value);

	//Getters 
	static int GetCamerasNumber();
};