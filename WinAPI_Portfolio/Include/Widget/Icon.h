#pragma once
#include "Widget.h"
class CIcon :
    public CWidget
{
    friend class CWidgetWindow;
    friend class CWidgetComponent;

protected:
    CIcon();
    CIcon(const CIcon& widget);
    virtual ~CIcon();

protected:
    CSharedPtr<class CTexture>  m_Texture;          // ������ �ؽ�ó
    AnimationFrameData          m_StateData[(int)EButton_State::Max];
    EButton_State               m_ButtonState;
    std::function<void()>       m_Callback[(int)EButton_Sound_State::Max];    // ��ư ���º��� ȣ��� �Լ�
    CSharedPtr<class CSound>    m_StateSound[(int)EButton_Sound_State::Max];

public:
    void SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);

#ifdef UNICODE

    void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const std::vector<std::wstring>& vecFullPath);

#else

    void SetTexture(const std::string& Name, const std::vector<std::string>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const std::vector<std::string>& vecFullPath);

#endif // UNICODE

    void SetButtonStateData(EButton_State State, const Vector2& Start,
        const Vector2& End);    // ��ư ���º� �ִϸ��̼��� �����ϴ� �Լ�
    void EnableButton(bool Enable)
    {
        m_ButtonState = Enable ? EButton_State::Normal : EButton_State::Disable;
    }

    void SetSound(EButton_Sound_State State, const std::string& Name);  // ��ư ���º� ���带 �����ϴ� �Լ�

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);

public:
    virtual void CollisionMouseHoveredCallback(const Vector2& Pos);
    virtual void CollisionMouseReleaseCallback();
};

