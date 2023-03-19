#pragma once
#include "Collider.h"
class CColliderCircle :
    public CCollider
{
    // GameObject에 달아줄 수 있도록 friend 처리한다.
    friend class CGameObject;

protected:
    CColliderCircle();
    CColliderCircle(const CColliderCircle& collider);
    virtual ~CColliderCircle();

protected:
    CircleInfo m_Info;

public:
    CircleInfo GetInfo() const
    {
        return m_Info;
    }

    // 충돌체의 크기를 정해준다.
    void SetRadius(float Radius)
    {
        m_Info.Radius = Radius;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& Mouse);
};

