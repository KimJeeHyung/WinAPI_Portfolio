#include "CharacterHUD.h"
#include "Button.h"
#include "Image.h"
#include "ProgressBar.h"
#include "CheckBox.h"
#include "Text.h"
#include "Number.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/StartScene.h"
#include "../Scene/EndScene.h"
#include "../Scene/SceneManager.h"
#include "../Input.h"
#include "../Resource/Sound/Sound.h"
#include "../Coroutine.h"

CCharacterHUD::CCharacterHUD() :
	m_Time(99.f),
	m_WaitTime(0.f),
	m_Start(true),
	m_Finish(false)
{
}

CCharacterHUD::~CCharacterHUD()
{
}

void CCharacterHUD::SetHP(float HP, bool isPlayer)
{
	if(isPlayer)
		m_HPBar->SetValue(HP);
	else
		m_HPBar2->SetValue(HP);
}

void CCharacterHUD::SetHPBarColor(int Step, bool isPlayer)
{
	if (isPlayer)
	{
		switch (Step)
		{
		case 1:
			m_HPBar->SetTexture(EProgressBar_Texture_Type::Bar, "HPBar", TEXT("LifeBarGreen2s.bmp"));
			m_HPBar->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
			break;
		case 2:
			m_HPBar->SetTexture(EProgressBar_Texture_Type::Bar, "HPBarY", TEXT("LifeBarYellow2s.bmp"));
			m_HPBar->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
			break;
		case 3:
			m_HPBar->SetTexture(EProgressBar_Texture_Type::Bar, "HPBarR", TEXT("LifeBarGreen2s.bmp"));
			m_HPBar->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
			break;
		}
	}
	else
	{
		switch (Step)
		{
		case 1:
			m_HPBar2->SetTexture(EProgressBar_Texture_Type::Bar, "HPBar2", TEXT("LifeBarGreen2sInv.bmp"));
			m_HPBar2->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
			break;
		case 2:
			m_HPBar2->SetTexture(EProgressBar_Texture_Type::Bar, "HPBar2Y", TEXT("LifeBarYellow2sInv.bmp"));
			m_HPBar2->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
			break;
		case 3:
			m_HPBar2->SetTexture(EProgressBar_Texture_Type::Bar, "HPBar2R", TEXT("LifeBarRed2sInv.bmp"));
			m_HPBar2->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
			break;
		}
	}
}

bool CCharacterHUD::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(210.f, 110.f);
	//SetPos(50.f, 50.f);

	// 체력바 틀
	m_HPBarFrame = CreateWidget<CImage>("HPBarFrame");

	m_HPBarFrame->SetTexture("RagnaHPBarFrame", TEXT("HUD_Ragna2s.bmp"));
	m_HPBarFrame->SetPos(0.f, -15.f);
	m_HPBarFrame->SetSize(624.f, 193.f);
	m_HPBarFrame->SetColorKey(255, 0, 255);
	m_HPBarFrame->SetZOrder(1);

	m_HPBarFrame2 = CreateWidget<CImage>("HPBarFrame2");

	m_HPBarFrame2->SetTexture("NoelHPBarFrame", TEXT("HUD_Noel2s.bmp"));
	m_HPBarFrame2->SetPos(655.f, -15.f);
	m_HPBarFrame2->SetSize(624.f, 193.f);
	m_HPBarFrame2->SetColorKey(255, 0, 255);
	m_HPBarFrame2->SetZOrder(1);

	// 체력바 막대(실제 체력에 따라 깎이는 부분)
	m_HPBar = CreateWidget<CProgressBar>("HPBar");

	m_HPBar->SetTexture(EProgressBar_Texture_Type::Bar, "HPBar", TEXT("LifeBarGreen2s.bmp"));
	m_HPBar->SetSize(408.f, 30.f);
	m_HPBar->SetPos(146.f, 56.f);
	m_HPBar->SetBarDir(EProgressBar_Dir::RightToLeft);
	m_HPBar->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
	m_HPBar->SetZOrder(2);

	m_HPBar2 = CreateWidget<CProgressBar>("HPBar2");

	m_HPBar2->SetTexture(EProgressBar_Texture_Type::Bar, "HPBar2", TEXT("LifeBarGreen2sInv.bmp"));
	m_HPBar2->SetSize(408.f, 30.f);
	m_HPBar2->SetPos(725.f, 56.f);
	m_HPBar2->SetBarDir(EProgressBar_Dir::LeftToRight);
	m_HPBar2->SetColorKey(EProgressBar_Texture_Type::Bar, 255, 0, 255);
	m_HPBar2->SetZOrder(2);

	// 프레임 텍스트
	/*m_FPSText = CreateWidget<CText>("FPSText");

	m_FPSText->SetText(TEXT("FPS"));
	m_FPSText->SetPos(1140.f, 5.f);
	m_FPSText->SetTextColor(255, 0, 0);
	m_FPSText->SetEnableShadow(true);
	m_FPSText->SetShadowOffset(2.f, 2.f);
	m_FPSText->SetZOrder(2);*/

	// 타이머 배경
	m_TimerBase = CreateWidget<CImage>("TimerBase");

	m_TimerBase->SetTexture("TimerBase", TEXT("Timer_BaseNs3.bmp"));
	m_TimerBase->SetSize(154.f, 154.f);
	m_TimerBase->SetPos(562.f, 0.f);
	m_TimerBase->SetColorKey(255, 0, 255);

	// 타이머 숫자
	m_Timer = CreateWidget<CNumber>("Timer");
	m_Timer->SetNumber((int)m_Time);
	m_Timer->SetZero(true);

	m_Timer->SetTexture("Timer", TEXT("Number/Timer_Font.bmp"));
	m_Timer->SetPos(593.f, 0.f);
	m_Timer->SetSize(46.f, 128.f);
	m_Timer->SetColorKey(255, 0, 255);
	m_Timer->SetZOrder(2);

	// 라운드 종료 이미지
	m_FinishImage = CreateWidget<CImage>("Finish");

	m_FinishImage->SetTexture("Finish", TEXT("Finishs2.bmp"));
	m_FinishImage->SetPos(135.f, 250.f);
	m_FinishImage->SetSize(1000.f, 180.f);
	m_FinishImage->SetColorKey(255, 0, 255);
	m_FinishImage->SetEnable(false);

	// 라운드 시작 이미지
	m_IntroImage = CreateWidget<CImage>("Start");

	m_IntroImage->SetTexture("Start", TEXT("WheelOfFateS.bmp"));
	m_IntroImage->SetSize(1200.f, 404.f);
	m_IntroImage->SetPos(50.f, 150.f);
	m_IntroImage->SetColorKey(255, 0, 255);
	m_IntroImage->SetEnable(false);

	// 라운드 수 이미지
	m_RebelImage = CreateWidget<CImage>("Rebel");

	m_RebelImage->SetTexture("Rebel", TEXT("Rebel.bmp"));
	m_RebelImage->SetSize(638.f, 163.f);
	m_RebelImage->SetPos(325.f, 150.f);
	m_RebelImage->SetColorKey(255, 0, 255);
	m_RebelImage->SetEnable(false);

	// 라운드 숫자
	m_RebelNumber = CreateWidget<CNumber>("RoundNumber");
	m_RebelNumber->SetNumber(1);
	m_RebelNumber->SetZero(false);

	m_RebelNumber->SetTexture("RoundNumber", TEXT("RoundFont.bmp"));
	m_RebelNumber->SetSize(135.f, 230.f);
	m_RebelNumber->SetPos(573.f, 320.f);
	m_RebelNumber->SetColorKey(255, 0, 255);
	m_RebelNumber->SetEnable(false);

	// 액션 이미지
	m_ActionImage = CreateWidget<CImage>("Action");

	m_ActionImage->SetTexture("Action", TEXT("ActionS.bmp"));
	m_ActionImage->SetSize(1000.f, 229.f);
	m_ActionImage->SetPos(135.f, 250.f);
	m_ActionImage->SetColorKey(255, 0, 255);
	m_ActionImage->SetEnable(false);

	return true;
}

void CCharacterHUD::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	CCoroutine start = StartSequence();

	if (m_Start)
	{
		m_WaitTime += DeltaTime;

		start.co_handler.resume();

		if (m_WaitTime >= 4.f)
		{
			start.co_handler.resume();

			if (m_WaitTime >= 5.f)
				start.co_handler.resume();
			if (m_WaitTime >= 6.f)
				start.co_handler.resume();
			if (m_WaitTime >= 7.5f)
				start.co_handler.resume();
		}
	}

	// 라운드가 진행 중일 때만 시간을 감소시킨다.
	if(!m_Start && !m_Finish)
		m_Time -= DeltaTime;

	// 제한 시간이 지나도 라운드 종료 이미지 출력
	if (m_Time <= 0.f)
	{
		m_Time = 0.f;
		FinishSequence();
	}

	m_Timer->SetNumber((int)m_Time);

	if (m_Finish)
	{
		m_WaitTime += DeltaTime;

		if (m_WaitTime >= 5.f)
		{
			CInput::GetInst()->ClearCallback();
			CSceneManager::GetInst()->CreateScene<CEndScene>();
		}
	}

	/*float FPS = CGameManager::GetInst()->GetFPS();

	char Text[256] = {};
	sprintf_s(Text, "FPS : %.5f", FPS);

	TCHAR Unicode[256] = {};
	int Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, Text, -1, Unicode, Length);

	m_FPSText->SetText(Unicode);*/
}

CCoroutine CCharacterHUD::StartSequence()
{
	m_IntroImage->SetEnable(true);

	if(!m_Scene->GetSceneResource()->FindSound("Intro")->GetPlay())
		m_Scene->GetSceneResource()->SoundPlay("Intro");

	co_await std::suspend_always{};

	m_IntroImage->SetEnable(false);
	m_RebelImage->SetEnable(true);
	m_RebelNumber->SetEnable(true);
	m_RebelNumber->SetNumber(1);

	if (!m_Scene->GetSceneResource()->FindSound("Rebel")->GetPlay())
		m_Scene->GetSceneResource()->SoundPlay("Rebel");

	co_await std::suspend_always{};

	if (!m_Scene->GetSceneResource()->FindSound("One")->GetPlay())
		m_Scene->GetSceneResource()->SoundPlay("One");

	co_await std::suspend_always{};

	m_RebelImage->SetEnable(false);
	m_RebelNumber->SetEnable(false);
	m_ActionImage->SetEnable(true);

	if (!m_Scene->GetSceneResource()->FindSound("Action")->GetPlay())
		m_Scene->GetSceneResource()->SoundPlay("Action");

	co_await std::suspend_always{};

	m_ActionImage->SetEnable(false);
	m_Start = false;
	m_WaitTime = 0.f;
}

void CCharacterHUD::FinishSequence()
{
	m_FinishImage->SetEnable(true);

	if(!m_Scene->GetSceneResource()->FindSound("Finish")->GetPlay())
		m_Scene->GetSceneResource()->SoundPlay("Finish");

	m_Finish = true;
}
