#include "EditScene.h"
#include "../GameObject/TileMap.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../Input.h"
#include "../GameManager.h"
#include "EditDlg.h"
#include "../Resource/Texture/Texture.h"

CEditScene::CEditScene() :
	m_TileMapDlg(nullptr)
{
}

CEditScene::~CEditScene()
{
	CGameManager::GetInst()->SetEditMode(false);
	SAFE_DELETE(m_TileMapDlg);
}

bool CEditScene::Init()
{
	GetCamera()->SetResolution(1280.f, 720.f);
	GetCamera()->SetWorldResolution(1280.f, 720.f);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Input에서 만들어둔 BindKey에 CEditScene의 함수들을 등록해준다.
	CInput::GetInst()->AddBindFunction<CEditScene>("MoveUp", Input_Type::Push,
		this, &CEditScene::MoveUp);
	CInput::GetInst()->AddBindFunction<CEditScene>("MoveDown", Input_Type::Push,
		this, &CEditScene::MoveDown);
	CInput::GetInst()->AddBindFunction<CEditScene>("MoveRight", Input_Type::Push,
		this, &CEditScene::MoveRight);
	CInput::GetInst()->AddBindFunction<CEditScene>("MoveLeft", Input_Type::Push,
		this, &CEditScene::MoveLeft);
	CInput::GetInst()->AddBindFunction<CEditScene>("OpenTileMapEditor", Input_Type::Down,
		this, &CEditScene::OpenTileMapEditor);
	CInput::GetInst()->AddBindFunction<CEditScene>("Fire", Input_Type::Push,
		this, &CEditScene::MouseLButtonDrag);

	CGameManager::GetInst()->SetEditMode(true);

	return true;
}

void CEditScene::CreateTileMap(int CountX, int CountY, int SizeX, int SizeY)
{
	if (!m_TileMap)
		m_TileMap = CreateObject<CTileMap>("TileMap");

	m_TileMap->CreateTile(CountX, CountY, Vector2((float)SizeX, (float)SizeY));
}

void CEditScene::SetTileTexture(CTexture* Texture)
{
	if (m_TileMap)
		m_TileMap->SetTileTexture(Texture);
}

void CEditScene::LoadTileMap(const char* FullPath)
{
	if (!m_TileMap)
		m_TileMap = CreateObject<CTileMap>("TileMap");

	m_TileMap->LoadFullPath(FullPath);
}

// 카메라를 왼쪽으로 이동
void CEditScene::MoveLeft()
{
	Vector2 CameraPos = GetCamera()->GetPos();

	GetCamera()->SetPos(CameraPos + Vector2(-500.f * DELTA_TIME, 0.f));
}

// 카메라를 오른쪽으로 이동
void CEditScene::MoveRight()
{
	Vector2 CameraPos = GetCamera()->GetPos();

	GetCamera()->SetPos(CameraPos + Vector2(500.f * DELTA_TIME, 0.f));
}

// 카메라를 위쪽으로 이동
void CEditScene::MoveUp()
{
	Vector2 CameraPos = GetCamera()->GetPos();

	GetCamera()->SetPos(CameraPos + Vector2(0.f, -500.f * DELTA_TIME));
}

// 카메라를 아래쪽으로 이동
void CEditScene::MoveDown()
{
	Vector2 CameraPos = GetCamera()->GetPos();

	GetCamera()->SetPos(CameraPos + Vector2(0.f, 500.f * DELTA_TIME));
}

void CEditScene::OpenTileMapEditor()
{
	if (!m_TileMapDlg)
	{
		m_TileMapDlg = new CEditDlg;

		m_TileMapDlg->m_Scene = this;
		m_TileMapDlg->Init();
	}
}

void CEditScene::MouseLButtonDrag()
{
	// 생성된 타일맵이 없으면 무시한다.
	if (!m_TileMap)
		return;

	// 마우스가 화면 밖을 벗어나도 드래그를 중단한다.
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	CCamera* Camera = GetCamera();

	if (MousePos.x < 0.f || MousePos.x > Camera->GetResolution().x ||
		MousePos.y < 0.f || MousePos.y > Camera->GetResolution().y)
		return;

	switch (m_TileMapDlg->GetBrushType())
	{
	case ETile_Brush::Frame:
		ChangeFrame();
		break;
	case ETile_Brush::Option:
		ChangeOption();
		break;
	}
}

void CEditScene::ChangeFrame()
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorldPos();

	const AnimationFrameData* Frame = m_TileMapDlg->GetTileFrameData();

	if (!Frame)
		return;

	// 마우스 위치에 있는 타일의 프레임을 변경한다.
	m_TileMap->SetTileFrame(MousePos, Frame->Start, Frame->End);
	// 타일 출력 체크박스에 따라 타일의 출력 여부를 정해준다.
	m_TileMap->SetTileRender(MousePos, m_TileMapDlg->GetCheckRender());
	// 타일 충돌 체크박스에 따라 타일의 충돌 여부를 정해준다.
	m_TileMap->SetTileSideCollision(MousePos, m_TileMapDlg->GetCheckSideCollision());
}

void CEditScene::ChangeOption()
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorldPos();

	ETile_Option SelectOption = m_TileMapDlg->GetSelectOption();

	// 마우스 위치에 있는 타일의 옵션을 변경한다.
	m_TileMap->ChangeTileOption(MousePos, SelectOption);
	// 타일 출력 체크박스에 따라 타일의 출력 여부를 정해준다.
	m_TileMap->SetTileRender(MousePos, m_TileMapDlg->GetCheckRender());
	// 타일 충돌 체크박스에 따라 타일의 충돌 여부를 정해준다.
	m_TileMap->SetTileSideCollision(MousePos, m_TileMapDlg->GetCheckSideCollision());
}
