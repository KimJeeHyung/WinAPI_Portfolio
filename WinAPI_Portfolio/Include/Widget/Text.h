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
    TCHAR*      m_Text;             // 출력할 문자열
    int         m_Count;            // 현재 문자열의 글자수
    int         m_Capacity;         // 문자열의 최대 용량
    COLORREF    m_TextColor;        // 글자 색깔
    COLORREF    m_ShadowColor;      // 그림자 색깔
    bool        m_Shadow;           // 그림자 여부
    Vector2     m_ShadowOffset;     // 그림자와 텍스트 사이의 간격
    CSharedPtr<class CFont> m_Font; // 폰트

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

        // 인자로 들어온 문자열이 현재 문자열보다 크면 현재 문자열 크기를 그만큼 늘려준다.
        // 1을 더해주는 이유는 문자열 끝에 있는 널 문자까지 포함해야 하기 때문이다.
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

    // 한 글자씩 추가(텍스트 타이핑 효과)
    void AddText(const TCHAR Text)
    {
        // 먼저 기존 문자열의 글자 수에 1을 더한다.(한 글자이므로)
        int Length = m_Count + 1;

        // 더한 값이 현재 문자열 최대 용량보다 클 경우
        if (Length + 1 > m_Capacity)
        {
            // 문자열 최대 용량을 위에서 구한 값으로 늘린다.
            m_Capacity = Length + 1;

            // 새 문자열을 늘린 문자열 크기로 생성한다.
            TCHAR* NewText = new TCHAR[m_Capacity];
            memset(NewText, 0, sizeof(TCHAR) * m_Capacity);

            // 새 문자열에 기존 문자열을 복사한다.
            lstrcpy(NewText, m_Text);

            // 기존 문자열의 메모리를 제거하고 새 문자열을 지정해준다.
            SAFE_DELETE_ARRAY(m_Text);

            m_Text = NewText;
        }

        // 문자열의 마지막 인덱스에 추가할 글자를 넣어준다. 
        m_Text[m_Count] = Text;

        // 현재 글자 수를 1 더한다.
        ++m_Count;
    }

    // 문자열 추가
    void AddText(const TCHAR* Text)
    {
        // 먼저 기존 문자열의 글자 수에 추가할 문자열의 크기를 더한다.
        int Length = m_Count + lstrlen(Text);

        // 더한 값이 현재 문자열 최대 용량보다 클 경우
        if (Length + 1 > m_Capacity)
        {
            // 문자열 최대 용량을 위에서 구한 값으로 늘린다.
            m_Capacity = Length + 1;

            // 새 문자열을 늘린 문자열 크기로 생성한다.
            TCHAR* NewText = new TCHAR[m_Capacity];
            memset(NewText, 0, sizeof(TCHAR) * m_Capacity);

            // 새 문자열에 기존 문자열을 복사한다.
            lstrcpy(NewText, m_Text);
                
            // 기존 문자열의 메모리를 제거하고 새 문자열을 지정해준다.
            SAFE_DELETE_ARRAY(m_Text);

            m_Text = NewText;
        }

        // 용량이 늘어난 문자열 뒤에 인자로 추가할 문자열을 붙여넣는다.
        lstrcat(m_Text, Text);

        // 현재 글자 수에 추가한 문자열의 글자 수를 더한다.
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

