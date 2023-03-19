#pragma once
#include "../Ref.h"
class CWidget :
    public CRef
{
    friend class CWidgetWindow;
    friend class CWidgetComponent;

protected:
    CWidget();
    CWidget(const CWidget& widget);
    virtual ~CWidget();

protected:
    class CScene* m_Scene;          // 자신이 소속된 씬
    class CWidgetWindow* m_Owner;   // 자신을 갖고 있는 위젯윈도우
    int     m_ZOrder;               // 위젯 출력 순서를 위한 Z값
    Vector2 m_Pos;                  // 위젯 위치
    Vector2 m_Size;                 // 위젯 크기
    bool    m_MouseHovered;         // 마우스가 이 위젯 위에 올라가 있는지

public:
    const Vector2& GetPos() const
    {
        return m_Pos;
    }

    const Vector2& GetSize() const
    {
        return m_Size;
    }

    int GetZOrder() const
    {
        return m_ZOrder;
    }

public:
    void SetPos(float x, float y)
    {
        m_Pos.x = x;
        m_Pos.y = y;
    }

    void SetPos(const Vector2& Pos)
    {
        m_Pos = Pos;
    }

    void SetSize(float x, float y)
    {
        m_Size.x = x;
        m_Size.y = y;
    }

    void SetSize(const Vector2& Size)
    {
        m_Size = Size;
    }

    void SetZOrder(int ZOrder)
    {
        m_ZOrder = ZOrder;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);

public:
    bool CollisionMouse(const Vector2& Pos);

public:
    virtual void CollisionMouseHoveredCallback(const Vector2& Pos);
    virtual void CollisionMouseReleaseCallback();
};

