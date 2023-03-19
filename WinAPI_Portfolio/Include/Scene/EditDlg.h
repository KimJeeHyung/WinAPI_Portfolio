#pragma once

#include "../GameInfo.h"

// 에디터에서 변경할 타일 데이터
enum class ETile_Brush
{
	Frame,	// 타일 텍스처의 프레임
	Option	// 타일의 옵션(지나갈 수 있는지 없는지)
};

/*타일맵 편집을 위한 에디터 다이얼로그*/
class CEditDlg
{
	friend class CEditScene;

public:
	CEditDlg();
	~CEditDlg();

private:
	class CEditScene*	m_Scene;					// 이 다이얼로그를 갖고 있는 에디터 씬
	HWND				m_hDlg;						// 다이얼로그 윈도우의 핸들러
	HWND				m_hTextureListBox;			// 텍스처 이름을 보여주는 리스트박스의 핸들러
	HWND				m_hTextureFrameListBox;		// 텍스처 프레임 정보를 보여주는 리스트박스의 핸들러
	HWND				m_hBrushTypeCombo;			// 변경할 타일 데이터를 선택하는 콤보박스의 핸들러
	HWND				m_hOptionCombo;				// 타일의 옵션을 선택하는 콤보박스의 핸들러
	HWND				m_hRenderCheckBox;			// 타일을 출력할지 선택하는 체크박스의 핸들러
	HWND				m_hSideCollisionCheckBox;	// 지나갈 수 있는 타일인지 선택하는 체크박스의 핸들러
	int					m_SelectTextureIndex;		// 텍스처 리스트박스에서 현재 선택된 셀의 인덱스
	int					m_SelectTextureFrameIndex;	// 프레임 리스트박스에서 현재 선택된 셀의 인덱스
	TCHAR				m_SelectTextureName[128];	// 리스트박스에서 얻어온 텍스처 이름을 저장하는 문자열
	CSharedPtr<class CTexture> m_SelectTexture;
	std::vector<std::vector<AnimationFrameData>> m_vecTextureFrame;

	ETile_Brush		m_BrushType;
	ETile_Option	m_SelectOption;

public:
	bool GetCheckRender() const
	{
		// 타일 출력 체크박스가 체크되어 있는지 확인하고 그 결과를 리턴한다.
		return SendMessage(m_hRenderCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	bool GetCheckSideCollision() const
	{
		// 타일 충돌 체크박스가 체크되어 있는지 확인하고 그 결과를 리턴한다.
		return SendMessage(m_hSideCollisionCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	ETile_Brush GetBrushType() const
	{
		return m_BrushType;
	}

	ETile_Option GetSelectOption() const
	{
		return m_SelectOption;
	}

	const AnimationFrameData* GetTileFrameData() const
	{
		// 텍스처 또는 프레임이 선택되지 않았으면 nullptr리턴
		if (m_SelectTextureIndex == -1 || m_SelectTextureFrameIndex == -1)
			return nullptr;

		return &m_vecTextureFrame[m_SelectTextureIndex][m_SelectTextureFrameIndex];
	}

public:
	bool Init();
	void CreateTileMap();
	void LoadTexture();
	void SelectTexture();
	void AddFrame();
	void SelectFrame();
	void SelectBrushType();
	void SelectOption();
	void Save();
	void Load();
	void LoadTextureBack();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

