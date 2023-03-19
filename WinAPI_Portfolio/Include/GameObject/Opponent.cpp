#include "Opponent.h"
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

COpponent::COpponent() :
	m_Idle(true),
	m_CrouchStart(false),
	m_CrouchStay(false),
	m_CrouchToStand(false),
	m_Movable(true),
	m_Jumpable(true),
	m_JumpVoiceReady(true),
	m_Block(false),
	m_Body(nullptr)
{
	SetTypeID<COpponent>();
}

COpponent::COpponent(const COpponent& Obj) :
	CCharacter(Obj),
	m_CrouchStart(false),
	m_CrouchStay(false),
	m_CrouchToStand(false)
{
}

COpponent::~COpponent()
{
}

bool COpponent::Init()
{
	CGameObject::Init();

	SetPos(1800.f, 900.f);
	SetSize(100.f, 100.f);
	SetPivot(0.5f, 0.7f);
	m_MoveSpeed = 600.f;

	InitAnimation();

	// ������ ���� ����.
	m_PlayerDir = 1;
	m_PrevDir = m_PlayerDir;

	// ���� ���� �ƴ� ��.
	m_Attack = false;

	// �浹ü �߰�

	m_Body = AddCollider<CColliderBox>("Body");
	m_Body->SetExtent(125.f, 350.f);
	m_Body->SetOffset(0.f, -40.f);
	m_Body->SetCollisionProfile("Opponent");

	m_Body->SetCollisionBeginFunction<COpponent>(this, &COpponent::Push);
	m_Body->SetCollisionEndFunction<COpponent>(this, &COpponent::PushEnd);

	// Input���� ������ BindKey�� Player�� �̵� �Լ����� ������ش�.
	CInput::GetInst()->AddBindFunction<COpponent>("MoveUp2", Input_Type::Push,
		this, &COpponent::JumpKey);
	CInput::GetInst()->AddBindFunction<COpponent>("MoveUp2", Input_Type::Up,
		this, &COpponent::JumpKeyUp);

	CInput::GetInst()->AddBindFunction<COpponent>("MoveDown2", Input_Type::Push,
		this, &COpponent::Crouch);
	CInput::GetInst()->AddBindFunction<COpponent>("MoveDown2", Input_Type::Up,
		this, &COpponent::CrouchToIdle);

	CInput::GetInst()->AddBindFunction<COpponent>("MoveRight2", Input_Type::Push,
		this, &COpponent::GunRotation);
	CInput::GetInst()->AddBindFunction<COpponent>("MoveRight2", Input_Type::Up,
		this, &COpponent::MoveStop);

	CInput::GetInst()->AddBindFunction<COpponent>("MoveLeft2", Input_Type::Push,
		this, &COpponent::GunRotationInv);
	CInput::GetInst()->AddBindFunction<COpponent>("MoveLeft2", Input_Type::Up,
		this, &COpponent::MoveStop);

	/*CInput::GetInst()->AddBindFunction<COpponent>("Fire", Input_Type::Down,
		this, &COpponent::Attack);*/

	CInput::GetInst()->AddBindFunction<COpponent>("Jump", Input_Type::Push,
		this, &COpponent::JumpKey);
	CInput::GetInst()->AddBindFunction<COpponent>("Jump", Input_Type::Up,
		this, &COpponent::JumpKeyUp);

	m_HP = 11000;
	m_HPMax = 11000;

	// �÷��̾ �߷��� ������ �ް� �Ѵ�.
	SetPhysicsSimulate(true);
	// ���� �ӵ�
	SetJumpVelocity(80.f);
	SetGravityAccel(GRAVITY * 2.7f);
	// �� �浹 Ȱ��ȭ
	SetSideWallCheck(true);

	return true;
}

void COpponent::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	DeltaTime *= m_TimeScale;

	// ���� ������� ��� 
	if (m_Block && !m_Jump)
	{
		// �ڱⰡ ���� �������� ��븦 �о��.
		m_Opponent->MoveDir(Vector2((float)m_PlayerDir, 0.f));

		// ��븦 �հ� ������ �� ���� ���ش�.
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

	// ī�޶� ������ ����� �ʵ��� ���ش�.
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

// ��Ȯ�� �̵��� ����� ���� PostUpdate�� �ۼ��Ѵ�.
void COpponent::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	// ��� ĳ���Ͱ� �÷��̾�� �����ʿ� ������ �÷��̾�� �������� �ٶ󺸰�
	// ���ʿ� ������ ������ �ٶ󺻴�.
	if (m_Opponent->GetPos().x > m_Pos.x)
		m_PlayerDir = 1;
	else
		m_PlayerDir = -1;

	int AnimDirIndex = 0;

	//  �÷��̾ ������ ���� ���� ��� 1�� �ε���(������ ���� �ִϸ��̼�) ���
	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	if (m_Walk && !m_Jump && !m_Land && !m_Attack)
	{
		// �÷��̾ ���� ������ �������� ������ �� ���� ������,
		// �ڷ� �� ���� �ڷ� �ȴ� �ִϸ��̼� ���
		if (m_MoveFront)
			ChangeAnimation(m_vecSequenceKey[AnimDirIndex][1]);
		else if (m_MoveBack)
			ChangeAnimation(m_vecSequenceKey[AnimDirIndex][2]);
	}
	// �÷��̾ �� �ڼ����� ���� �ڼ��� �ٲ�� ���� ��
	else if (m_CrouchStart && !m_CrouchStay && !m_CrouchToStand && !m_Jump && !m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][3]);
	}
	// ���� �ڼ��� ���� ���� ��
	else if (m_CrouchStay && !m_CrouchToStand && !m_Jump && !m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][4]);

		m_Body->SetExtent(125.f, 230.f);
		m_Body->SetOffset(-20.f, 20.f);
	}
	// ���� �ڼ����� �� �ڼ��� �ٲ�� ���� ��
	else if (m_CrouchToStand && !m_Jump && !m_Land && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][5]);
	}
	// ���� ��
	else if (m_Jump && m_Land && !m_Attack)
	{
		if (m_Movable)
		{
			if (m_Move.x > 0.f)
				ChangeAnimation(m_vecSequenceKey[AnimDirIndex][6]);
			else if (m_Move.x < 0.f)
				ChangeAnimation(m_vecSequenceKey[AnimDirIndex][7]);
		}
		else
			ChangeAnimation(m_vecSequenceKey[AnimDirIndex][8]);

		if (m_JumpCount == 2)
		{
			// 2�� ���� �� ���� ī��Ʈ�� 1 ���� �� �����Ӹ��� �ִϸ��̼��� �ʱ�ȭ���� �ʵ��� ���ش�.
			m_JumpCount++;
			SetCurrentAniamtion(m_vecSequenceKey[AnimDirIndex][8]);
		}

		m_Body->SetExtent(125.f, 325.f);
		m_Body->SetOffset(0.f, -175.f);
	}
	// ���� �� ���� ���� ��
	else if (m_Land && !m_Attack)
	{
		// ���� �߿��� �Ͻ������� ������ �� ����.
		m_Movable = false;
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][9]);

		m_Body->SetExtent(125.f, 350.f);
		m_Body->SetOffset(0.f, -40.f);
	}
	// ���� ���� � ���µ� �ƴ� ���� �������� ���� �� Idle�� ���ư���.
	else if (m_Idle && !m_Attack)
	{
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][0]);

		m_Body->SetExtent(125.f, 350.f);
		m_Body->SetOffset(0.f, -40.f);
	}
}

void COpponent::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);

	// ����� ���� ������ ���� ������ �����Ѵ�.
	m_PrevDir = m_PlayerDir;
}

float COpponent::InflictDamage(float Damage)
{
	Damage = CCharacter::InflictDamage(Damage);

	m_HP -= (int)Damage;

	if (m_HP <= 0)
	{
		m_Scene->FindWidget<CCharacterHUD>("CharacterHUD")->FinishSequence();
	}

	m_Scene->FindWidget<CCharacterHUD>("CharacterHUD")->SetHP(m_HP / (float)m_HPMax, false);

	if(m_HP <= m_HPMax * 0.35f)
		m_Scene->FindWidget<CCharacterHUD>("CharacterHUD")->SetHPBarColor(3, false);
	else if (m_HP < m_HPMax)
		m_Scene->FindWidget<CCharacterHUD>("CharacterHUD")->SetHPBarColor(2, false);

	return Damage;
}

void COpponent::InitAnimation()
{
	CreateAnimation();

	AddAnimation("NoelRightIdle", true, 0.95f);
	AddAnimation("NoelRightWalkFoward", true, 0.8f);
	AddAnimation("NoelRightWalkBackward", true, 0.8f);
	AddAnimation("NoelRightCrouchStart", false, 0.15f);
	AddAnimation("NoelRightCrouchStay", true, 1.05f);
	AddAnimation("NoelRightCrouchEnd", false, 0.15f);
	AddAnimation("NoelRightJump", false);
	AddAnimation("NoelRightFrontJump", false);
	AddAnimation("NoelRightBackJump", false);
	AddAnimation("NoelRightLand", false, 0.25f);
	AddAnimation("RagnaRightAttack", false, 0.3f);	// ���� X

	AddAnimation("NoelLeftIdle", true, 0.95f);
	AddAnimation("NoelLeftWalkFoward", true, 0.8f);
	AddAnimation("NoelLeftWalkBackward", true, 0.8f);
	AddAnimation("NoelLeftCrouchStart", false, 0.15f);
	AddAnimation("NoelLeftCrouchStay", true, 1.05f);
	AddAnimation("NoelLeftCrouchEnd", false, 0.15f);
	AddAnimation("NoelLeftJump", false);
	AddAnimation("NoelLeftFrontJump", false);
	AddAnimation("NoelLeftBackJump", false);
	AddAnimation("NoelLeftLand", false, 0.25f);
	AddAnimation("RagnaLeftAttack", false, 0.3f);	// ���� X

	SetEndFunction<COpponent>("RagnaRightAttack", this, &COpponent::AttackEnd);
	SetEndFunction<COpponent>("RagnaLeftAttack", this, &COpponent::AttackEnd);
	SetEndFunction<COpponent>("NoelRightCrouchStart", this, &COpponent::CrouchStay);
	SetEndFunction<COpponent>("NoelLeftCrouchStart", this, &COpponent::CrouchStay);
	SetEndFunction<COpponent>("NoelRightCrouchEnd", this, &COpponent::CrouchEnd);
	SetEndFunction<COpponent>("NoelLeftCrouchEnd", this, &COpponent::CrouchEnd);
	SetEndFunction<COpponent>("NoelRightLand", this, &COpponent::Land);
	SetEndFunction<COpponent>("NoelLeftLand", this, &COpponent::Land);

	// ���� �ִϸ��̼��� ������ �����ӿ��� �Ѿ��� ��������
	// ��Ƽ���̸� �߰��Ѵ�.
	AddNotify<COpponent>("RagnaRightAttack", 2, this, &COpponent::Attack);
	AddNotify<COpponent>("RagnaLeftAttack", 2, this, &COpponent::Attack);

	AddNotify<COpponent>("NoelRightWalkFoward", 3, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelRightWalkFoward", 9, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelRightWalkBackward", 2, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelRightWalkBackward", 7, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelLeftWalkFoward", 3, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelLeftWalkFoward", 9, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelLeftWalkBackward", 2, this, &COpponent::WalkSound);
	AddNotify<COpponent>("NoelLeftWalkBackward", 7, this, &COpponent::WalkSound);

	AddNotify<COpponent>("NoelLeftJump", 0, this, &COpponent::WalkSound);

	m_vecSequenceKey[0].push_back("NoelRightIdle");
	m_vecSequenceKey[0].push_back("NoelRightWalkFoward");
	m_vecSequenceKey[0].push_back("NoelRightWalkBackward");
	m_vecSequenceKey[0].push_back("NoelRightCrouchStart");
	m_vecSequenceKey[0].push_back("NoelRightCrouchStay");
	m_vecSequenceKey[0].push_back("NoelRightCrouchEnd");
	m_vecSequenceKey[0].push_back("NoelRightFrontJump");
	m_vecSequenceKey[0].push_back("NoelRightBackJump");
	m_vecSequenceKey[0].push_back("NoelRightJump");
	m_vecSequenceKey[0].push_back("NoelRightLand");
	m_vecSequenceKey[0].push_back("RagnaRightAttack");

	m_vecSequenceKey[1].push_back("NoelLeftIdle");
	m_vecSequenceKey[1].push_back("NoelLeftWalkBackward");
	m_vecSequenceKey[1].push_back("NoelLeftWalkFoward");
	m_vecSequenceKey[1].push_back("NoelLeftCrouchStart");
	m_vecSequenceKey[1].push_back("NoelLeftCrouchStay");
	m_vecSequenceKey[1].push_back("NoelLeftCrouchEnd");
	m_vecSequenceKey[1].push_back("NoelLeftBackJump");
	m_vecSequenceKey[1].push_back("NoelLeftFrontJump");
	m_vecSequenceKey[1].push_back("NoelLeftJump");
	m_vecSequenceKey[1].push_back("NoelLeftLand");
	m_vecSequenceKey[1].push_back("RagnaLeftAttack");
}

void COpponent::MoveFront()
{
}

void COpponent::Crouch()
{
	if (!m_Playable || m_Attack)
		return;

	m_CrouchStart = true;
	m_Walk = false;
	m_Idle = false;
}

void COpponent::CrouchStay()
{
	m_CrouchStart = false;
	m_CrouchStay = true;
	m_Idle = false;
}

void COpponent::CrouchEnd()
{
	m_CrouchToStand = false;
	m_Idle = true;
}

void COpponent::GunRotation()
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

void COpponent::GunRotationInv()
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

void COpponent::JumpKey()
{
	if (!m_Playable)
		return;

	// �ɾ��ְų� ���� ���� �� ������ �� �� ���� ���ش�.
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

void COpponent::JumpKeyUp()
{
	m_Jumpable = true;
}

void COpponent::AttackEnd()
{
	m_Attack = false;
	m_Idle = true;
	m_Movable = true;
}

void COpponent::Attack()
{
	if (!m_Playable)
		return;

	if (m_Attack)
		return;

	m_Attack = true;

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	ChangeAnimation(m_vecSequenceKey[AnimDirIndex][2]);
}

void COpponent::WalkSound()
{
	if (m_Jump)
	{
		if (m_JumpVoiceReady)
		{
			m_Scene->GetSceneResource()->SoundPlay("Walk2");
			m_JumpVoiceReady = false;
		}
	}
	else
		m_Scene->GetSceneResource()->SoundPlay("Walk2");
}

void COpponent::ReturnToRightIdle()
{
	ChangeAnimation(m_vecSequenceKey[1][0]);
}

void COpponent::ReturnToLeftIdle()
{
	ChangeAnimation(m_vecSequenceKey[0][0]);
}

void COpponent::MoveStop()
{
	m_Walk = false;
	m_Idle = true;
}

void COpponent::CrouchToIdle()
{
	if (!m_Playable)
		return;

	m_CrouchStart = false;
	m_CrouchStay = false;
	m_CrouchToStand = true;
}

void COpponent::Land()
{
	// ������ ������ �Ŀ� ������ �� �ִ�.
	m_Land = false;
	m_Movable = true;
	m_Jumpable = true;
	m_JumpVoiceReady = true;
	m_Idle = true;
}

void COpponent::Push(CCollider* Src, CCollider* Dest)
{
	/*if (m_Idle)
		return;*/

	if (Dest->GetName() == "Body")
		m_Block = true;
}

void COpponent::PushEnd(CCollider* Src, CCollider* Dest)
{
	if (Dest->GetName() == "Body")
		m_Block = false;
}

void COpponent::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	//m_Scene->GetSceneResource()->SoundPlay("PlayerHit");
}

void COpponent::CollisionEnd(CCollider* Src, CCollider* Dest)
{
}
