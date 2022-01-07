
#include "GameManager.h"

int GameManager::cameras = 0; 

GameManager::GameManager(int _camNumber) 
{ 
	cameras = _camNumber;  
}

bool GameManager::HasWon()
{
	return cameras <= 0;
}

void GameManager::SetCamerasNumber(int _value)
{
	cameras = _value;
}

int GameManager::GetCamerasNumber()
{
	return cameras;
}
