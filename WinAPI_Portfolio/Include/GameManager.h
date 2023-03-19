#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

/*
게임의 흐름을 구성하는 단계
1. 사용자의 입력
2. 입력받은 내용에 대해서 데이터 업데이트
   인공지능들의 데이터 업데이트
3. 업데이트된 데이터를 토대로 충돌을 수행
4. 출력이 되어야 하는 물체들을 판단
5. 화면에 출력이 되어야 하는 물체들을 출력
*/

// 윈도우 창을 생성하고 전체 게임을 관리하는 클래스이다.
class CGameManager
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hDC;
	HDC			m_hBackDC;	// 더블버퍼링을 위한 백버퍼
	HBITMAP		m_hBackBmp;
	HBITMAP		m_hBackPrevBmp;

	class CTimer*	m_Timer;
	float			m_FrameLimitTime;
	float			m_DeltaTime;

	Resolution m_RS;

	float m_TimeScale;	// 글로벌 타임 재생 비율(0이면 일시정지)

	HBRUSH	m_Brush[(int)EBrush_Type::Max];	// 충돌체 외곽선에 지정할 색깔
	HPEN	m_Pen[(int)EBrush_Type::Max];	// 선에 색을 입힐 땐 HPEN을 쓴다.

	bool m_EditMode;	// 현재 에디터 모드가 활성화됐는지 여부

	//class CPlayer*	m_Player;

	//RECT		m_TestRc;
	//int			m_Dir;

	// 이 변수가 static 멤버변수로 설정된 이유는 WndProc static 멤버함수에서
	// 이 변수를 윈도우가 종료될 때 false로 만들어주기 위해서
	// static 멤버변수로 만들어주었다.
	static bool m_Loop;

public:
	bool GetEditMode() const
	{
		return m_EditMode;
	}

	HBRUSH GetBrush(EBrush_Type Type) const
	{
		return m_Brush[(int)Type];
	}

	HPEN GetPen(EBrush_Type Type) const
	{
		return m_Pen[(int)Type];
	}

	Resolution GetResolution() const
	{
		return m_RS;
	}

	float GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	HDC GetWindowDC() const
	{
		return m_hDC;
	}

	HWND GetWindowHandle() const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance() const
	{
		return m_hInst;
	}

	float GetTimeScale() const
	{
		return m_TimeScale;
	}

public:
	void SetEditMode(bool Mode)
	{
		m_EditMode = Mode;
	}

	void SetTimeScale(float Scale)
	{
		m_TimeScale = Scale;
	}

	float GetFPS() const;

	void Exit();	// 게임 종료 함수

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(float DeltaTime);

private:
	void Register();
	bool Create();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CGameManager)
};

