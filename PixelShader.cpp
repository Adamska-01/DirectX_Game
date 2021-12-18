#include "PixelShader.h"

PixelShader* PixelShader::Instance = nullptr;

HRESULT PixelShader::SetShader(Graphics* gfx, std::string _shader)
{
	HRESULT hr = S_OK;

	if (PSmap[_shader] == nullptr)
	{
		ID3DBlob* PS, * error;
		hr = D3DX11CompileFromFile("PixelShader.hlsl",
			0,
			0,
			_shader.c_str(),
			"ps_4_0",
			0,
			0,
			0,
			&PS,
			&error,
			0
		);

		if (error != 0)
		{
			OutputDebugString((char*)error->GetBufferPointer());
			error->Release();
			if (FAILED(hr)) return hr;
		}

		//Create shader objects
		hr = GetDevice(gfx)->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PSmap[_shader]);
		if (FAILED(hr)) return hr;

		PS->Release();
	}

	return hr;
}

void PixelShader::Bind(Graphics* gfx, std::string _shader)
{
	GetContext(gfx)->PSSetShader(PSmap[_shader], nullptr, 0u);
}

void PixelShader::Clean()
{
	std::map<std::string, ID3D11PixelShader*>::iterator it;
	for (it = PSmap.begin(); it != PSmap.end(); it++)
		if (it->second != nullptr) it->second->Release();

	PSmap.clear();
}
