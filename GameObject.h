#pragma once
#include <d3d11.h>
#include <dxerr.h>
#include <DirectXMath.h>
#define XM_NO_INTRINSICS_   //Use less optimal but more compatible code 
#define XM_NO_ALLIGNMENT_

using namespace DirectX;

class GameObject
{
protected:	//Transform data 
	XMVECTOR posVector;			//16 bytes
	XMVECTOR rotVector;			//16 bytes
	XMFLOAT3 rot;				//12 bytes 
	XMFLOAT3 pos;				//12 bytes  
protected:	//vector constants 
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
protected:
	XMVECTOR vec_forward;
	XMVECTOR vec_backward;
	XMVECTOR vec_right;
	XMVECTOR vec_left;

public:
	GameObject() = default;
	~GameObject() = default;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetLeftVector();

	//Setters
	virtual void SetPosition(const XMVECTOR& _pos);
	virtual void SetPosition(float _x, float _y, float _z);
	virtual void AdjustPosition(const XMVECTOR& _pos);
	virtual void AdjustPosition(float _x, float _y, float _z);
	virtual void SetRotation(const XMVECTOR& _rot);
	virtual void SetRotation(float _x, float _y, float _z);
	virtual void AdjustRotation(const XMVECTOR& _rot);
	virtual void AdjustRotation(float _x, float _y, float _z);
	virtual void SetLookAtPos(XMFLOAT3 _lookAtPos);

private:
	void UpdateVectors();
};