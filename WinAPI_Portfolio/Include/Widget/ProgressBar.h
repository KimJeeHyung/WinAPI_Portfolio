#pragma once
#include "Widget.h"

// ����� �ؽ�ó ����
enum class EProgressBar_Texture_Type
{
    Back,   // ���
    Bar,    // ����
    End
};

// ü�¹ٰ� ä������ ����(���̴� ������ �ƴԿ� ����!)
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
    CSharedPtr<class CTexture>  m_Texture[(int)EProgressBar_Texture_Type::End]; // ����� �ؽ�ó
    EProgressBar_Dir            m_Dir;      // ����ٰ� ���� ����
    float                       m_Value;    // 0 ~ 1 ������ ��(����� ����)
    Vector2                     m_BarSize;  // ������� ũ��(���� ũ�� ������ m_Size�� �ƴ� �̰ɷ� ��)
    Vector2                     m_BarPos;   // ������� ��ġ(���밡 ä������ ���⿡ ���� ������ ��ġ�� �ٲ� �� ����)

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

