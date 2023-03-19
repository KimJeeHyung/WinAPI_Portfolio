#pragma once
#include "Widget.h"

// 진행바 텍스처 종류
enum class EProgressBar_Texture_Type
{
    Back,   // 배경
    Bar,    // 막대
    End
};

// 체력바가 채워지는 방향(깎이는 방향이 아님에 주의!)
enum class EProgressBar_Dir
{
    LeftToRight,
    RightToLeft,
    BottomToTop,
    TopToBottom,
    End
};

class CProgressBar :
    public CWidget
{
    friend class CWidgetWindow;
    friend class CWidgetComponent;

protected:
    CProgressBar();
    CProgressBar(const CProgressBar& widget);
    virtual ~CProgressBar();

protected:
    CSharedPtr<class CTexture>  m_Texture[(int)EProgressBar_Texture_Type::End]; // 진행바 텍스처
    EProgressBar_Dir            m_Dir;      // 진행바가 차는 방향
    float                       m_Value;    // 0 ~ 1 사이의 값(진행바 비율)
    Vector2                     m_BarSize;  // 진행바의 크기(막대 크기 조절은 m_Size가 아닌 이걸로 함)
    Vector2                     m_BarPos;   // 진행바의 위치(막대가 채워지는 방향에 따라 막대의 위치가 바뀔 수 있음)

public:
    void SetBarDir(EProgressBar_Dir Dir)
    {
        m_Dir = Dir;
    }

    void SetValue(float Value)
    {
        m_Value = Value;

        if (m_Value > 1.f)
            m_Value = 1.f;
        else if (m_Value < 0.f)
            m_Value = 0.f;
    }

    void AddValue(float Value)
    {
        m_Value += Value;

        if (m_Value > 1.f)
            m_Value = 1.f;
        else if (m_Value < 0.f)
            m_Value = 0.f;
    }

public:
    float GetValue() const
    {
        return m_Value;
    }

public:
    void SetTexture(EProgressBar_Texture_Type Type, const std::string& Name,
        const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(EProgressBar_Texture_Type Type, const std::string& Name,
        const TCHAR* FullPath);

#ifdef UNICODE

    void SetTexture(EProgressBar_Texture_Type Type, const std::string& Name,
        const std::vector<std::wstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(EProgressBar_Texture_Type Type, const std::string& Name,
        const std::vector<std::wstring>& vecFullPath);

#else

    void SetTexture(EProgressBar_Texture_Type Type, const std::string& Name,
        const std::vector<std::string>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(EProgressBar_Texture_Type Type, const std::string& Name,
        const std::vector<std::string>& vecFullPath);

#endif // UNICODE

    void SetColorKey(EProgressBar_Texture_Type Type,
        unsigned char r, unsigned char g, unsigned char b);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);
};

