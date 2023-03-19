#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"
#include "../Scene/Scene.h"
#include "Effect.h"

CBullet::CBullet() :
	m_Damage(0.f)
{
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet& Obj) :
	CGameObject(Obj),
	m_Angle(Obj.m_Angle),
	m_Distance(Obj.m_Distance)
{
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	CGameObject::Init();

	m_MoveSpeed = 700.f;
	m_Angle = 180.f;
	m_Distance = 600.f;

	SetPos(0.f, 0.f);
	SetSize(50.f, 38.f);
	SetPivot(0.5f, 0.5f);

	SetTexture("Bullet", TEXT("Bullet.bmp"));
	SetColorKey(0, 248, 0);

	/*CColliderBox* Box = AddCollider<CColliderBox>("Body");

	Box->SetExtent(50.f, 38.f);

	Box->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
	Box->SetCollisionEndFunction<CBullet>(this, &CBullet::CollisionEnd);*/

	CColliderCircle* Circle = AddCollider<CColliderCircle>("Body");

	Circle->SetRadius(20.f);

	Circle->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
	Circle->SetCollisionEndFunction<CBullet>(this, &CBullet::CollisionEnd);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Move(m_Angle);

	m_Distance -= m_MoveSpeed * DeltaTime;

	// �����Ÿ���ŭ ���ư��� �Ѿ� ����
	if (m_Distance <= 0.f)
		SetActive(false);
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);

	/*Vector2 RenderLT;

	RenderLT = m_Pos - m_Pivot * m_Size;

	Ellipse(hDC, (int)RenderLT.x, (int)RenderLT.y,
		(int)(RenderLT.x + m_Size.x), (int)(RenderLT.y + m_Size.y));*/
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	SetActive(false);

	CEffect* Effect = m_Scene->CreateObject<CEffect>("HitEffect");

	Effect->SetPivot(0.5f, 0.5f);
	Effect->SetPos(Src->GetHitPoint());

	Effect->AddAnimation("LeftHitEffect", false, 0.3f);

	// Damage ó��
	// �Ѿ��� ���� ���� InflictDamage�Լ��� ������� �����Ѵ�.
	// (���� �Ѿ��� �¾Ҵ��� ������ ���� �ʿ� ����)
	Dest->GetOwner()->InflictDamage(m_Damage);
}

void CBullet::CollisionEnd(CCollider* Src, CCollider* Dest)
{
}
