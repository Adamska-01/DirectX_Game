#pragma once
#include "Bindable.h"
#include <string>
#include <map> 


class PixelShader : public Bindable
{
public:
	inline static PixelShader* GetInstance() {
		return Instance = (Instance != nullptr) ? Instance : new PixelShader();
	}
	HRESULT SetShader(Graphics* gfx, std::string _shader);
	void Bind(Graphics* gfx, std::string _shader)  override;
	 
	void Clean();

	//Getters
	inline std::map<std::string, ID3D11PixelShader*> GetShaderMap() { return PSmap; };

private:
	PixelShader() {};
	static PixelShader* Instance;

protected:
	std::map<std::string, ID3D11PixelShader*> PSmap;
};