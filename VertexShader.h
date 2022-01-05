#pragma once
#include "Bindable.h"
#include <string>
#include <map> 
 

class VertexShader : public Bindable
{
public:
	struct mapData
	{
		ID3D11VertexShader* vsShader;
		ID3D11InputLayout* inputLayout;
	};
protected: 
	std::map<std::string, mapData> VSmap;

private:
	VertexShader() {};
	static VertexShader* Instance;
public:
	inline static VertexShader* GetInstance() {
		return Instance = (Instance != nullptr) ? Instance : new VertexShader(); 
	}

	HRESULT SetShaderAndIL(Graphics* gfx, std::string _shader, D3D11_INPUT_ELEMENT_DESC il[], int _arraySize);
	void Bind(Graphics* gfx, std::string _shader)  override;
	  
	void Clean();

public:
	//Getters
	inline std::map<std::string, mapData> GetShaderMap() { return VSmap; };
};