#pragma once
#include "../Ref.h"

/*오브젝트에 위젯을 붙여넣을 수 있도록 해주는 클래스*/
class CWidgetComponent :
    public CRef
{
    friend class CGameObject;

protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& widget);
    virtual ~CWidgetComponent();

private:
    class CGameObject*          m_Owner;    // 이 컴포넌트를 갖고 있는 오브젝트
    class CScene*               m_Scene;    // 이 컴포넌트가 속해 있는 씬 
    CSharedPtr<class CWidget>   m_Widget;   // 오브젝트에 출력할 위젯
    Vector2                     m_Pos;

public:
    Vector2 GetPos() const;
    float GetBottom() const;

    void SetPos(float x, float y)
    {
        m_Pos = Vector2(x, y);
    }

    void SetPos(const Vector2& Pos)
    {
        m_Pos = Pos;
    }

    void SetWidget(class CWidget* Widget);

    class CWidget* GetWidget(class CWidget* Widget)
    {
        return m_Widget;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);

    template <typename T>
    T* GetWidget() const
    {
        return (T*)m_Widget.Get();
    }

    template <typename T>
    T* CreateWidget(const std::string& Name)
    {
        T* Widget = new T;

        Widget->SetName(Name);
        Widget->m_Scene = m_Scene;

        if (!Widget->Init())
        {
            SAFE_DELETE(Widget);
            return nullptr;
        }

        m_Widget = Widget;

        return (T*)Widget;
    }
};

