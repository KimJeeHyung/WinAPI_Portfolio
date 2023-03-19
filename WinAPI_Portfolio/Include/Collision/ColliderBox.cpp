#include "ColliderBox.h"
#include "../GameObject/GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "CollisionManager.h"
#include "ColliderCircle.h"

CColliderBox::CColliderBox() :
	m_Width(100.f),
	m_Height(100.f)
{
	SetTypeID<CColliderBox>();

	m_ColliderType = ECollider_Type::Box;
}

CColliderBox::CColliderBox(const CColliderBox& collider) :
	CCollider(collider),
	m_Width(collider.m_Width),
	m_Height(collider.m_Height),
	m_Info(collider.m_Info)
{
}

CColliderBox::~CColliderBox()
{
}

bool CColliderBox::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderBox::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderBox::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	// Owner의 위치와 Offset을 이용해 충돌체의 위치를 구한다.
	Vector2 Pos = m_Owner->GetPos();
	Vector2 Size(m_Width, m_Height);

	m_Info.LT = Pos + m_Offset - Size / 2.f;
	m_Info.RB = Pos + m_Offset + Size / 2.f;

	m_Bottom = m_Info.RB.y;
}

void CColliderBox::Render(HDC hDC, float DeltaTime)
{
	// 디버그 모드 시에만 충돌체가 보이도록 만들어준다.
#ifdef _DEBUG

	// 충돌체가 무언가와 충돌하지 않을 때는 초록색, 충돌하면 빨간색으로 표시한다.
	HBRUSH Brush = CGameManager::GetInst()->GetBrush(EBrush_Type::Green);

	if(!m_CollisionList.empty() || m_MouseCollison)
		Brush = CGameManager::GetInst()->GetBrush(EBrush_Type::Red);

	// 카메라의 위치를 고려해 충돌체가 윈도우에 그려질 위치를 지정해준다.
	CCamera* Camera = m_Scene->GetCamera();

	RECT RenderRC;

	RenderRC.left = (long)(m_Info.LT.x - Camera->GetPos().x);
	RenderRC.top = (long)(m_Info.LT.y - Camera->GetPos().y);
	RenderRC.right = (long)(m_Info.RB.x - Camera->GetPos().x);
	RenderRC.bottom = (long)(m_Info.RB.y - Camera->GetPos().y);

	FrameRect(hDC, &RenderRC, Brush);

#endif // _DEBUG
}

bool CColliderBox::Collision(CCollider* Dest)
{
	// 상대의 충돌체가 박스 모양인지 원 모양인지에 따라 충돌 확인이 달라진다.
	switch (Dest->GetColliderType())
	{
	case ECollider_Type::Box:
		return CCollisionManager::GetInst()->CollisionBoxToBox(m_HitPoint, this, (CColliderBox*)Dest);
	case ECollider_Type::Circle:
		return CCollisionManager::GetInst()->CollisionBoxToCircle(m_HitPoint, this, (CColliderCircle*)Dest);
	}

	return false;
}

bool CColliderBox::CollisionMouse(const Vector2& Mouse)
{
	return CCollisionManager::GetInst()->CollisionPointToBox(m_HitPoint, Mouse, m_Info);
}
