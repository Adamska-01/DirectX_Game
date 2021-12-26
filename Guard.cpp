#include "Guard.h"
#include "CollisionHandler.h"

Guard::Guard(Graphics* _gfx, ID3D11Device* _device, ID3D11DeviceContext* _immContext)
    :
    model(new Model(_gfx, _device, _immContext))
{
    health = 100.0f;
    speed = 0.5f;

    state = States::PATROLLING;
    minAlertDistance = 8.5f;

    currentTimePatrol = 0.0f;
    currentTimeAttack = 0.0f;
    intervalPatrol = 4.0f;
    intervalAttack = 2.0f;
}

Guard::~Guard()
{
}

void Guard::LoadObjModel(ObjFileModel* _obj, std::string _VSshader, std::string _PSshader, std::string _texture)
{
    model->LoadObjModel(_obj, _VSshader, _PSshader, _texture);
}

void Guard::UpdateLogic(float dt, Player* p)
{ 
    if (state == States::PATROLLING)
    {
        AdjustPosition(GetForwardVector() * dt * speed);
        currentTimePatrol += dt;
        if (currentTimePatrol >= intervalPatrol)
        {
            speed *= -1;
            currentTimePatrol = 0;
        }
    }
    else if (state == States::ATTACK)
    {
        //Check if it can attack
        p->GetCamera()->CalculateBoundingSphereWorldPos();
        CalculateBoundingSphereWorldPos();
        if(CollisionHandler::SphereToSphereCollision(p->GetCamera()->sphere, sphere))
        {
            currentTimeAttack += dt;
            if (currentTimeAttack >= intervalAttack)
            {
                //TODO: attack player

                currentTimeAttack = 0;
            }
        }
        else
        {
            //Follow the player 
            SetLookAtPos(p->GetCamera()->GetPositionFloat3());
            AdjustPosition(GetForwardVector());
        }
    }
    else if (state == States::GOTOSTART)
    {
        //Go to start pos 
        SetLookAtPos(startPos);
        AdjustPosition(GetForwardVector());
    }

    if (IsDead()) //Respawn
    {
        SetPosition(startPos.x, startPos.y, startPos.z);
        state = States::PATROLLING;
    }

    AssignState(p);
}

void Guard::AssignState(Player* p)
{
    float distance = sqrt(pow((p->GetCamera()->GetPositionFloat3().x - pos.x), 2) + pow((p->GetCamera()->GetPositionFloat3().y - pos.y), 2) + pow((p->GetCamera()->GetPositionFloat3().z - pos.z), 2));
    if (state == States::PATROLLING || state == States::GOTOSTART)
    {
        if (distance <= minAlertDistance)
        {
            state = States::ATTACK;
        }
    }
    else if (state == States::ATTACK)
    {
        if (distance > minAlertDistance)
        {
            state = States::GOTOSTART;
        }
    }
    else if (state == States::GOTOSTART)
    {
        float distanceFromStart = sqrt(pow((startPos.x - pos.x), 2) + pow((startPos.y - pos.y), 2) + pow((startPos.z - pos.z), 2));

        if (distanceFromStart <= 0.1f)
        {
            state = States::PATROLLING;
        }
    }
}

void Guard::UpdateConstantBF(XMMATRIX _view, XMMATRIX _projection)
{
    model->UpdateConstantBf(_view, _projection, posVector, rotVector, scaleVector);
}

void Guard::Draw()
{
    model->Draw();
}

bool Guard::IsDead()
{
    return health <= 0;
}
