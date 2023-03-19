#include "EndScene.h"
#include "../GameObject/Background.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../Input.h"
#include "../GameManager.h"

CEndScene::CEndScene()
{
}

CEndScene::~CEndScene()
{
}

bool CEndScene::Init()
{
	GetCamera()->SetResolution(1280.f, 720.f);
	GetCamera()->SetWorldResolution(1280.f, 720.f);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CreateObject<CBackground>("Background");

	CInput::GetInst()->AddBindFunction<CEndScene>("Enter", Input_Type::Down, this, &CEndScene::EndGame);

	CInput::GetInst()->SetCursor(true);

	return true;
}

void CEndScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
}

void CEndScene::EndGame()
{
	CGameManager::GetInst()->Exit();
}
