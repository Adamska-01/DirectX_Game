#pragma once
#include "Graphics.h"
#include "Constants.h"


//Interface class
class Bindable
{
public:
	//Binds stuff to the pipeline
	virtual void Bind(Graphics* gfx, std::string _shader) = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext(Graphics* gfx);
	static ID3D11Device* GetDevice(Graphics* gfx); 
};