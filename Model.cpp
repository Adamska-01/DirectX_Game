#include "Model.h"

Model::Model(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
    :
    gfx(_gfx),
    pD3DDevice(_device),
    pImmediateContext(_immContext)
{
    position = rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

Model::~Model()
{
    /*if (pObject != nullptr)
    {
        delete pObject;
        pObject = nullptr;
    }*/
    //System clean up
    //if (pSampler != nullptr) pTexture->Release(); 
    if (pConstantBuffer != nullptr) pConstantBuffer->Release();
}

HRESULT Model::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
    HRESULT hr = S_OK;

    //Set shaders
    SetShaders(_VSshader, _PSshader); 
    //Set texture
    SetTexture(_texture);
    //Load model
    pObject = _obj; 
     
    //Set up and create constant buffer
    D3D11_BUFFER_DESC cBufDesc;
    ZeroMemory(&cBufDesc, sizeof(cBufDesc));
    cBufDesc.Usage = D3D11_USAGE_DEFAULT;   //can use UpdateSubresource() to update 
    cBufDesc.ByteWidth = 240;    //MUST be a multiple of 16, calculate from CB struct
    cBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    //Use as a constant buffer
    hr = pD3DDevice->CreateBuffer(&cBufDesc, NULL, &pConstantBuffer);
    if (FAILED(hr)) return hr; 

    return hr;
}

void Model::SetTexture(std::string _texture)
{ 
    texture = _texture;
}

void Model::UpdateConstantBf(XMMATRIX _view, XMMATRIX _projection, XMVECTOR _pos, XMVECTOR _rot, XMVECTOR _scale, XMVECTOR _clMod, AmbientLight* _ambLight, DirectionalLight* _dirLight, PointLight* _pointLight)
{
    //Set world matrix 
    XMMATRIX scale = XMMatrixScalingFromVector(_scale);
    XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(_rot);
    XMMATRIX pos = XMMatrixTranslationFromVector(_pos);

    XMMATRIX world = scale * rot * pos;

    //Update constant buffer
    MODEL_CONSTANT_BUFFER cb;
    if (_ambLight != nullptr)
        cb.ambientLightColour = _ambLight->GetColour();
    else
        cb.ambientLightColour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    if (_dirLight != nullptr)
    {
        cb.directionalLightColour = _dirLight->GetColour();
        cb.directionalLightVector = _dirLight->GetDirection(world);
    }    
    else
        cb.directionalLightColour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    //point light and Attenuation
    if (_pointLight != nullptr)
    {
        cb.pointLightPos = _pointLight->GetInversePos(world);
        cb.pointLightColour = _pointLight->GetColour();
        cb.pointLightAttenuation = _pointLight->GetAttenuation(); //constant falloff factor
        cb.range = _pointLight->GetRange();
    } 
    else
        cb.pointLightColour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    cb.colourModifier = _clMod;
    cb.worldView = world * _view;
    cb.worldViewProjection = world * _view * _projection;
    //Set constant buffer to activa state
    pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
    //upload the new values for the constant buffer 
    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, 0, &cb, 0, 0);
} 

void Model::Draw()
{
    //Set shaders and input layout
    VertexShader::GetInstance()->Bind(gfx, vsShader);
    PixelShader::GetInstance()->Bind(gfx, psShader); 

    //Set sample states and texture
    Textures::GetInstance()->Bind(gfx, texture);  

    pObject->Draw();
} 

void Model::SetShaders(std::string _vs, std::string _ps)
{
    vsShader = _vs;
    psShader = _ps;
} 

ObjFileModel* Model::GetVertexBuffer()
{
    return pObject;
}
