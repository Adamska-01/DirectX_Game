#pragma once
#include "Bindable.h"
#include <string>
#include <map> 


class Textures : public Bindable
{  
private:
	ID3D11SamplerState* pSampler = nullptr;
	std::map<std::string, ID3D11ShaderResourceView*> textureMap;

private:
	Textures() {};
	static Textures* Instance;
public:
	inline static Textures* GetInstance() {
		return Instance = (Instance != nullptr) ? Instance : new Textures();
	}
	HRESULT SetTexture(Graphics* gfx, std::string _texture);
	void Bind(Graphics* gfx, std::string _texture)  override;

	void Clean();

public:
	//Getters
	inline std::map<std::string, ID3D11ShaderResourceView*> GetTextureMap() { return textureMap; };
};