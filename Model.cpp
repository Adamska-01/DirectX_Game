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
    cBufDesc.ByteWidth = 224;    //MUST be a multiple of 16, calculate from CB struct
    cBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    //Use as a constant buffer
    hr = pD3DDevice->CreateBuffer(&cBufDesc, NULL, &pConstantBuffer);
    if (FAILED(hr)) return hr;

    //Calculate bounding sphere
    CalculateModelCentrePoint();
    CalculateBoundingSphereRadius();

    return hr;
}

void Model::SetTexture(std::string _texture)
{ 
    texture = _texture;
}

void Model::UpdateConstantBf(XMMATRIX _view, XMMATRIX _projection, AmbientLight* _ambLight, DirectionalLight* _dirLight, PointLight* _pointLight)
{
    //Set world matrix 
    XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMMATRIX pos = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX world = scale * rot * pos;

    //Update constant buffer
    MODEL_CONSTANT_BUFFER cb;
    if (_ambLight != nullptr)
        cb.ambientLightColour = _ambLight->GetColour(); 
    if (_dirLight != nullptr)
    {
        cb.directionalLightColour = _dirLight->GetColour();
        cb.directionalLightVector = _dirLight->GetDirection(world);
    }     
    //point light and Attenuation
    if (_pointLight != nullptr)
    {
        cb.pointLightPos = _pointLight->GetInversePos(world);
        cb.pointLightColour = _pointLight->GetColour();
        cb.pointLightAttenuation = _pointLight->GetAttenuation(); //constant falloff factor
        cb.range = _pointLight->GetRange();
    } 
    cb.worldView = world * _view;
    cb.worldViewProjection = world * _view * _projection;
    //Set constant buffer to activa state
    pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
    //upload the new values for the constant buffer 
    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, 0, &cb, 0, 0);
} 

void Model::LookAt_XYZ(float _x, float _y, float _z)
{
    dx = _x - position.x;
    dy = _y - position.y;
    dz = _z - position.z;

    rotation.y = atan2(dx, dz);
    rotation.x = -atan2(dy, sqrt((_x - position.x) * (_x - position.x) + (_z - position.z) * (_z - position.z)));
}

void Model::CalculateModelCentrePoint()
{
    XMFLOAT3 minBound = { 0.0f,0.0f,0.0f };
    XMFLOAT3 maxBound = { 0.0f,0.0f,0.0f };
    for (int i = 0; i < pObject->numverts; i++)
    {
        //X
        if (pObject->vertices[i].Pos.x > maxBound.x)
            maxBound.x = pObject->vertices[i].Pos.x;
        if (pObject->vertices[i].Pos.x < minBound.x)
            minBound.x = pObject->vertices[i].Pos.x;
           
        //Y
        if (pObject->vertices[i].Pos.y > maxBound.y)
            maxBound.y = pObject->vertices[i].Pos.y;
        if (pObject->vertices[i].Pos.y < minBound.y)
            minBound.y = pObject->vertices[i].Pos.y;

        //Z
        if (pObject->vertices[i].Pos.z > maxBound.z)
            maxBound.z = pObject->vertices[i].Pos.z;
        if (pObject->vertices[i].Pos.z < minBound.z)
            minBound.z = pObject->vertices[i].Pos.z;
    }

    //Calculate model centre
    XMFLOAT3 maxMinusmin = { (maxBound.x - minBound.x) / 2, (maxBound.y - minBound.y) / 2, (maxBound.z - minBound.z) / 2 };
    XMFLOAT3 minPlusMaxMinusMin = { minBound.x + maxMinusmin.x,  minBound.y + maxMinusmin.y, minBound.z + maxMinusmin.z };
    
    colSphereCentre = minPlusMaxMinusMin; 
}

void Model::CalculateBoundingSphereRadius()
{ 
    for (int i = 0; i < pObject->numverts; i++)
    {
        float rad = sqrt(pow((colSphereCentre.x - pObject->vertices[i].Pos.x), 2) + pow((colSphereCentre.y - pObject->vertices[i].Pos.y), 2) + pow((colSphereCentre.z - pObject->vertices[i].Pos.z), 2));
        
        if (colSphereRadius < rad)
            colSphereRadius = rad;
    } 
}

XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
    //Set world matrix 
    XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMMATRIX pos = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX world = scale * rot * pos;

    XMVECTOR offset = XMLoadFloat3(&colSphereCentre);
    offset = XMVector3Transform(offset, world);
    
    return offset;
}

float Model::GetBoundingSphereRadius()
{
    return colSphereRadius * scale.x; //Does not support xyz scale
}

bool Model::CheckCollision(Model* _model)
{
    if (_model == this)
        return false;

    XMVECTOR spherePos = GetBoundingSphereWorldSpacePosition();
    XMVECTOR _spherePos = _model->GetBoundingSphereWorldSpacePosition();

    XMFLOAT3 pos; XMStoreFloat3(&pos, spherePos);
    XMFLOAT3 _pos; XMStoreFloat3(&_pos, _spherePos);

    float distance = sqrt(pow((pos.x - _pos.x), 2) + pow((pos.y - _pos.y), 2) + pow((pos.z - _pos.z), 2));

    float radius = GetBoundingSphereRadius();
    float _radius = _model->GetBoundingSphereRadius();

    if (distance <= (radius + _radius))
        return true;

    return false;
}

void Model::SetShaders(std::string _vs, std::string _ps)
{
    vsShader = _vs;
    psShader = _ps;
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


void Model::MoveForwardY(float _speed)
{
    XMMATRIX modelRot = XMMatrixRotationRollPitchYaw(0.0f, rotation.y, 0.0f);

    XMVECTOR forwardVec = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), modelRot);
    forwardVec *= _speed;
    XMFLOAT3 movement; XMStoreFloat3(&movement, forwardVec);

    position.x += movement.x;
    position.z += movement.z;

}

void Model::MoveForwardXY(float _speed)
{
    XMMATRIX modelRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

    XMVECTOR forwardVec = XMVector3Normalize(XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), modelRot));
    forwardVec *= _speed;
    XMFLOAT3 movement; XMStoreFloat3(&movement, forwardVec);

    position.x += movement.x;
    position.y += movement.y;
    position.z += movement.z;
}

void Model::MoveBackwardsXY(float _speed)
{
    XMMATRIX modelRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

    XMVECTOR forwardVec = XMVector3Normalize(XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), modelRot));
    forwardVec *= _speed;
    XMFLOAT3 movement; XMStoreFloat3(&movement, forwardVec);

    position.x += movement.x;
    position.y += movement.y;
    position.z += movement.z;
}

//Transformations
void Model::Translate(const XMVECTOR& _pos)
{
    XMFLOAT3 trans = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMStoreFloat3(&trans, _pos);
    position.x += trans.x;
    position.y += trans.y;
    position.z += trans.z;
}

void Model::Translate(float _x, float _y, float _z)
{
    position.x += _x;
    position.y += _y;
    position.z += _z;
}

void Model::Rotate(const XMVECTOR& _rot)
{
    XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMStoreFloat3(&rot, _rot);
    rotation.x += rot.x;
    rotation.y += rot.y;
    rotation.z += rot.z;
}

void Model::Rotate(float _x, float _y, float _z)
{
    rotation.x += _x;
    rotation.y += _y;
    rotation.z += _z;
}

void Model::Scale(const XMVECTOR& _scale)
{
    XMStoreFloat3(&scale, _scale);
}

void Model::Scale(float _x, float _y, float _z)
{
    scale = XMFLOAT3(_x, _y, _z);
}

void Model::SetModel(ObjFileModel* _obj)
{
    pObject = _obj;
}

void Model::SetPosition(const XMVECTOR& _pos)
{
    XMStoreFloat3(&position, _pos);
}

void Model::SetPosition(float _x, float _y, float _z)
{
    position = XMFLOAT3(_x, _y, _z);
}

void Model::SetRotation(const XMVECTOR& _rot)
{
    XMStoreFloat3(&rotation, _rot);
}

void Model::SetRotation(float _x, float _y, float _z)
{
    rotation = XMFLOAT3(_x, _y, _z);
}

const XMFLOAT3& Model::GetPosition()
{
    return position;
}

const XMFLOAT3& Model::GetRotation()
{
    return rotation;
}

const XMFLOAT3& Model::GetScale()
{
    return scale;
}

ObjFileModel* Model::GetModel()
{
    return pObject;
}
