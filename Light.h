#pragma once  
#include "GameObject.h"

using namespace DirectX;

class Light : public GameObject
{  
protected:  
	XMVECTOR colour;

public:
	Light();
	~Light() {}; 

	//Getters
	XMVECTOR GetColour() const;

	//Setters
	void SetColour(float _r, float _g, float _b, float _a);
};

