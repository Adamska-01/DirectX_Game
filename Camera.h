#pragma once 
#include <d3d11.h>
#include <dxerr.h>
#include <DirectXMath.h>
#define XM_NO_INTRINSICS_   //Use less optimal but more compatible code 
#define XM_NO_ALLIGNMENT_
#include <math.h>

using namespace DirectX;
 
class Camera
{
private:
	XMVECTOR posVector;			//16 bytes
	XMVECTOR rotVector;			//16 bytes
	XMFLOAT3 rot;				//12 bytes 
	XMFLOAT3 pos;				//12 bytes  
	XMMATRIX viewMatrix;		//64 bytes
	XMMATRIX projectionMatrix;  //64 bytes 
private:
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_backward;
	XMVECTOR vec_right;
	XMVECTOR vec_left;

public:
	Camera();
	void SetProjectionValues(float _fovDegrees, float _aspectRatio, float _nearZ, float _farZ);
	  
	//Getters
	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjetionMatrix() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetLeftVector();

	//Setters
	void SetPosition(const XMVECTOR& _pos);
	void SetPosition(float _x, float _y, float _z);
	void AdjustPosition(const XMVECTOR& _pos);
	void AdjustPosition(float _x, float _y, float _z);
	void SetRotation(const XMVECTOR& _rot);
	void SetRotation(float _x, float _y, float _z);
	void AdjustRotation(const XMVECTOR& _rot);
	void AdjustRotation(float _x, float _y, float _z);
	void SetLookAtPos(XMFLOAT3 _lookAtPos);

private: 
	void UpdateViewMatrix(); 
};