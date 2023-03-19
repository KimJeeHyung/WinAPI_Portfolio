#include "EditDlg.h"
#include "../GameManager.h"
#include "../resource.h"
#include "EditScene.h"
#include "../PathManager.h"
#include "SceneResource.h"
#include "../Resource/Texture/Texture.h"
#include "../GameObject/TileMap.h"

// WndProc()가 static이므로 이 클래스에 있는 함수를 사용하기 위해 전역변수를 만들어준다.
// (전역변수 대신에 static변수를 만들어도 됨)
CEditDlg* g_Dlg = nullptr;

CEditDlg::CEditDlg() :
	m_SelectTextureName{}
{
	g_Dlg = this;
	m_SelectTextureIndex = -1;			// 현재 선택된 게 없다는 뜻으로 -1로 초기화
	m_SelectTextureFrameIndex = -1;		// 현재 선택된 게 없다는 뜻으로 -1로 초기화
	m_BrushType = ETile_Brush::Frame;
	m_SelectOption = ETile_Option::Normal;
}

CEditDlg::~CEditDlg()
{
}

bool CEditDlg::Init()
{
	CGameManager::GetInst()->SetEditMode(true);

	// Modal : 부모윈도우는 멈추고 열리는 다이얼로그만 동작
	// Modeless : 부모윈도우와 다이얼로그 모두 동작
	m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(),
		MAKEINTRESOURCE(IDD_DIALOG_EDIT), CGameManager::GetInst()->GetWindowHandle(),
		CEditDlg::WndProc);

	ShowWindow(m_hDlg, SW_SHOW);

	// 숫자 입력칸에 기본으로 들어갈 숫자를 정해준다.
	// 마지막 인자는 signed unsigned 여부
	SetDlgItemInt(m_hDlg, IDC_EDIT_COUNTX, 34, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_COUNTY, 24, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_SIZEX, 64, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_SIZEY, 50, TRUE);

	m_hTextureListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTURE);
	m_hTextureFrameListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTURE_FRAME);
	m_hBrushTypeCombo = GetDlgItem(m_hDlg, IDC_COMBO_BRUSH_TYPE);
	m_hOptionCombo = GetDlgItem(m_hDlg, IDC_COMBO_OPTION);
	m_hRenderCheckBox = GetDlgItem(m_hDlg, IDC_CHECK_RENDER);
	m_hSideCollisionCheckBox = GetDlgItem(m_hDlg, IDC_CHECK_SIDECOLLISION);

	// 브러시 콤보박스에 데이터를 추가한다.
	SendMessage(m_hBrushTypeCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Frame"));
	SendMessage(m_hBrushTypeCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Option"));

	// 타일 옵션 콤보박스에 데이터를 추가한다.
	SendMessage(m_hOptionCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Normal"));
	SendMessage(m_hOptionCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("ImpossibleToMove"));

	// 각각의 콤보박스의 초기 상태를 0번으로 설정한다.
	SendMessage(m_hBrushTypeCombo, CB_SETCURSEL, 0, 0);
	SendMessage(m_hOptionCombo, CB_SETCURSEL, 0, 0);

	// 출력 체크박스의 초기 상태를 체크로 설정한다.
	SendMessage(m_hRenderCheckBox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

	return true;
}

void CEditDlg::CreateTileMap()
{
	// 아래의 함수들이 동작됐는지 확인할 변수
	BOOL Transfer = FALSE;

	// 타일맵의 가로 세로 개수와 사이즈를 입력하는 칸에 입력한 숫자를 받아온다.
	int CountX = GetDlgItemInt(m_hDlg, IDC_EDIT_COUNTX, &Transfer, TRUE);
	int CountY = GetDlgItemInt(m_hDlg, IDC_EDIT_COUNTY, &Transfer, TRUE);
	int SizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_SIZEX, &Transfer, TRUE);
	int SizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_SIZEY, &Transfer, TRUE);

	// 받아온 값으로 타일맵을 생성한다.
	m_Scene->CreateTileMap(CountX, CountY, SizeX, SizeY);
}

void CEditDlg::LoadTexture()
{
	TCHAR FilePath[MAX_PATH] = {};

	// 이미지 파일을 불러오기 위한 파일 다이얼로그를 만든다.
	// (파일 다이얼로그 생성은 계속 이 방식대로 하면 됨)
	OPENFILENAME OpenFile = {};

	// 구조체 크기는 구조체 크기로 고정
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// 파일 다이얼로그의 핸들러는 에디터 다이얼로그
	OpenFile.hwndOwner = m_hDlg;
	// 선택할 파일로 모든 파일과 비트맵 파일만 보여주도록 필터를 추가한다.
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0BmpFile\0*.bmp");
	// FilePath에 전체경로가 들어온다.
	OpenFile.lpstrFile = FilePath;
	// 경로(배열)의 최대 개수
	OpenFile.nMaxFile = MAX_PATH;
	// 기본 경로로 텍스처 폴더의 경로를 지정한다.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0이 아니라면 구조체가 제대로 받아져왔다는 뜻이다.
	if (GetOpenFileName(&OpenFile) != 0)
	{
		TCHAR FileName[128] = {};

		// 파일 이름만 받아오면 되므로 드라이브, 디렉토리, 확장자는 nullptr,
		// 크기는 0으로 둔다.
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

		char TextureName[128] = {};

#ifdef UNICODE

		// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);

		// 파일 이름을 유니코드에서 멀티바이트로 바꿔준다.
		WideCharToMultiByte(CP_ACP, 0, FileName, -1,
			TextureName, PathLength, 0, 0);

#else

		strcpy_s(TextureName, FileName);

#endif // UNICODE

		// 리스트박스에 텍스처 이름을 추가한다.
		SendMessage(m_hTextureListBox, LB_ADDSTRING, 0, (LPARAM)FileName);

		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(TextureName, FilePath))
			return;

		m_Scene->GetSceneResource()->SetColorKey(TextureName, 255, 0, 255);

		// 이 텍스처에 대한 프레임 배열을 추가한다.
		std::vector<AnimationFrameData> vecData;
		m_vecTextureFrame.push_back(vecData);
	}
}

void CEditDlg::SelectTexture()
{
	// 텍스처 리스트박스에서 현재 선택된 셀(LB_GETCURSEL)의 인덱스를 얻어오도록 한다.
	m_SelectTextureIndex = (int)SendMessage(m_hTextureListBox, LB_GETCURSEL, 0, 0);

	// 인덱스가 제대로 들어왔을 경우
	if (m_SelectTextureIndex != -1)
	{
		memset(m_SelectTextureName, 0, sizeof(TCHAR) * 128);
		// LB_GETTEXT를 이용해 텍스처 이름을 얻어온다.
		SendMessage(m_hTextureListBox, LB_GETTEXT, m_SelectTextureIndex, (LPARAM)m_SelectTextureName);

		char TextureName[128] = {};

#ifdef UNICODE

		// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, m_SelectTextureName, -1, 0, 0, 0, 0);

		// 파일 이름을 유니코드에서 멀티바이트로 바꿔준다.
		WideCharToMultiByte(CP_ACP, 0, m_SelectTextureName, -1,
			TextureName, PathLength, 0, 0);

#else

		strcpy_s(TextureName, FileName);

#endif // UNICODE

		// 얻어온 텍스처 이름으로 텍스처를 찾아서 타일맵의 텍스처로 정해준다.
		m_SelectTexture = m_Scene->GetSceneResource()->FindTexture(TextureName);

		m_Scene->SetTileTexture(m_SelectTexture);

		m_SelectTextureFrameIndex = -1;
	}
}

void CEditDlg::AddFrame()
{
	// 선택된 텍스처가 없으면 무시한다.
	if (m_SelectTextureIndex == -1)
		return;

	// 아래의 함수들이 동작됐는지 확인할 변수
	BOOL Transfer = FALSE;

	// 텍스처 프레임의 시작과 끝 좌표를 입력하는 칸에 입력한 숫자를 받아온다.
	int StartX = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, &Transfer, TRUE);
	int StartY = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, &Transfer, TRUE);
	int EndX = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, &Transfer, TRUE);
	int EndY = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, &Transfer, TRUE);
	
	// 프레임 데이터에 입력한 값을 추가해준다.
	AnimationFrameData Data;
	Data.Start.x = (float)StartX;
	Data.Start.y = (float)StartY;
	Data.End.x = (float)EndX;
	Data.End.y = (float)EndY;

	TCHAR ListData[32] = {};

	// 선택한 텍스처에 추가된 프레임 데이터 갯수를 문자열로 변환
	wsprintf(ListData, TEXT("%d"), (int)m_vecTextureFrame[m_SelectTextureIndex].size());

	// 변환한 문자열을 리스트박스에 출력한다.
	SendMessage(m_hTextureFrameListBox, LB_ADDSTRING, 0, (LPARAM)ListData);

	m_vecTextureFrame[m_SelectTextureIndex].push_back(Data);
}

void CEditDlg::SelectFrame()
{
	// 선택된 텍스처가 없으면 무시한다.
	if (m_SelectTextureIndex == -1)
		return;

	// 프레임 리스트박스에서 현재 선택된 셀(LB_GETCURSEL)의 인덱스를 얻어오도록 한다.
	m_SelectTextureFrameIndex = (int)SendMessage(m_hTextureFrameListBox, LB_GETCURSEL, 0, 0);

	if (m_SelectTextureFrameIndex != -1)
	{
		// 선택한 텍스처의 프레임 데이터를 가져온다.
		AnimationFrameData Data = m_vecTextureFrame[m_SelectTextureIndex][m_SelectTextureFrameIndex];

		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, (int)Data.Start.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, (int)Data.Start.y, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, (int)Data.End.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, (int)Data.End.y, TRUE);
	}
}

void CEditDlg::SelectBrushType()
{
	// 브러시 콤보박스에서 현재 선택된 셀(CB_GETCURSEL)의 인덱스를 얻어오도록 한다.
	m_BrushType = (ETile_Brush)SendMessage(m_hBrushTypeCombo, CB_GETCURSEL, 0, 0);
}

void CEditDlg::SelectOption()
{
	// 타일 옵션 콤보박스에서 현재 선택된 셀(CB_GETCURSEL)의 인덱스를 얻어오도록 한다.
	m_SelectOption = (ETile_Option)SendMessage(m_hOptionCombo, CB_GETCURSEL, 0, 0);
}

void CEditDlg::Save()
{
	// 생성된 타일맵이 없으면 저장하지 않는다.
	if (!m_Scene->GetTileMap())
		return;

	TCHAR FilePath[MAX_PATH] = {};

	// 타일맵 정보를 파일로 저장하기 위한 파일 다이얼로그를 만든다.
	OPENFILENAME OpenFile = {};

	// 구조체 크기는 구조체 크기로 고정
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// 파일 다이얼로그의 핸들러는 에디터 다이얼로그
	OpenFile.hwndOwner = m_hDlg;
	// 선택할 파일로 모든 파일과 타일맵 파일만 보여주도록 필터를 추가한다.
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0TileMapFile\0*.tmp");
	// FilePath에 전체경로가 들어온다.
	OpenFile.lpstrFile = FilePath;
	// 경로(배열)의 최대 개수
	OpenFile.nMaxFile = MAX_PATH;
	// 기본 경로로 텍스처 폴더의 경로를 지정한다.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0이 아니라면 저장이 제대로 되었다는 뜻이다.
	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

#ifdef UNICODE

		// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);

		// 경로 이름을 유니코드에서 멀티바이트로 바꿔준다.
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
			FullPath, PathLength, 0, 0);

#else

		strcpy_s(FullPath, FilePath);

#endif // UNICODE

		m_Scene->GetTileMap()->SaveFullPath(FullPath);
	}
}

void CEditDlg::Load()
{
	TCHAR FilePath[MAX_PATH] = {};

	// 타일맵 정보를 파일로 저장하기 위한 파일 다이얼로그를 만든다.
	OPENFILENAME OpenFile = {};

	// 구조체 크기는 구조체 크기로 고정
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// 파일 다이얼로그의 핸들러는 에디터 다이얼로그
	OpenFile.hwndOwner = m_hDlg;
	// 선택할 파일로 모든 파일과 타일맵 파일만 보여주도록 필터를 추가한다.
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0TileMapFile\0*.tmp");
	// FilePath에 전체경로가 들어온다.
	OpenFile.lpstrFile = FilePath;
	// 경로(배열)의 최대 개수
	OpenFile.nMaxFile = MAX_PATH;
	// 기본 경로로 텍스처 폴더의 경로를 지정한다.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0이 아니라면 저장이 제대로 되었다는 뜻이다.
	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

#ifdef UNICODE

		// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);

		// 경로 이름을 유니코드에서 멀티바이트로 바꿔준다.
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
			FullPath, PathLength, 0, 0);

#else

		strcpy_s(FullPath, FilePath);

#endif // UNICODE

		m_Scene->LoadTileMap(FullPath);
	}
}

void CEditDlg::LoadTextureBack()
{
	if (!m_Scene->GetTileMap())
		return;

	TCHAR FilePath[MAX_PATH] = {};

	// 이미지 파일을 불러오기 위한 파일 다이얼로그를 만든다.
	// (파일 다이얼로그 생성은 계속 이 방식대로 하면 됨)
	OPENFILENAME OpenFile = {};

	// 구조체 크기는 구조체 크기로 고정
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// 파일 다이얼로그의 핸들러는 에디터 다이얼로그
	OpenFile.hwndOwner = m_hDlg;
	// 선택할 파일로 모든 파일과 비트맵 파일만 보여주도록 필터를 추가한다.
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0BmpFile\0*.bmp");
	// FilePath에 전체경로가 들어온다.
	OpenFile.lpstrFile = FilePath;
	// 경로(배열)의 최대 개수
	OpenFile.nMaxFile = MAX_PATH;
	// 기본 경로로 텍스처 폴더의 경로를 지정한다.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0이 아니라면 구조체가 제대로 받아져왔다는 뜻이다.
	if (GetOpenFileName(&OpenFile) != 0)
	{
		TCHAR FileName[128] = {};

		// 파일 이름만 받아오면 되므로 드라이브, 디렉토리, 확장자는 nullptr,
		// 크기는 0으로 둔다.
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

		char TextureName[128] = {};

#ifdef UNICODE

		// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);

		// 파일 이름을 유니코드에서 멀티바이트로 바꿔준다.
		WideCharToMultiByte(CP_ACP, 0, FileName, -1,
			TextureName, PathLength, 0, 0);

#else

		strcpy_s(TextureName, FileName);

#endif // UNICODE

		m_Scene->GetTileMap()->SetTextureFullPath(TextureName, FilePath);
		m_Scene->GetTileMap()->SetColorKey(255, 0, 255);
	}
}

LRESULT CEditDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		// LOWORD : 4바이트인 WPARAM의 하위 16비트를 받아온다.
		switch (LOWORD(wParam))
		{
		// 생성 버튼을 누르면 타일맵을 생성한다.
		case IDC_BUTTON_CREATE_TILEMAP:
			g_Dlg->CreateTileMap();
			break;
		// 이미지 불러오기 버튼을 누르면 텍스처를 불러온다.
		case IDC_BUTTON_LOADTEXTURE:
			g_Dlg->LoadTexture();
			break;
		// 프레임 추가 버튼을 누르면 프레임 데이터를 저장한다.
		case IDC_BUTTON_ADD_FRAME:
			g_Dlg->AddFrame();
			break;
		// 배경 불러오기 버튼
		case IDC_BUTTON_LOADTEXTURE_BACK:
			g_Dlg->LoadTextureBack();
			break;
		// 텍스처 리스트박스를 클릭할 경우 선택한 텍스처로 타일맵 텍스처가 출력된다.
		case IDC_LIST_TEXTURE:
			// HIWORD : 4바이트인 WPARAM의 상위 16비트를 받아온다.
			switch (HIWORD(wParam))
			{
			// 리스트박스에서 선택한 것이 바뀌었을 경우
			case LBN_SELCHANGE:
				g_Dlg->SelectTexture();
				break;
			}
			break;
		// 텍스처 프레임 리스트박스를 클릭할 경우 선택한 인덱스의 프레임 정보가 출력된다.
		case IDC_LIST_TEXTURE_FRAME:
			switch (HIWORD(wParam))
			{
			// 리스트박스에서 선택한 것이 바뀌었을 경우
			case LBN_SELCHANGE:
				g_Dlg->SelectFrame();
				break;
			}
			break;
		// 브러시 콤보박스 선택
		case IDC_COMBO_BRUSH_TYPE:
			switch (HIWORD(wParam))
			{
			// 콤보박스에서 선택한 것이 바뀌었을 경우
			case CBN_SELCHANGE:
				g_Dlg->SelectBrushType();
				break;
			}
			break;
		// 타일 옵션 콤보박스 선택
		case IDC_COMBO_OPTION:
			switch (HIWORD(wParam))
			{
			// 콤보박스에서 선택한 것이 바뀌었을 경우
			case CBN_SELCHANGE:
				g_Dlg->SelectOption();
				break;
			}
			break;
		case IDC_BUTTON_SAVE:
			g_Dlg->Save();
			break;
		case IDC_BUTTON_LOAD:
			g_Dlg->Load();
			break;
		}
		break;
	default:
		break;
	}

	return 0;
}
