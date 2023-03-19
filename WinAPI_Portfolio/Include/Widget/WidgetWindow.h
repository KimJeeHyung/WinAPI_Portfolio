#pragma once
#include "../Ref.h"
#include "Widget.h"

class CWidgetWindow :
    public CRef
{
    friend class CScene;

protected:
    CWidgetWindow();
    virtual ~CWidgetWindow();

protected:
    class CScene*   m_Scene;    // 자신이 속해 있는 씬
    int             m_ZOrder;   // 출력 순서를 위한 Z값
    std::vector<CSharedPtr<class CWidget>> m_vecWidget; // 이 윈도우가 가지고 있는 위젯(UI) 목록

    Vector2 m_Pos;
    Vector2 m_Size;

public:
    class CWidget* GetWidget(int Index)
    {
        return m_vecWidget[Index];
    }

    int GetWidgetCount() const
    {
        return (int)m_vecWidget.size();
    }

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

public:
    void SortCollision();
    // 충돌한 위젯을 리턴할 수 있도록 이중포인터로 인자를 넣는다.
    bool CollisionMouse(class CWidget** Widget, const Vector2& Pos);

public:
    template <typename T>
    T* FindWidget(const std::string& Name)
    {
        size_t Size = m_vecWidget.size();

        for (size_t i = 0; i < Size; i++)
        {
            if (m_vecWidget[i]->GetName() == Name)
                return (T*)m_vecWidget[i];
        }

        return nullptr;
    }

    template <typename T>
    T* CreateWidget(const std::string& Name)
    {
        T* Widget = new T;

        Widget->SetName(Name);
        Widget->m_Scene = m_Scene;
        Widget->m_Owner = this;

        if (!Widget->Init())
        {
            SAFE_DELETE(Widget);
            return nullptr;
        }

        m_vecWidget.push_back(Widget);

        return (T*)Widget;
    }

private:
    static bool SortCollisionWidget(const CSharedPtr<class CWidget>& Src,
        const CSharedPtr<class CWidget>& Dest);
    static bool SortRenderWidget(const CSharedPtr<class CWidget>& Src,
        const CSharedPtr<class CWidget>& Dest);
};

