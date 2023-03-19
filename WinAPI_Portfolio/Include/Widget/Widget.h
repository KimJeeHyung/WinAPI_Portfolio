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
    class CScene* m_Scene;          // �ڽ��� �Ҽӵ� ��
    class CWidgetWindow* m_Owner;   // �ڽ��� ���� �ִ� ����������
    int     m_ZOrder;               // ���� ��� ������ ���� Z��
    Vector2 m_Pos;                  // ���� ��ġ
    Vector2 m_Size;                 // ���� ũ��
    bool    m_MouseHovered;         // ���콺�� �� ���� ���� �ö� �ִ���

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

