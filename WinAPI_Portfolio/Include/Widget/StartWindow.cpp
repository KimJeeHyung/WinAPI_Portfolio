#include "StartWindow.h"
#include "Button.h"
#include "Image.h"
#include "CheckBox.h"
#include "Icon.h"
#include "Text.h"
#include "Number.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/EditScene.h"
#include "../Scene/SceneManager.h"
#include "../Input.h"
#include "../GameManager.h"

CStartWindow::CStartWindow()
{
}

CStartWindow::~CStartWindow()
{
}

bool CStartWindow::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	m_Scene->GetSceneResource()->LoadSound("UI", "ButtonHovered", false, "se00_153.wav");
	m_Scene->GetSceneResource()->LoadSound("UI", "ButtonClick", false, "se00_152.wav");

	SetSize(1280.f, 720.f);

	// 배경 이미지
	CImage* Background = CreateWidget<CImage>("Background");

	Background->SetTexture("StartBack", TEXT("Title.bmp"));
	Background->SetSize(1280.f, 720.f);

	// 스타트 버튼
	CButton* StartButton = CreateWidget<CButton>("StartButton");

	StartButton->SetTexture("StartButton", TEXT("StartButtonN.bmp"));
	StartButton->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(191.f, 160.f));
	StartButton->SetButtonStateData(EButton_State::MouseHovered, Vector2(191.f, 0.f), Vector2(382.f, 160.f));
	StartButton->SetButtonStateData(EButton_State::Click, Vector2(191.f, 0.f), Vector2(382.f, 160.f));
	StartButton->SetButtonStateData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(191.f, 160.f));
	StartButton->SetColorKey(255, 0, 255);

	StartButton->SetSound(EButton_Sound_State::MouseHovered, "ButtonHovered");
	StartButton->SetSound(EButton_Sound_State::Click, "ButtonClick");

	StartButton->SetPos(450.f, 478.f);
	StartButton->SetZOrder(1);

	StartButton->SetCallback<CStartWindow>(EButton_Sound_State::Click, this,
		&CStartWindow::StartButtonCallback);

	// 종료 버튼
	CButton* EndButton = CreateWidget<CButton>("EndButton");

	EndButton->SetTexture("EndButton", TEXT("EndButtonN.bmp"));
	EndButton->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(191.f, 160.f));
	EndButton->SetButtonStateData(EButton_State::MouseHovered, Vector2(191.f, 0.f), Vector2(382.f, 160.f));
	EndButton->SetButtonStateData(EButton_State::Click, Vector2(191.f, 0.f), Vector2(382.f, 160.f));
	EndButton->SetButtonStateData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(191.f, 160.f));
	EndButton->SetColorKey(255, 0, 255);

	EndButton->SetSound(EButton_Sound_State::MouseHovered, "ButtonHovered");
	EndButton->SetSound(EButton_Sound_State::Click, "ButtonClick");

	EndButton->SetPos(545.f, 558.f);
	EndButton->SetZOrder(1);

	EndButton->SetCallback<CStartWindow>(EButton_Sound_State::Click, this,
		&CStartWindow::EndButtonCallback);

	// 에디터 버튼
	CButton* EditButton = CreateWidget<CButton>("EditButton");

	EditButton->SetTexture("EditButton", TEXT("EditButtonN.bmp"));
	EditButton->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(191.f, 160.f));
	EditButton->SetButtonStateData(EButton_State::MouseHovered, Vector2(191.f, 0.f), Vector2(382.f, 160.f));
	EditButton->SetButtonStateData(EButton_State::Click, Vector2(191.f, 0.f), Vector2(382.f, 160.f));
	EditButton->SetButtonStateData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(191.f, 160.f));
	EditButton->SetColorKey(255, 0, 255);

	EditButton->SetSound(EButton_Sound_State::MouseHovered, "ButtonHovered");
	EditButton->SetSound(EButton_Sound_State::Click, "ButtonClick");

	EditButton->SetPos(645.f, 478.f);
	EditButton->SetZOrder(1);

	EditButton->SetCallback<CStartWindow>(EButton_Sound_State::Click, this,
		&CStartWindow::EditButtonCallback);

	// 이미지
	/*CImage* Image1 = CreateWidget<CImage>("Image1");

	Image1->SetTexture("Image1", TEXT("molu.bmp"));
	Image1->SetPos(50.f, 50.f);
	Image1->SetSize(600.f, 600.f);
	Image1->SetZOrder(1);*/

	// 체크박스
	//CCheckBox* CheckBox = CreateWidget<CCheckBox>("CheckBox");

	//CheckBox->SetTexture("CheckBox", TEXT("CheckBox.bmp"));
	//CheckBox->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 311.f), Vector2(310.f, 628.f));
	//CheckBox->SetButtonStateData(EButton_State::MouseHovered, Vector2(0.f, 311.f), Vector2(310.f, 628.f));
	//CheckBox->SetButtonStateData(EButton_State::Click, Vector2(0.f, 311.f), Vector2(310.f, 628.f));
	//CheckBox->SetButtonStateData(EButton_State::Checked, Vector2(0.f, 0.f), Vector2(310.f, 314.f));

	//CheckBox->SetPos(800.f, 50.f);
	//m_Scene->GetSceneResource()->SetColorKey("CheckBox", 255, 0, 255);
	//CheckBox->SetZOrder(1);

	//// 아이콘
	//CIcon* Icon = CreateWidget<CIcon>("Icon");

	//Icon->SetTexture("Icon", TEXT("UIDefault.bmp"));
	//Icon->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(100.f, 100.f));
	//Icon->SetButtonStateData(EButton_State::MouseHovered, Vector2(0.f, 0.f), Vector2(100.f, 100.f));
	//Icon->SetButtonStateData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(100.f, 100.f));
	//Icon->SetPos(600.f, 25.f);
	//Icon->SetZOrder(2);

	//// 텍스트
	//m_Text = CreateWidget<CText>("Text");

	//m_Text->SetText(TEXT("텍스트 출력!!"));
	//m_Text->SetPos(100.f, 10.f);
	//m_Text->SetTextColor(255, 0, 0);
	//m_Text->SetZOrder(2);

	//m_Text->SetEnableShadow(true);
	//m_Text->SetShadowOffset(5.f, 5.f);

	//m_TextTime = 0.f;

	//memset(m_AddText, 0, sizeof(TCHAR) * 32);
	//lstrcpy(m_AddText, TEXT("흑염룡"));
	//m_AddCount = 0;

	//// 숫자(시계 출력)
	//m_Hour = CreateWidget<CNumber>("Hour");
	//CImage* Colon = CreateWidget<CImage>("Colon");
	//m_Minute = CreateWidget<CNumber>("Minute");
	//CImage* Colon2 = CreateWidget<CImage>("Colon2");
	//m_Second = CreateWidget<CNumber>("Second");

	//std::vector<std::wstring> vecFileName;

	//for (int i = 0; i <= 9; i++)
	//{
	//	TCHAR FileName[MAX_PATH] = {};
	//	// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
	//	wsprintf(FileName, TEXT("Number/%d.bmp"), i);
	//	vecFileName.push_back(FileName);
	//}

	//m_Hour->SetTexture("Number", vecFileName);
	//m_Minute->SetTexture("Number", vecFileName);
	//m_Second->SetTexture("Number", vecFileName);
	//Colon->SetTexture("Colon", TEXT("Number/Colon.bmp"));
	//Colon2->SetTexture("Colon", TEXT("Number/Colon.bmp"));

	//m_Hour->SetSize(29.f, 48.f);
	//m_Hour->SetPos(1000.f, 30.f);

	//Colon->SetSize(29.f, 48.f);
	//Colon->SetPos(1058.f, 30.f);

	//m_Minute->SetSize(29.f, 48.f);
	//m_Minute->SetPos(1087.f, 30.f);

	//Colon2->SetSize(29.f, 48.f);
	//Colon2->SetPos(1145.f, 30.f);

	//m_Second->SetSize(29.f, 48.f);
	//m_Second->SetPos(1174.f, 30.f);

	//m_Hour->SetColorKey(255, 255, 255);
	//Colon->SetColorKey(255, 255, 255);
	//m_Minute->SetColorKey(255, 255, 255);
	//m_Second->SetColorKey(255, 255, 255);

	//// 프레임 텍스트
	//m_FPSText = CreateWidget<CText>("FPSText");

	//m_FPSText->SetText(TEXT("FPS"));
	//m_FPSText->SetPos(1100.f, 5.f);
	//m_FPSText->SetTextColor(255, 0, 0);
	//m_FPSText->SetEnableShadow(true);
	//m_FPSText->SetShadowOffset(2.f, 2.f);

	return true;
}

void CStartWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	/*SYSTEMTIME Time;

	GetLocalTime(&Time);

	m_Hour->SetNumber(Time.wHour);
	m_Minute->SetNumber(Time.wMinute);
	m_Second->SetNumber(Time.wSecond);

	if (m_AddCount < lstrlen(m_AddText))
	{
		m_TextTime += DeltaTime;

		if (m_TextTime >= 1.f)
		{
			m_TextTime -= 1.f;

			m_Text->AddText(m_AddText[m_AddCount]);

			++m_AddCount;
		}
	}

	float FPS = CGameManager::GetInst()->GetFPS();

	char Text[256] = {};
	sprintf_s(Text, "FPS : %.5f", FPS);

	TCHAR Unicode[256] = {};
	int Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, Text, -1, Unicode, Length);

	m_FPSText->SetText(Unicode);*/
}

void CStartWindow::StartButtonCallback()
{
	// 메인 씬에 들어가기 전 키에 바인딩된 함수들을 모두 제거해준다.
	// (플레이어에 키 바인딩이 중복으로 되는 것을 방지)
	CInput::GetInst()->ClearCallback();
	CSceneManager::GetInst()->CreateScene<CMainScene>();
}

void CStartWindow::EndButtonCallback()
{
	CGameManager::GetInst()->Exit();
}

void CStartWindow::EditButtonCallback()
{
	CInput::GetInst()->ClearCallback();
	CSceneManager::GetInst()->CreateScene<CEditScene>();
}
