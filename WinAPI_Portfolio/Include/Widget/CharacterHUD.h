#pragma once
#include "WidgetWindow.h"
class CCharacterHUD :
    public CWidgetWindow
{
    friend class CScene;

protected:
    CCharacterHUD();
    virtual ~CCharacterHUD();

protected:
    CSharedPtr<class CProgressBar>  m_HPBar;            // ü�¹� ����
    CSharedPtr<class CProgressBar>  m_HPBar2;
    CSharedPtr<class CImage>        m_HPBarFrame;       // ü�¹� Ʋ
    CSharedPtr<class CImage>        m_HPBarFrame2;

    CSharedPtr<class CNumber>       m_Timer;            // Ÿ�̸� ����
    CSharedPtr<class CImage>        m_TimerBase;        // Ÿ�̸� ���

    CSharedPtr<class CImage>        m_IntroImage;       // ��Ʈ�� �̹���
    CSharedPtr<class CImage>        m_RebelImage;       // ���� �̹���
    CSharedPtr<class CNumber>       m_RebelNumber;      // ���� ����
    CSharedPtr<class CImage>        m_ActionImage;      // ���� ���� �̹���
    CSharedPtr<class CImage>        m_FinishImage;      // ���� ���� �̹���

    CSharedPtr<class CText>         m_FPSText;

    float   m_Time;         // Ÿ�̸ӿ� ����� ����(=���� ���� �ð�)
    float   m_WaitTime;     // StartSequence�� �簳�ϱ� ���� ��� �ð�
    bool    m_Start;        // StartSequence�� ���� ������
    bool    m_Finish;       // ���尡 ��������

public:
    void SetHP(float HP, bool isPlayer);            // �÷��̾� �Ǵ� ��� ĳ������ ü���� �����ϴ� �Լ�
    void SetHPBarColor(int Step, bool isPlayer);    // ü�¹� ������ ������ �����ϴ� �Լ�

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    class CCoroutine StartSequence();
    void FinishSequence();

public:
    bool GetStart() const
    {
        return m_Start;
    }

    bool GetFinish() const
    {
        return m_Finish;
    }
};

