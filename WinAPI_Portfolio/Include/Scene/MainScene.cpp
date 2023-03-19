#include "MainScene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Opponent.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Background.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../Input.h"
#include "../Widget/CharacterHUD.h"
#include "../GameObject/TileMap.h"
#include "../Resource/ResourceManager.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CreateAnimationSequence();
	CreateSound();

	GetCamera()->SetResolution(1280.f, 720.f);
	GetCamera()->SetWorldResolution(1500.f, 1200.f);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	//CreateObject<CBackground>("Background");

	m_Opponent = CreateObject<COpponent>("Opponent");

	CPlayer* Player = CreateObject<CPlayer>("Player");

	SetPlayer(Player);

	//m_Player->SetPlayable(true);

	//m_Monster = CreateObject<CMonster>("Monster");

	m_Player->SetOpponent(m_Opponent);

	m_Opponent->SetOpponent(m_Player);

	//m_Opponent->SetPlayable(true);
	SetPlayable(true);

	GetCamera()->SetTargets(m_Player, m_Opponent);
	GetCamera()->SetDefaultY(m_Player->GetPos().y);

	CTileMap* TileMap = CreateObject<CTileMap>("TileMap");
	TileMap->LoadFileName("Stage.tmp", MAP_PATH);

	/*CInput::GetInst()->AddBindFunction<CMainScene>("Cam1", Input_Type::Down,
		this, &CMainScene::Cam1Key);
	CInput::GetInst()->AddBindFunction<CMainScene>("Cam2", Input_Type::Down,
		this, &CMainScene::Cam2Key);*/

	CreateWidgetWindow<CCharacterHUD>("CharacterHUD");

	CInput::GetInst()->SetCursor(false);

	return true;
}

void CMainScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);

	if (FindWidget<CCharacterHUD>("CharacterHUD")->GetStart() ||
		FindWidget<CCharacterHUD>("CharacterHUD")->GetFinish())
		SetPlayable(false);
	else
		SetPlayable(true);
}

void CMainScene::CreateAnimationSequence()
{
	CreateRagnaAnimation();
	CreateNoelAnimation();
}

// 라그나 애니메이션
void CMainScene::CreateRagnaAnimation()
{
	/* 캐릭터가 우측을 보고 있을 때 */
#pragma region Right

	// Idle 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 11; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Idle/rg000_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightIdle", "RagnaRightIdle",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 12; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightIdle", 0.f, 0.f, 806.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightIdle", 255, 0, 255);
	}

	// 앞으로 이동 애니메이션(Sprtie 방식)
	GetSceneResource()->CreateAniamtionSequence("RagnaRightWalkFoward", "RagnaRightWalkFoward",
		TEXT("Player/Right/Ragna/WalkFoward/WalkFowardRight.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 9; i++)
	{
		GetSceneResource()->AddAnimationFrame("RagnaRightWalkFoward", 305.f * i, 0.f, 305.f, 488.f);
	}

	GetSceneResource()->SetColorKey("RagnaRightWalkFoward", 255, 0, 255);

	// 뒤로 이동 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/WalkBackward/rg031_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightWalkBackward", "RagnaRightWalkBackward",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 8; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightWalkBackward", 0.f, 0.f, 240.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightWalkBackward", 255, 0, 255);
	}

	// 방향 전환 애니메이션(Frame 방식)
	//{
	//	std::vector<std::wstring> vecFileName;

	//	for (int i = 0; i <= 2; i++)
	//	{
	//		TCHAR FileName[MAX_PATH] = {};
	//		// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
	//		wsprintf(FileName, TEXT("Player/Right/Ragna/Idle/rg003_%d.bmp"), i);
	//		vecFileName.push_back(FileName);
	//	}

	//	CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightIdleToLeft", "RagnaRightIdleToLeft",
	//		vecFileName, TEXTURE_PATH);

	//	for (int i = 0; i < 3; i++)
	//	{
	//		CResourceManager::GetInst()->AddAnimationFrame("RagnaRightIdleToLeft", 0.f, 0.f, 216.f, 428.f);
	//	}

	//	CResourceManager::GetInst()->SetColorKey("RagnaRightIdleToLeft", 255, 0, 255);
	//}

	// 앉기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 1; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Crouch/rg010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightCrouchStart", "RagnaRightCrouchStart",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightCrouchStart", 0.f, 0.f, 352.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightCrouchStart", 255, 0, 255);
	}

	// 앉기 유지(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 13; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Crouch/rg010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightCrouchStay", "RagnaRightCrouchStay",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 12; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightCrouchStay", 0.f, 0.f, 352.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightCrouchStay", 255, 0, 255);
	}

	// 서기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i >= 0; i--)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Crouch/rg010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightCrouchEnd", "RagnaRightCrouchEnd",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightCrouchEnd", 0.f, 0.f, 352.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightCrouchEnd", 255, 0, 255);
	}

	// 점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Jump/rg020_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightJump", "RagnaRightJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightJump", 0.f, 0.f, 380.f, 518.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightJump", 255, 0, 255);
	}

	// 착지 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 4; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Jump/rg024_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightLand", "RagnaRightLand",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 5; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightLand", 0.f, 0.f, 362.f, 449.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightLand", 255, 0, 255);
	}

	// 공격(5A) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 5; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Attack/5A/rg200_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightAttack5A", "RagnaRightAttack5A",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 6; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightAttack5A", 0.f, 0.f, 506.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightAttack5A", 255, 0, 255);
	}

	// 공격(5B) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 16; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Attack/5B/rg211_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightAttack5B", "RagnaRightAttack5B",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 17; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightAttack5B", 0.f, 0.f, 806.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightAttack5B", 255, 0, 255);
	}

	// 공격(5C) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 18; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Attack/5C/rg202_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightAttack5C", "RagnaRightAttack5C",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 19; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightAttack5C", 0.f, 0.f, 1200.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightAttack5C", 255, 0, 255);
	}

	// 공격(5D) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 29; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Attack/5D/rg203_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightAttack5D", "RagnaRightAttack5D",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 30; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaRightAttack5D", 0.f, 0.f, 1200.f, 670.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaRightAttack5D", 255, 0, 255);
	}

	// 이펙트 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 10; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Ragna/Effect/vrrgef203atk_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RightHitEffectD", "RightHitEffectD",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 11; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RightHitEffectD", 0.f, 0.f, 1200.f, 630.f);
		}

		CResourceManager::GetInst()->SetColorKey("RightHitEffectD", 255, 0, 255);
	}

#pragma endregion

	/* 캐릭터가 좌측을 보고 있을 때 */
#pragma region Left

	// Idle 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 11; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Idle/rg000_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftIdle", "RagnaLeftIdle",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 12; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftIdle", 0.f, 0.f, 806.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftIdle", 255, 0, 255);
	}

	// 앞으로 이동 애니메이션(Sprite 방식)
	GetSceneResource()->CreateAniamtionSequence("RagnaLeftWalkFoward", "RagnaLeftWalkFoward",
		TEXT("Player/Left/Ragna/WalkFoward/WalkFowardLeft.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 9; i++)
	{
		GetSceneResource()->AddAnimationFrame("RagnaLeftWalkFoward", 305.f * i, 0.f, 305.f, 488.f);
	}

	GetSceneResource()->SetColorKey("RagnaLeftWalkFoward", 255, 0, 255);

	// 뒤로 이동 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/WalkBackward/rg031_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftWalkBackward", "RagnaLeftWalkBackward",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 8; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftWalkBackward", 0.f, 0.f, 240.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftWalkBackward", 255, 0, 255);
	}

	// 방향 전환 애니메이션(Frame 방식)
	//{
	//	std::vector<std::wstring> vecFileName;

	//	for (int i = 0; i <= 2; i++)
	//	{
	//		TCHAR FileName[MAX_PATH] = {};
	//		// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
	//		wsprintf(FileName, TEXT("Player/Left/Ragna/Idle/rg003_%d.bmp"), i);
	//		vecFileName.push_back(FileName);
	//	}

	//	CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftIdleToRight", "RagnaLeftIdleToRight",
	//		vecFileName, TEXTURE_PATH);

	//	for (int i = 0; i < 3; i++)
	//	{
	//		CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftIdleToRight", 0.f, 0.f, 216.f, 428.f);
	//	}

	//	CResourceManager::GetInst()->SetColorKey("RagnaLeftIdleToRight", 255, 0, 255);
	//}

	// 앉기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 1; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Crouch/rg010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftCrouchStart", "RagnaLeftCrouchStart",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftCrouchStart", 0.f, 0.f, 352.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftCrouchStart", 255, 0, 255);
	}

	// 앉기 유지(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 13; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Crouch/rg010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftCrouchStay", "RagnaLeftCrouchStay",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 12; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftCrouchStay", 0.f, 0.f, 352.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftCrouchStay", 255, 0, 255);
	}

	// 서기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i >= 0; i--)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Crouch/rg010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftCrouchEnd", "RagnaLeftCrouchEnd",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftCrouchEnd", 0.f, 0.f, 352.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftCrouchEnd", 255, 0, 255);
	}

	// 점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Jump/rg020_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftJump", "RagnaLeftJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftJump", 0.f, 0.f, 380.f, 518.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftJump", 255, 0, 255);
	}

	// 착지 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 4; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Jump/rg024_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftLand", "RagnaLeftLand",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 5; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftLand", 0.f, 0.f, 362.f, 449.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftLand", 255, 0, 255);
	}

	// 공격(5A) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 5; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Attack/5A/rg200_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftAttack5A", "RagnaLeftAttack5A",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 6; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftAttack5A", 0.f, 0.f, 506.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftAttack5A", 255, 0, 255);
	}

	// 공격(5B) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 16; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Attack/5B/rg211_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftAttack5B", "RagnaLeftAttack5B",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 17; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftAttack5B", 0.f, 0.f, 806.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftAttack5B", 255, 0, 255);
	}

	// 공격(5C) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 18; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Attack/5C/rg202_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftAttack5C", "RagnaLeftAttack5C",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 19; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftAttack5C", 0.f, 0.f, 1200.f, 488.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftAttack5C", 255, 0, 255);
	}

	// 공격(5D) 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 29; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Attack/5D/rg203_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftAttack5D", "RagnaLeftAttack5D",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 30; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftAttack5D", 0.f, 0.f, 1200.f, 670.f);
		}

		CResourceManager::GetInst()->SetColorKey("RagnaLeftAttack5D", 255, 0, 255);
	}

	// 이펙트 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 10; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Ragna/Effect/vrrgef203atk_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("LeftHitEffectD", "LeftHitEffectD",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 11; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("LeftHitEffectD", 0.f, 0.f, 1200.f, 630.f);
		}

		CResourceManager::GetInst()->SetColorKey("LeftHitEffectD", 255, 0, 255);
	}

#pragma endregion
}

// 노엘 애니메이션
void CMainScene::CreateNoelAnimation()
{
	/* 캐릭터가 우측을 보고 있을 때 */
#pragma region Right

	// Idle 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 7; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Idle/no000_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightIdle", "NoelRightIdle",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 8; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightIdle", 0.f, 0.f, 276.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightIdle", 255, 0, 255);
	}

	// 앞으로 이동 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 11; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/WalkFoward/no030_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightWalkFoward", "NoelRightWalkFoward",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 10; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightWalkFoward", 0.f, 0.f, 272.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightWalkFoward", 255, 0, 255);
	}

	// 뒤로 이동 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 10; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/WalkBackward/no031_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightWalkBackward", "NoelRightWalkBackward",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightWalkBackward", 0.f, 0.f, 248.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightWalkBackward", 255, 0, 255);
	}

	//// 방향 전환 애니메이션(Frame 방식)
	//{
	//	std::vector<std::wstring> vecFileName;

	//	for (int i = 0; i <= 2; i++)
	//	{
	//		TCHAR FileName[MAX_PATH] = {};
	//		// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
	//		wsprintf(FileName, TEXT("Player/Right/Ragna/Idle/rg003_%d.bmp"), i);
	//		vecFileName.push_back(FileName);
	//	}

	//	CResourceManager::GetInst()->CreateAniamtionSequence("RagnaRightIdleToLeft", "RagnaRightIdleToLeft",
	//		vecFileName, TEXTURE_PATH);

	//	for (int i = 0; i < 3; i++)
	//	{
	//		CResourceManager::GetInst()->AddAnimationFrame("RagnaRightIdleToLeft", 0.f, 0.f, 216.f, 428.f);
	//	}

	//	CResourceManager::GetInst()->SetColorKey("RagnaRightIdleToLeft", 255, 0, 255);
	//}

	// 앉기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 1; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Crouch/no010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightCrouchStart", "NoelRightCrouchStart",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightCrouchStart", 0.f, 0.f, 268.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightCrouchStart", 255, 0, 255);
	}

	// 앉기 유지(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 10; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Crouch/no010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightCrouchStay", "NoelRightCrouchStay",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightCrouchStay", 0.f, 0.f, 268.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightCrouchStay", 255, 0, 255);
	}

	// 서기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i >= 0; i--)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Crouch/no010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightCrouchEnd", "NoelRightCrouchEnd",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightCrouchEnd", 0.f, 0.f, 268.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightCrouchEnd", 255, 0, 255);
	}

	// 수직점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 7; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Jump/no020_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightJump", "NoelRightJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 8; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightJump", 0.f, 0.f, 284.f, 629.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightJump", 255, 0, 255);
	}

	// 앞점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Jump/no021_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightFrontJump", "NoelRightFrontJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightFrontJump", 0.f, 0.f, 366.f, 535.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightFrontJump", 255, 0, 255);
	}

	// 뒷점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Jump/no022_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightBackJump", "NoelRightBackJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightBackJump", 0.f, 0.f, 370.f, 524.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightBackJump", 255, 0, 255);
	}

	// 착지 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i <= 4; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Right/Noel/Jump/no024_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelRightLand", "NoelRightLand",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 4; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelRightLand", 0.f, 0.f, 284.f, 629.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelRightLand", 255, 0, 255);
	}

	//// 공격 애니메이션
	//GetSceneResource()->CreateAniamtionSequence("PlayerRightAttack", "PlayerRightAttack",
	//	TEXT("Player/Right/aswing.bmp"), TEXTURE_PATH);

	//for (int i = 0; i < 3; i++)
	//{
	//	GetSceneResource()->AddAnimationFrame("PlayerRightAttack", 176.f * i, 0.f, 176.f, 89.f);
	//}

	//GetSceneResource()->SetColorKey("PlayerRightAttack", 255, 0, 255);

#pragma endregion

	/* 캐릭터가 좌측을 보고 있을 때 */
#pragma region Left

	// Idle 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 7; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Idle/no000_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftIdle", "NoelLeftIdle",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 8; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftIdle", 0.f, 0.f, 276.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftIdle", 255, 0, 255);
	}

	// 앞으로 이동 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 11; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/WalkFoward/no030_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftWalkFoward", "NoelLeftWalkFoward",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 10; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftWalkFoward", 0.f, 0.f, 272.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftWalkFoward", 255, 0, 255);
	}

	// 뒤로 이동 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 10; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/WalkBackward/no031_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftWalkBackward", "NoelLeftWalkBackward",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftWalkBackward", 0.f, 0.f, 248.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftWalkBackward", 255, 0, 255);
	}

	//// 방향 전환 애니메이션(Frame 방식)
	//{
	//	std::vector<std::wstring> vecFileName;

	//	for (int i = 0; i <= 2; i++)
	//	{
	//		TCHAR FileName[MAX_PATH] = {};
	//		// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
	//		wsprintf(FileName, TEXT("Player/Left/Ragna/Idle/rg003_%d.bmp"), i);
	//		vecFileName.push_back(FileName);
	//	}

	//	CResourceManager::GetInst()->CreateAniamtionSequence("RagnaLeftIdleToRight", "RagnaLeftIdleToRight",
	//		vecFileName, TEXTURE_PATH);

	//	for (int i = 0; i < 3; i++)
	//	{
	//		CResourceManager::GetInst()->AddAnimationFrame("RagnaLeftIdleToRight", 0.f, 0.f, 216.f, 428.f);
	//	}

	//	CResourceManager::GetInst()->SetColorKey("RagnaLeftIdleToRight", 255, 0, 255);
	//}

	// 앉기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 1; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Crouch/no010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftCrouchStart", "NoelLeftCrouchStart",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftCrouchStart", 0.f, 0.f, 268.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftCrouchStart", 255, 0, 255);
	}

	// 앉기 유지(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 2; i <= 10; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Crouch/no010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftCrouchStay", "NoelLeftCrouchStay",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftCrouchStay", 0.f, 0.f, 268.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftCrouchStay", 255, 0, 255);
	}

	// 서기 전환 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i >= 0; i--)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Crouch/no010_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftCrouchEnd", "NoelLeftCrouchEnd",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 2; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftCrouchEnd", 0.f, 0.f, 268.f, 476.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftCrouchEnd", 255, 0, 255);
	}

	// 수직점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 7; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Jump/no020_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftJump", "NoelLeftJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 8; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftJump", 0.f, 0.f, 284.f, 629.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftJump", 255, 0, 255);
	}

	// 앞점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Jump/no021_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftFrontJump", "NoelLeftFrontJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftFrontJump", 0.f, 0.f, 366.f, 535.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftFrontJump", 255, 0, 255);
	}

	// 뒷점프 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 0; i <= 8; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Jump/no022_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftBackJump", "NoelLeftBackJump",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 9; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftBackJump", 0.f, 0.f, 370.f, 524.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftBackJump", 255, 0, 255);
	}

	// 착지 애니메이션(Frame 방식)
	{
		std::vector<std::wstring> vecFileName;

		for (int i = 1; i <= 4; i++)
		{
			TCHAR FileName[MAX_PATH] = {};
			// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
			wsprintf(FileName, TEXT("Player/Left/Noel/Jump/no024_%d.bmp"), i);
			vecFileName.push_back(FileName);
		}

		CResourceManager::GetInst()->CreateAniamtionSequence("NoelLeftLand", "NoelLeftLand",
			vecFileName, TEXTURE_PATH);

		for (int i = 0; i < 4; i++)
		{
			CResourceManager::GetInst()->AddAnimationFrame("NoelLeftLand", 0.f, 0.f, 284.f, 629.f);
		}

		CResourceManager::GetInst()->SetColorKey("NoelLeftLand", 255, 0, 255);
	}

#pragma endregion
}

void CMainScene::CreateSound()
{
	// BGM
	GetSceneResource()->LoadSound("BGM", "MainBGM", true, "BGM/StageBGM.wav");

	// Voice
	GetSceneResource()->LoadSound("Voice", "TeemoHit", false, "Ouch.mp3");
	GetSceneResource()->LoadSound("Voice", "PlayerHit", false, "Human_Good.mp3");
	GetSceneResource()->LoadSound("Voice", "RagnaJump", false, "Voice/Ragna/rg002a.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceA1", false, "Voice/Ragna/rg100b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceA2", false, "Voice/Ragna/rg101b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceB1", false, "Voice/Ragna/rg102b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceB2", false, "Voice/Ragna/rg103b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceC1", false, "Voice/Ragna/rg104b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceC2", false, "Voice/Ragna/rg105b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceD1", false, "Voice/Ragna/rg106b.wav");
	GetSceneResource()->LoadSound("Voice", "RagnaAttackVoiceD2", false, "Voice/Ragna/rg107b.wav");
	GetSceneResource()->SetVolume("Voice", 80);

	// SE
	GetSceneResource()->LoadSound("SE", "Walk1", false, "SE/Walk1.wav");
	GetSceneResource()->LoadSound("SE", "Walk2", false, "SE/Walk2.wav");
	GetSceneResource()->LoadSound("SE", "RagnaSE00", false, "SE/rgse_00.wav");
	GetSceneResource()->LoadSound("SE", "RagnaSE02", false, "SE/rgse_02.wav");
	GetSceneResource()->LoadSound("SE", "SwingGrap1_0", false, "SE/se00_13.wav");
	GetSceneResource()->LoadSound("SE", "SwingGrap1_2", false, "SE/se00_15.wav");
	GetSceneResource()->LoadSound("SE", "SwingGrap2_1", false, "SE/se00_17.wav");
	GetSceneResource()->LoadSound("SE", "SwingGrap2_2", false, "SE/se00_18.wav");
	GetSceneResource()->LoadSound("SE", "SwingPole_2", false, "SE/se00_27.wav");
	GetSceneResource()->SetVolume("SE", 70);

	// HitEffect
	GetSceneResource()->LoadSound("HitEffect", "RagnaAttack1", false, "HitEffect/se00_76.wav");
	GetSceneResource()->LoadSound("HitEffect", "RagnaAttack2", false, "HitEffect/se00_78.wav");
	GetSceneResource()->LoadSound("HitEffect", "RagnaAttack3", false, "HitEffect/se00_83.wav");

	// Announcer
	GetSceneResource()->LoadSound("Announcer", "Intro", false, "Announcer/ns012.wav");
	GetSceneResource()->LoadSound("Announcer", "Rebel", false, "Announcer/ns013.wav");
	GetSceneResource()->LoadSound("Announcer", "One", false, "Announcer/ns001.wav");
	GetSceneResource()->LoadSound("Announcer", "Action", false, "Announcer/ns014.wav");
	GetSceneResource()->LoadSound("Announcer", "Finish", false, "Announcer/ns020.wav");

	GetSceneResource()->SoundPlay("MainBGM");
	GetSceneResource()->SetVolume("BGM", 20);
}

// F1키를 누르면 카메라의 타겟이 플레이어로 변경
void CMainScene::Cam1Key()
{
	//GetCamera()->SetTarget(m_Player);
	m_Player->SetPlayable(true);
	m_Opponent->SetPlayable(false);
}

// F2키를 누르면 카메라의 타겟이 몬스터로 변경
void CMainScene::Cam2Key()
{
	//GetCamera()->SetTarget(m_Opponent);
	m_Player->SetPlayable(false);
	m_Opponent->SetPlayable(true);
}

void CMainScene::SetPlayable(bool Playable)
{
	m_Player->SetPlayable(Playable);
	m_Opponent->SetPlayable(Playable);
}
