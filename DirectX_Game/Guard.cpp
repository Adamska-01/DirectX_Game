#include "Guard.h"
#include "CollisionHandler.h"
#include <algorithm>

Guard::Guard(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
    :
    model(new Model(_gfx, _device, _immContext))
{
    //Stats
    health = 100.0f;
    speed = 13.0f;
    damagePerSecond = 5.0f;

    modColour = false;
    startPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

    state = States::PATROLLING;

    //Time-based variables 
    currentTimePatrol = 0.0f; 
    currentTimeColourMod = 0.0f;
    currentTimeRespawn = 0.0f;
    intervalPatrol = 3.0f; 
    intervalToRespawn = 2.0f;
    intervalColourMod = 0.2f;

    minAlertDistance = 20.0f; 
}

Guard::~Guard()
{
    if (model != nullptr)
    {
        delete model;
        model = nullptr;
    }
}

void Guard::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
    model->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
}

void Guard::UpdateLogic(float dt, Player* p, Map* _map)
{ 
    if (state == States::PATROLLING)
    {
        AdjustPosition(GetForwardVector() * dt * std::abs(speed));
       
        currentTimePatrol += dt;
        if (currentTimePatrol >= intervalPatrol) //Change dir after some time 
        {
            speed *= -1;
            currentTimePatrol = 0.0f;
            XMFLOAT3 lookat; XMStoreFloat3(&lookat, (GetPositionVector() + (GetForwardVector() * speed)));
            SetLookAtPos(lookat);
        }
    }
    else if (state == States::ATTACK)
    {
        //Check if it can attack
        p->GetCamera()->CalculateBoundingSphereWorldPos();
        CalculateBoundingSphereWorldPos();
        if(CollisionHandler::SphereToSphereCollision(p->GetCamera()->sphere, sphere))
        { 
            //Attack player
            p->DealDamageToSelf(damagePerSecond * dt); 
        }
        else
        {
            //Follow the player 
            SetLookAtPos(p->GetCamera()->GetPositionFloat3());
            AdjustPosition(GetForwardVector() * dt * std::abs(speed));
        }
    }
    else if (state == States::GOTOSTART)
    {
        //Go to start pos 
        SetLookAtPos(startPos);
        AdjustPosition(GetForwardVector() * dt * std::abs(speed));
    }

    //Check collisions with map and change direction
    CalculateBoundingSphereWorldPos();
    int length = _map->GetBrickNumber();
    for (int i = 0; i < length; i++)
    {
        if (CollisionHandler::SphereToBoxCollision(sphere, _map->GetBricks()[i]->box))
        {
            speed *= -1; 
            XMFLOAT3 lookat; XMStoreFloat3(&lookat, (GetPositionVector() + (GetForwardVector() * speed)));
            SetLookAtPos(lookat);
            currentTimePatrol = 0.0f;
            break;
        }
    }

    //Check collisions with projectiles 
    CheckCollisionAndDamage(p->GetProjectiles());

    //Respawn if dead
    if (IsDead()) 
    {
        SetPosition(startPos.x, -100.0f, startPos.z);
        currentTimeRespawn += dt;
        if (currentTimeRespawn >= intervalToRespawn)
        {
            SetPosition(startPos.x, startPos.y, startPos.z);
            XMFLOAT3 lookat; XMStoreFloat3(&lookat, (GetPositionVector() + (GetForwardVector() * speed)));
            SetLookAtPos(lookat);
            state = States::PATROLLING;
            health = 100.0f; 
            currentTimePatrol = 0.0f; 
            currentTimeColourMod = 0.0f;
            currentTimeRespawn = 0.0f; 
        }
    }

    //Allow to modify the colour if the guard is shot by the player 
    if (modColour)
    {
        currentTimeColourMod += dt;
        if (currentTimeColourMod >= intervalColourMod)
        {
            modColour = false;
            currentTimeColourMod = 0.0f;
        }
    }

    //AI
    AssignState(p);
}

void Guard::AssignState(Player* p)
{
    float distanceFromPlayer = (float)sqrt(pow((p->GetCamera()->GetPositionFloat3().x - pos.x), 2) + pow((p->GetCamera()->GetPositionFloat3().y - pos.y), 2) + pow((p->GetCamera()->GetPositionFloat3().z - pos.z), 2));
    if (state == States::PATROLLING || state == States::GOTOSTART)
    {
        //Attack if the player is too close 
        if (distanceFromPlayer <= minAlertDistance)
        {
            state = States::ATTACK; 
        }
    }
    if (state == States::ATTACK)
    {
        //Go back to starting pos if player is too far
        if (distanceFromPlayer > minAlertDistance)
        {
            state = States::GOTOSTART;
        }
    }
    if (state == States::GOTOSTART && distanceFromPlayer > minAlertDistance)
    {
        float distanceFromStart = (float)sqrt(pow((startPos.x - pos.x), 2) + pow((startPos.y - pos.y), 2) + pow((startPos.z - pos.z), 2));

        //Go to patrolling state if the player is too far
        if (distanceFromStart <= 0.05f)
        {
            state = States::PATROLLING;
            currentTimePatrol = 0.0f;
        }
    }
}

void Guard::DealDamageToSelf(float _dmg)
{
    health -= _dmg;
    health = std::clamp(health, 0.0f, 100.0f);
}

void Guard::CheckCollisionAndDamage(std::vector<Projectile*>const & _projectiles)
{
    CalculateBoundingSphereWorldPos();
    int length = _projectiles.size();
    for (int i = 0; i < length; i++)
    {
        _projectiles[i]->CalculateBoundingSphereWorldPos();
        if(CollisionHandler::SphereToSphereCollision(sphere, _projectiles[i]->sphere))
        {
            //Deal damage to self and sets projectile destruction
            DealDamageToSelf(_projectiles[i]->GetDamage());
            _projectiles[i]->SetDestruction(true);
            //Modify colour if the player hits the shot
            modColour = true;
        }
    }
}

void Guard::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection, AmbientLight* _ambLight, DirectionalLight* _dirLight, PointLight* _pointLight)
{
    if (modColour)
    {
        XMVECTOR colourMod = XMVectorSet(2.0f, 2.0f, 1.0f, 1.0f); 
        model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod, _ambLight, _dirLight, _pointLight);
    }
    else
    {
        XMVECTOR colourMod = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector, colourMod, _ambLight, _dirLight, _pointLight);
    }
}

void Guard::Draw()
{
    model->Draw();
}

bool Guard::IsDead()
{
    return health <= 0;
}

Model* Guard::GetModel()
{
    return model;
}

XMFLOAT3 Guard::GetStartPos()
{
    return startPos;
}

void Guard::SetStartPos(float _x, float _y, float _z)
{
    startPos.x = _x;
    startPos.y = _y;
    startPos.z = _z;
}
