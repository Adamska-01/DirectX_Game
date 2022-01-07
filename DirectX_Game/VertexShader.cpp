#include "VertexShader.h" 
#include "Debug.h"

VertexShader* VertexShader::Instance = nullptr;

HRESULT VertexShader::SetShaderAndIL(Graphics* gfx, std::string _shader, D3D11_INPUT_ELEMENT_DESC il[], int _arraySize)
{
	HRESULT hr = S_OK;

	if (VSmap[_shader].vsShader == nullptr && VSmap[_shader].inputLayout == nullptr)
	{
		ID3DBlob* VS, * error;
		hr = D3DX11CompileFromFile("VertexShader.hlsl",
			0,
			0,
			_shader.c_str(),
			"vs_4_0",
			0,
			0,
			0,
			&VS,
			&error,
			0
		);

		if (error != 0)
		{
			OutputDebugString((char*)error->GetBufferPointer());
			error->Release();
#if _DEBUG
			DBG_ASSERT_MSG_H(hr);
#endif
		}
		std::string s = "fd";
		 
		//Create shader objects
		hr = GetDevice(gfx)->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &VSmap[_shader].vsShader);
#if _DEBUG
		DBG_ASSERT_MSG_H(hr);
#endif

		//Create input layout  
		hr = GetDevice(gfx)->CreateInputLayout(il, _arraySize, VS->GetBufferPointer(), VS->GetBufferSize(), &VSmap[_shader].inputLayout);
#if _DEBUG
		DBG_ASSERT_MSG_H(hr);
#endif

		VS->Release();
	}

	return hr;
}

void VertexShader::Bind(Graphics* gfx, std::string _shader)
{
	//Set shader and input layout
	GetContext(gfx)->VSSetShader(VSmap[_shader].vsShader, nullptr, 0u);
	GetContext(gfx)->IASetInputLayout(VSmap[_shader].inputLayout);
}

void VertexShader::Clean()
{
	std::map<std::string, mapData>::iterator it;
	for (it = VSmap.begin(); it != VSmap.end(); it++)
	{
		if (it->second.vsShader != nullptr) it->second.vsShader->Release();
		if (it->second.inputLayout != nullptr) it->second.inputLayout->Release();
	}

	VSmap.clear();
}
