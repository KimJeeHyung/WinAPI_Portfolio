#pragma once
#include "Widget.h"
class CText :
    public CWidget
{
    friend class CWidgetWindow;
    friend class CWidgetComponent;

protected:
    CText();
    CText(const CText& widget);
    virtual ~CText();

protected:
    TCHAR*      m_Text;             // ����� ���ڿ�
    int         m_Count;            // ���� ���ڿ��� ���ڼ�
    int         m_Capacity;         // ���ڿ��� �ִ� �뷮
    COLORREF    m_TextColor;        // ���� ����
    COLORREF    m_ShadowColor;      // �׸��� ����
    bool        m_Shadow;           // �׸��� ����
    Vector2     m_ShadowOffset;     // �׸��ڿ� �ؽ�Ʈ ������ ����
    CSharedPtr<class CFont> m_Font; // ��Ʈ

public:
    void SetTextColor(unsigned char r, unsigned char g, unsigned char b)
    {
        m_TextColor = RGB(r, g, b);
    }

    void SetTextShadowColor(unsigned char r, unsigned char g, unsigned char b)
    {
        m_ShadowColor = RGB(r, g, b);
    }

    void SetEnableShadow(bool Enable)
    {
        m_Shadow = Enable;
    }

    void SetShadowOffset(float x, float y)
    {
        m_ShadowOffset = Vector2(x, y);
    }

    void SetText(const TCHAR* Text)
    {
        int Length = lstrlen(Text);

        // ���ڷ� ���� ���ڿ��� ���� ���ڿ����� ũ�� ���� ���ڿ� ũ�⸦ �׸�ŭ �÷��ش�.
        // 1�� �����ִ� ������ ���ڿ� ���� �ִ� �� ���ڱ��� �����ؾ� �ϱ� �����̴�.
        if (Length + 1 > m_Capacity)
        {
            m_Capacity = Length + 1;

            SAFE_DELETE_ARRAY(m_Text);

            m_Text = new TCHAR[m_Capacity];
            memset(m_Text, 0, sizeof(TCHAR) * m_Capacity);
        }

        lstrcpy(m_Text, Text);

        m_Count = lstrlen(m_Text);
    }

    // �� ���ھ� �߰�(�ؽ�Ʈ Ÿ���� ȿ��)
    void AddText(const TCHAR Text)
    {
        // ���� ���� ���ڿ��� ���� ���� 1�� ���Ѵ�.(�� �����̹Ƿ�)
        int Length = m_Count + 1;

        // ���� ���� ���� ���ڿ� �ִ� �뷮���� Ŭ ���
        if (Length + 1 > m_Capacity)
        {
            // ���ڿ� �ִ� �뷮�� ������ ���� ������ �ø���.
            m_Capacity = Length + 1;

            // �� ���ڿ��� �ø� ���ڿ� ũ��� �����Ѵ�.
            TCHAR* NewText = new TCHAR[m_Capacity];
            memset(NewText, 0, sizeof(TCHAR) * m_Capacity);

            // �� ���ڿ��� ���� ���ڿ��� �����Ѵ�.
            lstrcpy(NewText, m_Text);

            // ���� ���ڿ��� �޸𸮸� �����ϰ� �� ���ڿ��� �������ش�.
            SAFE_DELETE_ARRAY(m_Text);

            m_Text = NewText;
        }

        // ���ڿ��� ������ �ε����� �߰��� ���ڸ� �־��ش�. 
        m_Text[m_Count] = Text;

        // ���� ���� ���� 1 ���Ѵ�.
        ++m_Count;
    }

    // ���ڿ� �߰�
    void AddText(const TCHAR* Text)
    {
        // ���� ���� ���ڿ��� ���� ���� �߰��� ���ڿ��� ũ�⸦ ���Ѵ�.
        int Length = m_Count + lstrlen(Text);

        // ���� ���� ���� ���ڿ� �ִ� �뷮���� Ŭ ���
        if (Length + 1 > m_Capacity)
        {
            // ���ڿ� �ִ� �뷮�� ������ ���� ������ �ø���.
            m_Capacity = Length + 1;

            // �� ���ڿ��� �ø� ���ڿ� ũ��� �����Ѵ�.
            TCHAR* NewText = new TCHAR[m_Capacity];
            memset(NewText, 0, sizeof(TCHAR) * m_Capacity);

            // �� ���ڿ��� ���� ���ڿ��� �����Ѵ�.
            lstrcpy(NewText, m_Text);
                
            // ���� ���ڿ��� �޸𸮸� �����ϰ� �� ���ڿ��� �������ش�.
            SAFE_DELETE_ARRAY(m_Text);

            m_Text = NewText;
        }

        // �뷮�� �þ ���ڿ� �ڿ� ���ڷ� �߰��� ���ڿ��� �ٿ��ִ´�.
        lstrcat(m_Text, Text);

        // ���� ���� ���� �߰��� ���ڿ��� ���� ���� ���Ѵ�.
        m_Count += lstrlen(Text);
    }

    void pop_back()
    {
        --m_Count;

        m_Text[m_Count] = 0;
    }

    void clear()
    {
        m_Count = 0;

        m_Text[m_Count] = 0;
    }

    void SetFont(const std::string& Name);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);
};

