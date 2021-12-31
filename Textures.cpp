#include "Textures.h"

Textures* Textures::Instance = nullptr;

HRESULT Textures::SetTexture(Graphics* gfx, std::string _texture)
{
    HRESULT hr = S_OK;

    if (textureMap[_texture] == nullptr)
    {
        //Create Texture 
        hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(gfx), _texture.c_str(), NULL, NULL, &textureMap[_texture], NULL);
        if (FAILED(hr)) return hr;
    }

    //Create Sampler
    if (pSampler == nullptr)
    {
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler);
    }

    return hr;
}

void Textures::Bind(Graphics* gfx, std::string _texture)
{
    //Set sampler and shader 
    GetContext(gfx)->PSSetSamplers(0, 1, &pSampler);
    GetContext(gfx)->PSSetShaderResources(0, 1, &textureMap[_texture]);
}

void Textures::Clean()
{
    std::map<std::string, ID3D11ShaderResourceView*>::iterator it;
    for (it = textureMap.begin(); it != textureMap.end(); it++) 
        if(it->second != nullptr) it->second->Release(); 
    textureMap.clear();

    //Release sampler
    if(pSampler != nullptr) pSampler->Release();
}
