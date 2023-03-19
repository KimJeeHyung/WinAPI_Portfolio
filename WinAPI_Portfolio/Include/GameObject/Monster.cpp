#include "Monster.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"
#include "Effect.h"

CMonster::CMonster() :
	m_HP(100)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Obj) :
	CCharacter(Obj),
	m_Dir(Obj.m_Dir),
	m_FireTime(Obj.m_FireTime),
	m_FireCount(Obj.m_FireCount)
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	CGameObject::Init();

	SetPos(1000.f, 500.f);
	SetSize(90.f, 90.f);
	SetPivot(0.5f, 0.5f);

	m_MoveSpeed = 300.f;
	m_Dir = Vector2(1.f, 0.f);
	m_FireTime = 0.f;
	m_FireCount = 0;

	SetTexture("Monster", TEXT("teemo.bmp"));
	SetColorKey(255, 0, 255);

	/*CColliderBox* Box = AddCollider<CColliderBox>("Body");

	Box->SetExtent(100.f, 100.f);
	Box->SetCollisionProfile("Monster");

	Box->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionBegin);
	Box->SetCollisionEndFunction<CMonster>(this, &CMonster::CollisionEnd);*/

	CColliderCircle* Circle = AddCollider<CColliderCircle>("Body");

	Circle->SetRadius(50.f);
	Circle->SetCollisionProfile("Monster");

	Circle->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionBegin);
	Circle->SetCollisionEndFunction<CMonster>(this, &CMonster::CollisionEnd);

	Circle->SetMouseCollisionBeginFunction<CMonster>(this, &CMonster::CollisionMouseBegin);
	Circle->SetMouseCollisionEndFunction<CMonster>(this, &CMonster::CollisionMouseEnd);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	//MoveDir(m_Dir);

	// �Ǻ��� y���� 0.5�� �ƴ� ��� �Ǻ��� �������� ���� �Ʒ��� ���̰� �ٸ��� ������
	// �ٴڿ� ��Ҵ��� �Ǵ��� ���� (1 - �Ǻ�y)������ ����Ѵ�.
	//if (m_Pos.x > 1000.f)
	//{
	//	// ��ġ�� ȭ���� ��� ��� ȭ�� ��迡�� �����ϰԲ� y���� ����ش�.
	//	m_Pos.x = 999.f;
	//	// ���� ��ȯ
	//	m_Dir *= -1;
	//}
	//if (m_Pos.x < 300.f)
	//{
	//	// ��ġ�� ȭ���� ��� ��� ȭ�� ��迡�� �����ϰԲ� y���� ����ش�.
	//	m_Pos.x = 301.f;
	//	// ���� ��ȯ
	//	m_Dir *= -1;
	//}

	m_FireTime += DeltaTime;

	if (m_FireTime >= 0.5f)
	{
		m_FireTime -= 0.5f;

		//++m_FireCount;

		//CBullet* Bullet = m_Scene->CreateObject<CBullet>("Bullet");

		//// ���Ͱ� ��� �Ѿ��� MonsterAttack Ÿ���� �浹ü�� �����ش�.
		//CCollider* BulletCol = Bullet->FindCollider("Body");

		//BulletCol->SetCollisionProfile("MonsterAttack");

		//// �Ѿ��� ������ ��ġ�� x��ǥ
		//float BulletX = m_Pos.x - m_Pivot.x * m_Size.x -
		//	(1.f - Bullet->GetPivot().x) * Bullet->GetSize().x;

		//Bullet->SetPos(BulletX, m_Pos.y);
		//Bullet->SetDamage(10.f);

		//// 3��°�� �߻��ϴ� �Ѿ��� �÷��̾� �������� ������ �Ѵ�.
		//if (m_FireCount == 3)
		//{
		//	m_FireCount = 0;

		//	// �÷��̾� �������� ���� ������ ���Ѵ�.
		//	float Angle = Bullet->GetPos().Angle(m_Scene->GetPlayer()->GetPos());
		//	Bullet->SetAngle(Angle);
		//}
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);

	/*Vector2 RenderLT;

	RenderLT = m_Pos - m_Pivot * m_Size;

	Rectangle(hDC, (int)RenderLT.x, (int)RenderLT.y,
		(int)(RenderLT.x + m_Size.x), (int)(RenderLT.y + m_Size.y));*/
}

float CMonster::InflictDamage(float Damage)
{
	Damage = CCharacter::InflictDamage(Damage);

	m_HP -= (int)Damage;

	if (m_HP <= 0)
	{
		SetActive(false);
	}

	return Damage;
}

void CMonster::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	m_Scene->GetSceneResource()->SoundPlay("TeemoHit");
	//MessageBox(nullptr, TEXT("�׾��!!"), TEXT("^��^"), MB_OK);
}

void CMonster::CollisionEnd(CCollider* Src, CCollider* Dest)
{
	//MessageBox(nullptr, TEXT("����"), TEXT("^��^"), MB_OK);
}

void CMonster::CollisionMouseBegin(CCollider* Src, const Vector2& MousePos)
{
	/*CEffect* Effect = m_Scene->CreateObject<CEffect>("HitEffect");

	Effect->SetPivot(0.5f, 0.5f);
	Effect->SetPos(MousePos);

	Effect->AddAnimation("LeftHitEffect", false, 0.3f);*/
}

void CMonster::CollisionMouseEnd(CCollider* Src, const Vector2& MousePos)
{
}
