#pragma once
#include "Widget.h"
class CButton :
    public CWidget
{
    friend class CWidgetWindow;
    friend class CWidgetComponent;

protected:
    CButton();
    CButton(const CButton& widget);
    virtual ~CButton();

protected:
    CSharedPtr<class CTexture>  m_Texture;          // 버튼 텍스처
    AnimationFrameData          m_StateData[(int)EButton_State::Max];
    EButton_State               m_ButtonState;
    std::function<void()>       m_Callback[(int)EButton_Sound_State::Max];    // 버튼 상태별로 호출될 함수
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

    void SetColorKey(unsigned char r, unsigned char g, unsigned char b);

    void SetButtonStateData(EButton_State State, const Vector2& Start,
        const Vector2& End);    // 버튼 상태별 애니메이션을 설정하는 함수
    void EnableButton(bool Enable)
    {
        m_ButtonState = Enable ? EButton_State::Normal : EButton_State::Disable;
    }

    void SetSound(EButton_Sound_State State, const std::string& Name);  // 버튼 상태별 사운드를 설정하는 함수

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);

public:
    virtual void CollisionMouseHoveredCallback(const Vector2& Pos);
    virtual void CollisionMouseReleaseCallback();

public:
    template <typename T>
    void SetCallback(EButton_Sound_State State, T* Obj, void(T::* Func)())
    {
        m_Callback[(int)State] = std::bind(Func, Obj);
    }
};

