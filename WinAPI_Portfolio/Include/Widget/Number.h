#pragma once
#include "Widget.h"
class CNumber :
    public CWidget
{
    friend class CWidgetWindow;
    friend class CWidgetComponent;

protected:
    CNumber();
    CNumber(const CNumber& widget);
    virtual ~CNumber();

protected:
    CSharedPtr<class CTexture>  m_Texture;      // �̹��� �ؽ�ó
    int                 m_Number;       
    std::vector<int>    m_vecNumber;    // m_Number�� �� ���ھ� ������ �ٷ�� ���� �迭
    bool                m_Zero;      // ���ڸ��� ���� �տ� 0�� ������ ����

public:
    void SetZero(bool Zero)
    {
        m_Zero = Zero;
    }

    void SetNumber(int Number)
    {
        m_Number = Number;
    }

    void AddNumber(int Number)
    {
        m_Number += Number;
    }

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

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);
};

