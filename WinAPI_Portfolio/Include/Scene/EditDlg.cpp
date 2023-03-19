#include "EditDlg.h"
#include "../GameManager.h"
#include "../resource.h"
#include "EditScene.h"
#include "../PathManager.h"
#include "SceneResource.h"
#include "../Resource/Texture/Texture.h"
#include "../GameObject/TileMap.h"

// WndProc()�� static�̹Ƿ� �� Ŭ������ �ִ� �Լ��� ����ϱ� ���� ���������� ������ش�.
// (�������� ��ſ� static������ ���� ��)
CEditDlg* g_Dlg = nullptr;

CEditDlg::CEditDlg() :
	m_SelectTextureName{}
{
	g_Dlg = this;
	m_SelectTextureIndex = -1;			// ���� ���õ� �� ���ٴ� ������ -1�� �ʱ�ȭ
	m_SelectTextureFrameIndex = -1;		// ���� ���õ� �� ���ٴ� ������ -1�� �ʱ�ȭ
	m_BrushType = ETile_Brush::Frame;
	m_SelectOption = ETile_Option::Normal;
}

CEditDlg::~CEditDlg()
{
}

bool CEditDlg::Init()
{
	CGameManager::GetInst()->SetEditMode(true);

	// Modal : �θ�������� ���߰� ������ ���̾�α׸� ����
	// Modeless : �θ�������� ���̾�α� ��� ����
	m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(),
		MAKEINTRESOURCE(IDD_DIALOG_EDIT), CGameManager::GetInst()->GetWindowHandle(),
		CEditDlg::WndProc);

	ShowWindow(m_hDlg, SW_SHOW);

	// ���� �Է�ĭ�� �⺻���� �� ���ڸ� �����ش�.
	// ������ ���ڴ� signed unsigned ����
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

	// �귯�� �޺��ڽ��� �����͸� �߰��Ѵ�.
	SendMessage(m_hBrushTypeCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Frame"));
	SendMessage(m_hBrushTypeCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Option"));

	// Ÿ�� �ɼ� �޺��ڽ��� �����͸� �߰��Ѵ�.
	SendMessage(m_hOptionCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Normal"));
	SendMessage(m_hOptionCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("ImpossibleToMove"));

	// ������ �޺��ڽ��� �ʱ� ���¸� 0������ �����Ѵ�.
	SendMessage(m_hBrushTypeCombo, CB_SETCURSEL, 0, 0);
	SendMessage(m_hOptionCombo, CB_SETCURSEL, 0, 0);

	// ��� üũ�ڽ��� �ʱ� ���¸� üũ�� �����Ѵ�.
	SendMessage(m_hRenderCheckBox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

	return true;
}

void CEditDlg::CreateTileMap()
{
	// �Ʒ��� �Լ����� ���۵ƴ��� Ȯ���� ����
	BOOL Transfer = FALSE;

	// Ÿ�ϸ��� ���� ���� ������ ����� �Է��ϴ� ĭ�� �Է��� ���ڸ� �޾ƿ´�.
	int CountX = GetDlgItemInt(m_hDlg, IDC_EDIT_COUNTX, &Transfer, TRUE);
	int CountY = GetDlgItemInt(m_hDlg, IDC_EDIT_COUNTY, &Transfer, TRUE);
	int SizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_SIZEX, &Transfer, TRUE);
	int SizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_SIZEY, &Transfer, TRUE);

	// �޾ƿ� ������ Ÿ�ϸ��� �����Ѵ�.
	m_Scene->CreateTileMap(CountX, CountY, SizeX, SizeY);
}

void CEditDlg::LoadTexture()
{
	TCHAR FilePath[MAX_PATH] = {};

	// �̹��� ������ �ҷ����� ���� ���� ���̾�α׸� �����.
	// (���� ���̾�α� ������ ��� �� ��Ĵ�� �ϸ� ��)
	OPENFILENAME OpenFile = {};

	// ����ü ũ��� ����ü ũ��� ����
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// ���� ���̾�α��� �ڵ鷯�� ������ ���̾�α�
	OpenFile.hwndOwner = m_hDlg;
	// ������ ���Ϸ� ��� ���ϰ� ��Ʈ�� ���ϸ� �����ֵ��� ���͸� �߰��Ѵ�.
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0BmpFile\0*.bmp");
	// FilePath�� ��ü��ΰ� ���´�.
	OpenFile.lpstrFile = FilePath;
	// ���(�迭)�� �ִ� ����
	OpenFile.nMaxFile = MAX_PATH;
	// �⺻ ��η� �ؽ�ó ������ ��θ� �����Ѵ�.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0�� �ƴ϶�� ����ü�� ����� �޾����Դٴ� ���̴�.
	if (GetOpenFileName(&OpenFile) != 0)
	{
		TCHAR FileName[128] = {};

		// ���� �̸��� �޾ƿ��� �ǹǷ� ����̺�, ���丮, Ȯ���ڴ� nullptr,
		// ũ��� 0���� �д�.
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

		char TextureName[128] = {};

#ifdef UNICODE

		// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);

		// ���� �̸��� �����ڵ忡�� ��Ƽ����Ʈ�� �ٲ��ش�.
		WideCharToMultiByte(CP_ACP, 0, FileName, -1,
			TextureName, PathLength, 0, 0);

#else

		strcpy_s(TextureName, FileName);

#endif // UNICODE

		// ����Ʈ�ڽ��� �ؽ�ó �̸��� �߰��Ѵ�.
		SendMessage(m_hTextureListBox, LB_ADDSTRING, 0, (LPARAM)FileName);

		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(TextureName, FilePath))
			return;

		m_Scene->GetSceneResource()->SetColorKey(TextureName, 255, 0, 255);

		// �� �ؽ�ó�� ���� ������ �迭�� �߰��Ѵ�.
		std::vector<AnimationFrameData> vecData;
		m_vecTextureFrame.push_back(vecData);
	}
}

void CEditDlg::SelectTexture()
{
	// �ؽ�ó ����Ʈ�ڽ����� ���� ���õ� ��(LB_GETCURSEL)�� �ε����� �������� �Ѵ�.
	m_SelectTextureIndex = (int)SendMessage(m_hTextureListBox, LB_GETCURSEL, 0, 0);

	// �ε����� ����� ������ ���
	if (m_SelectTextureIndex != -1)
	{
		memset(m_SelectTextureName, 0, sizeof(TCHAR) * 128);
		// LB_GETTEXT�� �̿��� �ؽ�ó �̸��� ���´�.
		SendMessage(m_hTextureListBox, LB_GETTEXT, m_SelectTextureIndex, (LPARAM)m_SelectTextureName);

		char TextureName[128] = {};

#ifdef UNICODE

		// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, m_SelectTextureName, -1, 0, 0, 0, 0);

		// ���� �̸��� �����ڵ忡�� ��Ƽ����Ʈ�� �ٲ��ش�.
		WideCharToMultiByte(CP_ACP, 0, m_SelectTextureName, -1,
			TextureName, PathLength, 0, 0);

#else

		strcpy_s(TextureName, FileName);

#endif // UNICODE

		// ���� �ؽ�ó �̸����� �ؽ�ó�� ã�Ƽ� Ÿ�ϸ��� �ؽ�ó�� �����ش�.
		m_SelectTexture = m_Scene->GetSceneResource()->FindTexture(TextureName);

		m_Scene->SetTileTexture(m_SelectTexture);

		m_SelectTextureFrameIndex = -1;
	}
}

void CEditDlg::AddFrame()
{
	// ���õ� �ؽ�ó�� ������ �����Ѵ�.
	if (m_SelectTextureIndex == -1)
		return;

	// �Ʒ��� �Լ����� ���۵ƴ��� Ȯ���� ����
	BOOL Transfer = FALSE;

	// �ؽ�ó �������� ���۰� �� ��ǥ�� �Է��ϴ� ĭ�� �Է��� ���ڸ� �޾ƿ´�.
	int StartX = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, &Transfer, TRUE);
	int StartY = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, &Transfer, TRUE);
	int EndX = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, &Transfer, TRUE);
	int EndY = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, &Transfer, TRUE);
	
	// ������ �����Ϳ� �Է��� ���� �߰����ش�.
	AnimationFrameData Data;
	Data.Start.x = (float)StartX;
	Data.Start.y = (float)StartY;
	Data.End.x = (float)EndX;
	Data.End.y = (float)EndY;

	TCHAR ListData[32] = {};

	// ������ �ؽ�ó�� �߰��� ������ ������ ������ ���ڿ��� ��ȯ
	wsprintf(ListData, TEXT("%d"), (int)m_vecTextureFrame[m_SelectTextureIndex].size());

	// ��ȯ�� ���ڿ��� ����Ʈ�ڽ��� ����Ѵ�.
	SendMessage(m_hTextureFrameListBox, LB_ADDSTRING, 0, (LPARAM)ListData);

	m_vecTextureFrame[m_SelectTextureIndex].push_back(Data);
}

void CEditDlg::SelectFrame()
{
	// ���õ� �ؽ�ó�� ������ �����Ѵ�.
	if (m_SelectTextureIndex == -1)
		return;

	// ������ ����Ʈ�ڽ����� ���� ���õ� ��(LB_GETCURSEL)�� �ε����� �������� �Ѵ�.
	m_SelectTextureFrameIndex = (int)SendMessage(m_hTextureFrameListBox, LB_GETCURSEL, 0, 0);

	if (m_SelectTextureFrameIndex != -1)
	{
		// ������ �ؽ�ó�� ������ �����͸� �����´�.
		AnimationFrameData Data = m_vecTextureFrame[m_SelectTextureIndex][m_SelectTextureFrameIndex];

		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, (int)Data.Start.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, (int)Data.Start.y, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, (int)Data.End.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, (int)Data.End.y, TRUE);
	}
}

void CEditDlg::SelectBrushType()
{
	// �귯�� �޺��ڽ����� ���� ���õ� ��(CB_GETCURSEL)�� �ε����� �������� �Ѵ�.
	m_BrushType = (ETile_Brush)SendMessage(m_hBrushTypeCombo, CB_GETCURSEL, 0, 0);
}

void CEditDlg::SelectOption()
{
	// Ÿ�� �ɼ� �޺��ڽ����� ���� ���õ� ��(CB_GETCURSEL)�� �ε����� �������� �Ѵ�.
	m_SelectOption = (ETile_Option)SendMessage(m_hOptionCombo, CB_GETCURSEL, 0, 0);
}

void CEditDlg::Save()
{
	// ������ Ÿ�ϸ��� ������ �������� �ʴ´�.
	if (!m_Scene->GetTileMap())
		return;

	TCHAR FilePath[MAX_PATH] = {};

	// Ÿ�ϸ� ������ ���Ϸ� �����ϱ� ���� ���� ���̾�α׸� �����.
	OPENFILENAME OpenFile = {};

	// ����ü ũ��� ����ü ũ��� ����
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// ���� ���̾�α��� �ڵ鷯�� ������ ���̾�α�
	OpenFile.hwndOwner = m_hDlg;
	// ������ ���Ϸ� ��� ���ϰ� Ÿ�ϸ� ���ϸ� �����ֵ��� ���͸� �߰��Ѵ�.
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0TileMapFile\0*.tmp");
	// FilePath�� ��ü��ΰ� ���´�.
	OpenFile.lpstrFile = FilePath;
	// ���(�迭)�� �ִ� ����
	OpenFile.nMaxFile = MAX_PATH;
	// �⺻ ��η� �ؽ�ó ������ ��θ� �����Ѵ�.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0�� �ƴ϶�� ������ ����� �Ǿ��ٴ� ���̴�.
	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

#ifdef UNICODE

		// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);

		// ��� �̸��� �����ڵ忡�� ��Ƽ����Ʈ�� �ٲ��ش�.
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

	// Ÿ�ϸ� ������ ���Ϸ� �����ϱ� ���� ���� ���̾�α׸� �����.
	OPENFILENAME OpenFile = {};

	// ����ü ũ��� ����ü ũ��� ����
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// ���� ���̾�α��� �ڵ鷯�� ������ ���̾�α�
	OpenFile.hwndOwner = m_hDlg;
	// ������ ���Ϸ� ��� ���ϰ� Ÿ�ϸ� ���ϸ� �����ֵ��� ���͸� �߰��Ѵ�.
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0TileMapFile\0*.tmp");
	// FilePath�� ��ü��ΰ� ���´�.
	OpenFile.lpstrFile = FilePath;
	// ���(�迭)�� �ִ� ����
	OpenFile.nMaxFile = MAX_PATH;
	// �⺻ ��η� �ؽ�ó ������ ��θ� �����Ѵ�.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0�� �ƴ϶�� ������ ����� �Ǿ��ٴ� ���̴�.
	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

#ifdef UNICODE

		// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);

		// ��� �̸��� �����ڵ忡�� ��Ƽ����Ʈ�� �ٲ��ش�.
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

	// �̹��� ������ �ҷ����� ���� ���� ���̾�α׸� �����.
	// (���� ���̾�α� ������ ��� �� ��Ĵ�� �ϸ� ��)
	OPENFILENAME OpenFile = {};

	// ����ü ũ��� ����ü ũ��� ����
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// ���� ���̾�α��� �ڵ鷯�� ������ ���̾�α�
	OpenFile.hwndOwner = m_hDlg;
	// ������ ���Ϸ� ��� ���ϰ� ��Ʈ�� ���ϸ� �����ֵ��� ���͸� �߰��Ѵ�.
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0BmpFile\0*.bmp");
	// FilePath�� ��ü��ΰ� ���´�.
	OpenFile.lpstrFile = FilePath;
	// ���(�迭)�� �ִ� ����
	OpenFile.nMaxFile = MAX_PATH;
	// �⺻ ��η� �ؽ�ó ������ ��θ� �����Ѵ�.
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 0�� �ƴ϶�� ����ü�� ����� �޾����Դٴ� ���̴�.
	if (GetOpenFileName(&OpenFile) != 0)
	{
		TCHAR FileName[128] = {};

		// ���� �̸��� �޾ƿ��� �ǹǷ� ����̺�, ���丮, Ȯ���ڴ� nullptr,
		// ũ��� 0���� �д�.
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

		char TextureName[128] = {};

#ifdef UNICODE

		// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
		int PathLength = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);

		// ���� �̸��� �����ڵ忡�� ��Ƽ����Ʈ�� �ٲ��ش�.
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
		// LOWORD : 4����Ʈ�� WPARAM�� ���� 16��Ʈ�� �޾ƿ´�.
		switch (LOWORD(wParam))
		{
		// ���� ��ư�� ������ Ÿ�ϸ��� �����Ѵ�.
		case IDC_BUTTON_CREATE_TILEMAP:
			g_Dlg->CreateTileMap();
			break;
		// �̹��� �ҷ����� ��ư�� ������ �ؽ�ó�� �ҷ��´�.
		case IDC_BUTTON_LOADTEXTURE:
			g_Dlg->LoadTexture();
			break;
		// ������ �߰� ��ư�� ������ ������ �����͸� �����Ѵ�.
		case IDC_BUTTON_ADD_FRAME:
			g_Dlg->AddFrame();
			break;
		// ��� �ҷ����� ��ư
		case IDC_BUTTON_LOADTEXTURE_BACK:
			g_Dlg->LoadTextureBack();
			break;
		// �ؽ�ó ����Ʈ�ڽ��� Ŭ���� ��� ������ �ؽ�ó�� Ÿ�ϸ� �ؽ�ó�� ��µȴ�.
		case IDC_LIST_TEXTURE:
			// HIWORD : 4����Ʈ�� WPARAM�� ���� 16��Ʈ�� �޾ƿ´�.
			switch (HIWORD(wParam))
			{
			// ����Ʈ�ڽ����� ������ ���� �ٲ���� ���
			case LBN_SELCHANGE:
				g_Dlg->SelectTexture();
				break;
			}
			break;
		// �ؽ�ó ������ ����Ʈ�ڽ��� Ŭ���� ��� ������ �ε����� ������ ������ ��µȴ�.
		case IDC_LIST_TEXTURE_FRAME:
			switch (HIWORD(wParam))
			{
			// ����Ʈ�ڽ����� ������ ���� �ٲ���� ���
			case LBN_SELCHANGE:
				g_Dlg->SelectFrame();
				break;
			}
			break;
		// �귯�� �޺��ڽ� ����
		case IDC_COMBO_BRUSH_TYPE:
			switch (HIWORD(wParam))
			{
			// �޺��ڽ����� ������ ���� �ٲ���� ���
			case CBN_SELCHANGE:
				g_Dlg->SelectBrushType();
				break;
			}
			break;
		// Ÿ�� �ɼ� �޺��ڽ� ����
		case IDC_COMBO_OPTION:
			switch (HIWORD(wParam))
			{
			// �޺��ڽ����� ������ ���� �ٲ���� ���
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
