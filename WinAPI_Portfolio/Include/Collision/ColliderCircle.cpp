#include "ColliderCircle.h"
#include "../GameObject/GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "CollisionManager.h"
#include "ColliderBox.h"

CColliderCircle::CColliderCircle()
{
	SetTypeID<CColliderCircle>();

	m_ColliderType = ECollider_Type::Circle;

	m_Info.Radius = 30.f;
}

CColliderCircle::CColliderCircle(const CColliderCircle& collider) :
	CCollider(collider),
	m_Info(collider.m_Info)
{
}

CColliderCircle::~CColliderCircle()
{
}

bool CColliderCircle::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderCircle::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderCircle::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	// Owner의 위치와 Offset을 이용해 충돌체의 중점을 구한다.
	Vector2 Pos = m_Owner->GetPos();
	m_Info.Center = Pos + m_Offset;

	m_Bottom = m_Info.Center.y + m_Info.Radius;
}

void CColliderCircle::Render(HDC hDC, float DeltaTime)
{
	// 디버그 모드 시에만 충돌체가 보이도록 만들어준다.
#ifdef _DEBUG

	// 충돌체가 무언가와 충돌하지 않을 때는 초록색, 충돌하면 빨간색으로 표시한다.
	HPEN Pen = CGameManager::GetInst()->GetPen(EBrush_Type::Green);

	if (!m_CollisionList.empty() || m_MouseCollison)
		Pen = CGameManager::GetInst()->GetPen(EBrush_Type::Red);

	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);

	// 카메라의 위치를 고려해 충돌체가 윈도우에 그려질 위치를 지정해준다.
	CCamera* Camera = m_Scene->GetCamera();

	Vector2 Pos = m_Info.Center - Camera->GetPos();

	MoveToEx(hDC, (long)(Pos.x + m_Info.Radius), (long)Pos.y, nullptr);

	// 직선을 이용해 원을 그릴 때 너무 정밀하게 그리면 성능을 잡아먹기 때문에
	// 적당히 그려준다.
	for (int i = 12; i <= 360; i += 12)
	{
		Vector2 Target;
		Target.x = Pos.x + cosf(DegreeToRadian((float)i)) * m_Info.Radius;
		Target.y = Pos.y + sinf(DegreeToRadian((float)i)) * m_Info.Radius;

		LineTo(hDC, (long)Target.x, (long)Target.y);
	}

	SelectObject(hDC, PrevPen);

#endif // _DEBUG
}

bool CColliderCircle::Collision(CCollider* Dest)
{
	// 상대의 충돌체가 박스 모양인지 원 모양인지에 따라 충돌 확인이 달라진다.
	switch (Dest->GetColliderType())
	{
	case ECollider_Type::Box:
		return CCollisionManager::GetInst()->CollisionBoxToCircle(m_HitPoint, (CColliderBox*)Dest, this);
	case ECollider_Type::Circle:
		return CCollisionManager::GetInst()->CollisionCircleToCircle(m_HitPoint, this, (CColliderCircle*)Dest);
	}

	return false;
}

bool CColliderCircle::CollisionMouse(const Vector2& Mouse)
{
	return CCollisionManager::GetInst()->CollisionPointToCircle(m_HitPoint, Mouse, m_Info);
}
