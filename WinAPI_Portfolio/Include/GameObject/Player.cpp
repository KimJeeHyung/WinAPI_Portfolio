#include "Player.h"
#include "../Scene/Scene.h"
#include "Bullet.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneResource.h"
#include "../Collision/ColliderBox.h"
#include "../Widget/CharacterHUD.h"
#include "../Widget/WidgetComponent.h"
#include "../Widget/ProgressBar.h"
#include "../Widget/Text.h"
#include "Effect.h"
#include <time.h>

CPlayer::CPlayer() :
	m_Idle(true),
	m_CrouchStart(false),
	m_CrouchStay(false),
	m_CrouchToStand(false),
	m_Movable(true),
	m_Jumpable(true),
	m_JumpVoiceReady(true),
	m_Block(false),
	m_Body(nullptr),
	m_AttackLevel(0)
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& Obj) :
	CCharacter(Obj),
	m_CrouchStart(false),
	m_CrouchStay(false),
	m_CrouchToStand(false)
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	CGameObject::Init();

	SetPos(800.f, 900.f);
	SetSize(100.f, 100.f);
	SetPivot(0.5f, 0.7f);
	m_MoveSpeed = 600.f;

	// 1번 스킬과 2번 스킬의 쿨타임을 지정한 후 배열에 등록
	SkillCoolDownInfo Info = {};

	// 1번
	Info.CoolDown = 5.f;
	m_vecCoolDown.push_back(Info);

	InitAnimation();

	// 오른쪽 보고 있음.
	m_PlayerDir = 1;
	m_PrevDir = m_PlayerDir;

	// 공격 중이 아닐 때.
	m_Attack = false;

	// 충돌체 추가
	m_Body = AddCollider<CColliderBox>("Body");

	m_Body->SetExtent(150.f, 410.f);
	m_Body->SetOffset(0.f, -80.f);
	m_Body->SetCollisionProfile("Player");
	m_Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::Push);
	m_Body->SetCollisionEndFunction<CPlayer>(this, &CPlayer::PushEnd);

	// Input에서 만들어둔 BindKey에 Player의 이동 함수들을 등록해준다.
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", Input_Type::Push,
		this, &CPlayer::JumpKey);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", Input_Type::Up,
		this, &CPlayer::JumpKeyUp);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", Input_Type::Push,
		this, &CPlayer::Crouch);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", Input_Type::Up,
		this, &CPlayer::CrouchToIdle);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Push,
		this, &CPlayer::MoveFront);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Up,
		this, &CPlayer::MoveStop);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Push,
		this, &CPlayer::MoveBack);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Up,
		this, &CPlayer::MoveStop);

	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Down,
		this, &CPlayer::AttackA);
	CInput::GetInst()->AddBindFunction<CPlayer>("B", Input_Type::Down,
		this, &CPlayer::AttackB);
	CInput::GetInst()->AddBindFunction<CPlayer>("C", Input_Type::Down,
		this, &CPlayer::AttackC);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Down,
		this, &CPlayer::AttackD);

	CInput::GetInst()->AddBindFunction<CPlayer>("Jump", Input_Type::Push,
		this, &CPlayer::JumpKey);
	CInput::GetInst()->AddBindFunction<CPlayer>("Jump", Input_Type::Up,
		this, &CPlayer::JumpKeyUp);

	m_HP = 10000;
	m_HPMax = 10000;

	// 플레이어가 중력의 영향을 받게 한다.
	SetPhysicsSimulate(true);
	// 점프 속도
	SetJumpVelocity(80.f);
	SetGravityAccel(GRAVITY * 2.7f);
	// 옆 충돌 활성화
	SetSideWallCheck(true);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	DeltaTime *= m_TimeScale;

	size_t Size = m_vecCoolDown.size();

	for (size_t i = 0; i < Size; i++)
	{
		// i번째 스킬이 쿨타임 중이라면
		if (m_vecCoolDown[i].CoolDownEnable)
		{
			// 쿨타임 시간을 깎는다.
			m_vecCoolDown[i].CoolDown -= DeltaTime;

			// 쿨타임 시간이 지나면 스킬을 다시 사용가능하게 변경
			if (m_vecCoolDown[i].CoolDown <= 0.f)
				m_vecCoolDown[i].CoolDownEnable = false;
		}
	}

	// 상대와 닿아있을 경우 
	if (m_Block && !m_Jump)
	{
		// 자기가 보는 방향으로 상대를 밀어낸다.
		m_Opponent->MoveDir(Vector2((float)m_PlayerDir, 0.f));

		// 상대를 뚫고 지나갈 수 없게 해준다.
		if (m_Pos.x < m_Opponent->GetPos().x + m_Opponent->GetSize().x && m_Pos.y < m_Opponent->GetPos().y + m_Opponent->GetSize().y &&
			m_Pos.x + m_Size.x > m_Opponent->GetPos().x && m_Pos.y + m_Size.y > m_Opponent->GetPos().y)
		{
			float MoveX = 0.f;

			if (m_PlayerDir == -1)
				MoveX = m_Opponent->GetPos().x + m_Opponent->GetSize().x - m_Pos.x + 0.001f;
			else
				MoveX = m_Opponent->GetPos().x - (m_Pos.x + m_Size.x) - 0.001f;

			m_Pos.x += MoveX;
			//m_Move.x += MoveX;
		}
	}

	// 카메라 영역을 벗어나지 않도록 해준다.
	if (m_Pos.x - 100.f < m_Scene->GetCamera()->GetPos().x)
	{
		float MoveX = m_Scene->GetCamera()->GetPos().x - m_Pos.x + 100.f + 0.001f;

		m_Pos.x += MoveX;
		//m_Move.x += MoveX;
	}
	else if (m_Pos.x + m_Size.x > m_Scene->GetCamera()->GetPos().x + m_Scene->GetCamera()->GetResolution().x)
	{
		float MoveX = m_Scene->GetCamera()->GetPos().x + m_Scene->GetCamera()->GetResolution().x - (m_Pos.x + m_Size.x) - 0.001f;

		m_Pos.x += MoveX;
		//m_Move.x += MoveX;
	}
}

// 정확한 이동량 계산을 위해 PostUpdate에 작성한다.
void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	// 상대 캐릭터가 플레이어보다 오른쪽에 있으면 플레이어는 오른쪽을 바라보고
	// 왼쪽에 있으면 왼쪽을 바라본다.
	if (m_Opponent->GetPos().x > m_Pos.x)
		m_PlayerDir = 1;
	else
		m_PlayerDir = -1;

	//  플레이어가 왼쪽을 보고 있을 경우 1번 인덱스(왼쪽을 보는 애니메이션) 사용
	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	if (m_Walk && !m_Jump && !m_Land && !m_Attack)
	{
		// 플레이어가 보는 방향을 기준으로 앞으로 갈 때는 앞으로,
		// 뒤로 갈 때는 뒤로 걷는 애니메이션 재생
		if (m_MoveFront)
			ChangeAnimation(m_vecSequenceKey[AnimDirIndex][1]);
		else if (m_MoveBack)
			ChangeAnimation(m_vecSequenceKey[AnimDirIndex][2]);
	}
	// 플레이어가 선 자세에서 앉은 자세로 바뀌는 중일 때
	else if (m_CrouchStart && !m_CrouchStay && !m_CrouchToStand && !m_Jump && !m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][3]);
	}
	// 앉은 자세를 유지 중일 때
	else if (m_CrouchStay && !m_CrouchToStand && !m_Jump && !m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][4]);

		m_Body->SetExtent(200.f, 205.f);
		m_Body->SetOffset(50.f, 35.f);
	}
	// 앉은 자세에서 선 자세로 바뀌는 중일 때
	else if (m_CrouchToStand && !m_Jump && !m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][5]);
	}
	// 점프 중
	else if (m_Jump && m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][6]);

		if (m_JumpCount == 2)
		{
			// 2단 점프 후 점프 카운트를 1 더해 매 프레임마다 애니메이션이 초기화되지 않도록 해준다.
			m_JumpCount++;
			SetCurrentAniamtion(m_vecSequenceKey[AnimDirIndex][6]);
		}

		m_Body->SetExtent(150.f, 325.f);
		m_Body->SetOffset(0.f, -175.f);
	}
	// 점프 후 착지 중일 때
	else if (m_Land && !m_Attack)
	{
		// 착지 중에는 일시적으로 움직일 수 없다.
		m_Movable = false;
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][7]);

		m_Body->SetExtent(150.f, 410.f);
		m_Body->SetOffset(0.f, -80.f);
	}
	// 공격 포함 어떤 상태도 아닐 때만 움직이지 않을 때 Idle로 돌아간다.
	else if(m_Idle && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][0]);

		m_Body->SetExtent(150.f, 410.f);
		m_Body->SetOffset(0.f, -80.f);
	}
}

void CPlayer::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);

	// 출력이 끝날 때마다 이전 방향을 갱신한다.
	m_PrevDir = m_PlayerDir;
}

float CPlayer::InflictDamage(float Damage)
{
	Damage = CCharacter::InflictDamage(Damage);

	m_HP -= (int)Damage;

	/*if (m_HP <= 0)
	{
		SetActive(false);
	}*/

	m_Scene->FindWidget<CCharacterHUD>("CharacterHUD")->SetHP(m_HP / (float)m_HPMax, true);

	return Damage;
}

void CPlayer::InitAnimation()
{
	CreateAnimation();

	// 애니메이션 추가
	AddAnimation("RagnaRightIdle", true, 1.4f);
	AddAnimation("RagnaRightWalkFoward", true, 1.f, 1.f, true);
	AddAnimation("RagnaRightWalkBackward", true, 0.95f);
	AddAnimation("RagnaRightCrouchStart", false, 0.15f);
	AddAnimation("RagnaRightCrouchStay", true, 1.2f);
	AddAnimation("RagnaRightCrouchEnd", false, 0.15f);
	AddAnimation("RagnaRightJump", false);
	AddAnimation("RagnaRightLand", false, 0.25f);
	AddAnimation("RagnaRightAttack", false, 0.3f);
	AddAnimation("RagnaRightAttack5A", false, 0.25f);
	AddAnimation("RagnaRightAttack5B", false, 0.45f);
	AddAnimation("RagnaRightAttack5C", false, 0.5f);
	AddAnimation("RagnaRightAttack5D", false, 1.1f);

	AddAnimation("RagnaLeftIdle", true, 1.4f);
	AddAnimation("RagnaLeftWalkFoward", true);
	AddAnimation("RagnaLeftWalkBackward", true, 0.95f);
	AddAnimation("RagnaLeftCrouchStart", false, 0.15f);
	AddAnimation("RagnaLeftCrouchStay", true, 1.2f);
	AddAnimation("RagnaLeftCrouchEnd", false, 0.15f);
	AddAnimation("RagnaLeftJump", false);
	AddAnimation("RagnaLeftLand", false, 0.25f);
	AddAnimation("RagnaLeftAttack5A", false, 0.25f);
	AddAnimation("RagnaLeftAttack5B", false, 0.45f);
	AddAnimation("RagnaLeftAttack5C", false, 0.5f);
	AddAnimation("RagnaLeftAttack5D", false, 1.1f);

	// 애니메이션 Notify
	AddNotify<CPlayer>("RagnaRightWalkFoward", 2, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaRightWalkFoward", 6, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaRightWalkFoward", 1, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaRightWalkBackward", 1, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaRightWalkBackward", 5, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaRightWalkBackward", 7, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftWalkFoward", 2, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaLeftWalkFoward", 6, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaLeftWalkFoward", 7, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftWalkBackward", 1, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaLeftWalkBackward", 5, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaLeftWalkBackward", 7, this, &CPlayer::RagnaSE);

	AddNotify<CPlayer>("RagnaLeftCrouchStart", 0, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaRightCrouchStart", 0, this, &CPlayer::RagnaSE);

	AddNotify<CPlayer>("RagnaRightJump", 1, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaRightJump", 0, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftJump", 1, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaLeftJump", 0, this, &CPlayer::RagnaSE);

	AddNotify<CPlayer>("RagnaRightAttack5A", 1, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaRightAttack5A", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaRightAttack5A", 2, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaRightAttack5A", 3, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaLeftAttack5A", 1, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftAttack5A", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaLeftAttack5A", 2, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaLeftAttack5A", 3, this, &CPlayer::AttackCollisionOff);

	AddNotify<CPlayer>("RagnaRightAttack5B", 4, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaRightAttack5B", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaRightAttack5B", 7, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaRightAttack5B", 12, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaRightAttack5B", 16, this, &CPlayer::WalkSound);
	AddNotify<CPlayer>("RagnaLeftAttack5B", 4, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftAttack5B", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaLeftAttack5B", 7, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaLeftAttack5B", 12, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaLeftAttack5B", 16, this, &CPlayer::WalkSound);

	AddNotify<CPlayer>("RagnaRightAttack5C", 3, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaRightAttack5C", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaRightAttack5C", 6, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaRightAttack5C", 10, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaLeftAttack5C", 3, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftAttack5C", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaLeftAttack5C", 6, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaLeftAttack5C", 10, this, &CPlayer::AttackCollisionOff);

	AddNotify<CPlayer>("RagnaRightAttack5D", 2, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaRightAttack5D", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaRightAttack5D", 5, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaRightAttack5D", 9, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaRightAttack5D", 12, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaRightAttack5D", 14, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaRightAttack5D", 11, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 2, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 0, this, &CPlayer::RagnaVoice);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 5, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 9, this, &CPlayer::AttackCollisionOff);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 11, this, &CPlayer::RagnaSE);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 12, this, &CPlayer::AttackCollision);
	AddNotify<CPlayer>("RagnaLeftAttack5D", 14, this, &CPlayer::AttackCollisionOff);

	// 애니메이션 EndFunction
	SetEndFunction<CPlayer>("RagnaRightAttack5A", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaRightAttack5B", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaRightAttack5C", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaRightAttack5D", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaLeftAttack5A", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaLeftAttack5B", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaLeftAttack5C", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaLeftAttack5D", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("RagnaRightCrouchStart", this, &CPlayer::CrouchStay);
	SetEndFunction<CPlayer>("RagnaLeftCrouchStart", this, &CPlayer::CrouchStay);
	SetEndFunction<CPlayer>("RagnaRightCrouchEnd", this, &CPlayer::CrouchEnd);
	SetEndFunction<CPlayer>("RagnaLeftCrouchEnd", this, &CPlayer::CrouchEnd);
	SetEndFunction<CPlayer>("RagnaRightLand", this, &CPlayer::Land);
	SetEndFunction<CPlayer>("RagnaLeftLand", this, &CPlayer::Land);

	// 애니메이션 배열에 삽입
	m_vecSequenceKey[0].push_back("RagnaRightIdle");
	m_vecSequenceKey[0].push_back("RagnaRightWalkFoward");
	m_vecSequenceKey[0].push_back("RagnaRightWalkBackward");
	m_vecSequenceKey[0].push_back("RagnaRightCrouchStart");
	m_vecSequenceKey[0].push_back("RagnaRightCrouchStay");
	m_vecSequenceKey[0].push_back("RagnaRightCrouchEnd");
	m_vecSequenceKey[0].push_back("RagnaRightJump");
	m_vecSequenceKey[0].push_back("RagnaRightLand");
	m_vecSequenceKey[0].push_back("RagnaRightAttack5A");
	m_vecSequenceKey[0].push_back("RagnaRightAttack5B");
	m_vecSequenceKey[0].push_back("RagnaRightAttack5C");
	m_vecSequenceKey[0].push_back("RagnaRightAttack5D");

	m_vecSequenceKey[1].push_back("RagnaLeftIdle");
	m_vecSequenceKey[1].push_back("RagnaLeftWalkBackward");
	m_vecSequenceKey[1].push_back("RagnaLeftWalkFoward");
	m_vecSequenceKey[1].push_back("RagnaLeftCrouchStart");
	m_vecSequenceKey[1].push_back("RagnaLeftCrouchStay");
	m_vecSequenceKey[1].push_back("RagnaLeftCrouchEnd");
	m_vecSequenceKey[1].push_back("RagnaLeftJump");
	m_vecSequenceKey[1].push_back("RagnaLeftLand");
	m_vecSequenceKey[1].push_back("RagnaLeftAttack5A");
	m_vecSequenceKey[1].push_back("RagnaLeftAttack5B");
	m_vecSequenceKey[1].push_back("RagnaLeftAttack5C");
	m_vecSequenceKey[1].push_back("RagnaLeftAttack5D");
}

void CPlayer::Crouch()
{
	if (!m_Playable || m_Attack)
		return;

	m_CrouchStart = true;
	m_Walk = false;
	m_Idle = false;
}

void CPlayer::CrouchStay()
{
	m_CrouchStart = false;
	m_CrouchStay = true;
	m_Idle = false;
}

void CPlayer::CrouchEnd()
{
	m_CrouchToStand = false;
	m_Idle = true;
}

void CPlayer::MoveFront()
{
	if (!m_Playable)
		return;

	if (!m_CrouchStart && !m_CrouchStay && !m_CrouchToStand && m_Movable && !m_Attack)
	{
		m_Idle = false;
		m_Walk = true;
		m_MoveFront = true;
		m_MoveBack = false;
		MoveDir(Vector2(1.f, 0.f));
	}
}

void CPlayer::MoveBack()
{
	if (!m_Playable)
		return;

	if (!m_CrouchStart && !m_CrouchStay && !m_CrouchToStand && m_Movable && !m_Attack)
	{
		m_Idle = false;
		m_Walk = true;
		m_MoveBack = true;
		m_MoveFront = false;
		MoveDir(Vector2(-1.f, 0.f));
	}
}

void CPlayer::JumpKey()
{
	if (!m_Playable)
		return;

	// 앉아있거나 공격 중일 땐 점프를 할 수 없게 해준다.
	if (m_CrouchStart || m_CrouchStay || m_Attack)
		return;

	if (m_Jumpable)
	{
		if (m_Move.x == 0.f)
			m_Movable = false;
		Jump();
		m_Idle = false;
		m_Land = true;
		m_Jumpable = false;
		m_Walk = false;
	}
}

void CPlayer::JumpKeyUp()
{
	m_Jumpable = true;
}

bool CPlayer::Attack()
{
	if (!m_Playable)
		return false;

	if (m_Attack || m_Jump || m_Land || m_CrouchStart || m_CrouchStay || m_CrouchToStand)
		return false;

	m_Attack = true;
	m_Idle = false;
	m_Movable = false;

	return true;
}

void CPlayer::AttackEnd()
{
	m_Attack = false;
	m_Idle = true;
	m_Movable = true;
}

void CPlayer::AttackA()
{
	if (!Attack())
		return;

	m_AttackLevel = 1;

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	ChangeAnimation(m_vecSequenceKey[AnimDirIndex][8]);
}

void CPlayer::AttackB()
{
	if (!Attack())
		return;

	m_AttackLevel = 2;

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	ChangeAnimation(m_vecSequenceKey[AnimDirIndex][9]);
}

void CPlayer::AttackC()
{
	if (!Attack())
		return;

	m_AttackLevel = 3;

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	ChangeAnimation(m_vecSequenceKey[AnimDirIndex][10]);
}

void CPlayer::AttackD()
{
	if (!Attack())
		return;

	m_AttackLevel = 4;

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	ChangeAnimation(m_vecSequenceKey[AnimDirIndex][11]);

	CEffect* Effect = m_Scene->CreateObject<CEffect>("HitEffectD");

	Effect->SetPivot(0.5f, 0.7f);
	Effect->SetPos(m_Pos);

	if (m_PlayerDir == -1)
		Effect->AddAnimation("LeftHitEffectD", false, 0.9f);
	else
		Effect->AddAnimation("RightHitEffectD", false, 0.9f);
}

void CPlayer::AttackCollision()
{
	CColliderBox* HitBox = AddCollider<CColliderBox>("HitBox");

	// 왼쪽
	if (m_PlayerDir == -1)
	{
		switch (m_AttackLevel)
		{
		case 1:
			HitBox->SetExtent(240.f, 50.f);
			HitBox->SetOffset(-130.f, -185.f);
			break;
		case 2:
			HitBox->SetExtent(300.f, 100.f);
			HitBox->SetOffset(-230.f, -85.f);
			break;
		case 3:
			HitBox->SetExtent(310.f, 300.f);
			HitBox->SetOffset(-350.f, 0.f);
			break;
		case 4:
			if (!m_MultiHit)
			{
				HitBox->SetExtent(270.f, 300.f);
				HitBox->SetOffset(-170.f, -200.f);
			}
			else
			{
				HitBox->SetExtent(450.f, 600.f);
				HitBox->SetOffset(-275.f, -100.f);
			}
			break;
		}
	}
	// 오른쪽
	else
	{
		switch (m_AttackLevel)
		{
		case 1:
			HitBox->SetExtent(240.f, 50.f);
			HitBox->SetOffset(130.f, -185.f);
			break;
		case 2:
			HitBox->SetExtent(300.f, 100.f);
			HitBox->SetOffset(230.f, -85.f);
			break;
		case 3:
			HitBox->SetExtent(310.f, 300.f);
			HitBox->SetOffset(350.f, 0.f);
			break;
		case 4:
			if (!m_MultiHit)
			{
				HitBox->SetExtent(270.f, 300.f);
				HitBox->SetOffset(170.f, -200.f);
			}
			else
			{
				HitBox->SetExtent(450.f, 600.f);
				HitBox->SetOffset(275.f, -100.f);
			}
			break;
		}
	}

	HitBox->SetCollisionProfile("PlayerAttack");
	HitBox->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::AttackCollisionBegin);
	HitBox->SetCollisionEndFunction<CPlayer>(this, &CPlayer::CollisionEnd);
}

void CPlayer::AttackCollisionOff()
{
	CCollider* HitBox = FindCollider("HitBox");
	HitBox->ClearCollisionList();
	HitBox->SetActive(false);

	if(!m_MultiHit)
		m_MultiHit = true;
	else
		m_MultiHit = false;
}

void CPlayer::WalkSound()
{
	m_Scene->GetSceneResource()->SoundPlay("Walk2");
}

void CPlayer::RagnaSE()
{
	if (m_Attack)
	{
		switch (m_AttackLevel)
		{
		case 1:
			m_Scene->GetSceneResource()->SoundPlay("SwingGrap1_0");
			break;
		case 2:
			m_Scene->GetSceneResource()->SoundPlay("SwingGrap1_2");
			break;
		case 3:
			m_Scene->GetSceneResource()->SoundPlay("SwingPole_2");
			break;
		case 4:
			if(!m_MultiHit)
				m_Scene->GetSceneResource()->SoundPlay("SwingGrap2_1");
			else
			{
				m_Scene->GetSceneResource()->SoundPlay("RagnaSE02");
				m_Scene->GetSceneResource()->SoundPlay("SwingGrap2_2");
			}
			break;
		}
	}
	else
		m_Scene->GetSceneResource()->SoundPlay("RagnaSE00");
}

void CPlayer::RagnaVoice()
{
	if (m_Attack)
	{
		srand((unsigned int)time(0));
		int random = rand();
		int select = rand() % 2;
		
		switch (m_AttackLevel)
		{
		case 1:
			if(select == 1)
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceA1");
			else
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceA2");
			break;
		case 2:
			if (select == 1)
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceB1");
			else
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceB2");
			break;
		case 3:
			if (select == 1)
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceC1");
			else
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceC2");
			break;
		case 4:
			if (select == 1)
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceD1");
			else
				m_Scene->GetSceneResource()->SoundPlay("RagnaAttackVoiceD2");
			break;
		}
	}
	else if(m_JumpVoiceReady)
	{
		m_Scene->GetSceneResource()->SoundPlay("RagnaJump");
		m_JumpVoiceReady = false;
	}
}

void CPlayer::MoveStop()
{
	m_Walk = false;
	m_Idle = true;
}

void CPlayer::CrouchToIdle()
{
	if (!m_Playable)
		return;

	m_CrouchStart = false;
	m_CrouchStay = false;
	m_CrouchToStand = true;
}

void CPlayer::Land()
{
	// 완전히 착지한 후에 움직일 수 있다.
	m_Land = false;
	m_Movable = true;
	m_Jumpable = true;
	m_JumpVoiceReady = true;
	m_Idle = true;
}

void CPlayer::Push(CCollider* Src, CCollider* Dest)
{
	if (Dest->GetName() == "Body")
		m_Block = true;
}

void CPlayer::PushEnd(CCollider* Src, CCollider* Dest)
{
	if (Dest->GetName() == "Body")
		m_Block = false;
}

void CPlayer::AttackCollisionBegin(CCollider* Src, CCollider* Dest)
{
	switch (m_AttackLevel)
	{
	case 1:
		m_Scene->GetSceneResource()->SoundPlay("RagnaAttack1");
		Dest->GetOwner()->InflictDamage(300.f);
		break;
	case 2:
		m_Scene->GetSceneResource()->SoundPlay("RagnaAttack2");
		Dest->GetOwner()->InflictDamage(660.f);
		break;
	case 3:
		m_Scene->GetSceneResource()->SoundPlay("RagnaAttack3");
		Dest->GetOwner()->InflictDamage(900.f);
		break;
	case 4:
		m_Scene->GetSceneResource()->SoundPlay("RagnaAttack3");
		if(!m_MultiHit)
			Dest->GetOwner()->InflictDamage(500.f);
		else
			Dest->GetOwner()->InflictDamage(600.f);
		break;
	}
}

void CPlayer::CollisionEnd(CCollider* Src, CCollider* Dest)
{
}
