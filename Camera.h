#pragma once 
#include "GameObject.h"
#include <math.h>
 
 
class Camera : public GameObject
{
private: 
	XMMATRIX viewMatrix;		//64 bytes
	XMMATRIX projectionMatrix;  //64 bytes  

public:
	Camera();
	void SetProjectionValues(float _fovDegrees, float _aspectRatio, float _nearZ, float _farZ);
	
	//Getters
	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjetionMatrix() const;
	//Setters
	void SetPosition(const XMVECTOR& _pos) override;
	void SetPosition(float _x, float _y, float _z) override;
	void AdjustPosition(const XMVECTOR& _pos) override;
	void AdjustPosition(float _x, float _y, float _z) override;
	void SetRotation(const XMVECTOR& _rot) override;
	void SetRotation(float _x, float _y, float _z) override;
	void AdjustRotation(const XMVECTOR& _rot) override;
	void AdjustRotation(float _x, float _y, float _z) override;
	void SetLookAtPos(XMFLOAT3 _lookAtPos) override;

private: 
	void UpdateViewMatrix(); 
};