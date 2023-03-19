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
    CSharedPtr<class CProgressBar>  m_HPBar;            // 체력바 막대
    CSharedPtr<class CProgressBar>  m_HPBar2;
    CSharedPtr<class CImage>        m_HPBarFrame;       // 체력바 틀
    CSharedPtr<class CImage>        m_HPBarFrame2;

    CSharedPtr<class CNumber>       m_Timer;            // 타이머 숫자
    CSharedPtr<class CImage>        m_TimerBase;        // 타이머 배경

    CSharedPtr<class CImage>        m_IntroImage;       // 인트로 이미지
    CSharedPtr<class CImage>        m_RebelImage;       // 라운드 이미지
    CSharedPtr<class CNumber>       m_RebelNumber;      // 라운드 숫자
    CSharedPtr<class CImage>        m_ActionImage;      // 라운드 시작 이미지
    CSharedPtr<class CImage>        m_FinishImage;      // 라운드 종료 이미지

    CSharedPtr<class CText>         m_FPSText;

    float   m_Time;         // 타이머에 사용할 숫자(=라운드 제한 시간)
    float   m_WaitTime;     // StartSequence를 재개하기 위한 대기 시간
    bool    m_Start;        // StartSequence가 진행 중인지
    bool    m_Finish;       // 라운드가 끝났는지

public:
    void SetHP(float HP, bool isPlayer);            // 플레이어 또는 상대 캐릭터의 체력을 설정하는 함수
    void SetHPBarColor(int Step, bool isPlayer);    // 체력바 막대의 색깔을 변경하는 함수

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

