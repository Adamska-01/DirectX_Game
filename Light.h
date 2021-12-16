#pragma once 
#include <d3d11.h> 
#include <dxerr.h>
#include <DirectXMath.h> 
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT 

using namespace DirectX;

class Light
{
private:
	XMVECTOR positionV;
	XMVECTOR rotationV;

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;

	XMVECTOR colour;

public:
	Light();
	~Light() {};

	//Transformations
	void Translate(const XMVECTOR _pos);
	void Translate(float _x, float _y, float _z);
	void Rotate(const XMVECTOR _rot);
	void Rotate(float _x, float _y, float _z);

	//Getters
	XMFLOAT3 GetPositionFloat3() const;
	XMVECTOR GetPositionVector();
	XMFLOAT3 GetRotationFloat3() const;
	XMVECTOR GetRotationVector();
	XMVECTOR GetColour() const;

	//Setters
	void SetColour(float _r, float _g, float _b, float _a);
};

