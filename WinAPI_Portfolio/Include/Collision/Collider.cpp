#include "Collider.h"
#include "CollisionManager.h"

CCollider::CCollider() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Profile(nullptr),
	m_MouseCollison(false),
	m_Bottom(0.f)
{
}

CCollider::CCollider(const CCollider& collider) :
	CRef(collider),
	m_ColliderType(collider.m_ColliderType),
	m_Offset(collider.m_Offset),
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Profile(collider.m_Profile),
	m_MouseCollison(false),
	m_Bottom(0.f)
{
}

CCollider::~CCollider()
{
	std::list<CSharedPtr<CCollider>>::iterator iter;
	std::list<CSharedPtr<CCollider>>::iterator iterEnd;

	iter = m_CollisionList.begin();
	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		// 나와 상대가 충돌해서 내가 없어졌다면
		// 상대가 갖고 있는 충돌체 목록에서도 나를 없애줘야 한다.
		(*iter)->DeleteCollisionList(this);
	}
}

void CCollider::SetCollisionProfile(const std::string& Name)
{
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CCollider::AddCollisionList(CCollider* Collider)
{
	m_CollisionList.push_back(Collider);
}

bool CCollider::CheckCollisionList(CCollider* Collider)
{
	std::list<CSharedPtr<CCollider>>::iterator iter;
	std::list<CSharedPtr<CCollider>>::iterator iterEnd;

	iter = m_CollisionList.begin();
	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

void CCollider::DeleteCollisionList(CCollider* Collider)
{
	std::list<CSharedPtr<CCollider>>::iterator iter;
	std::list<CSharedPtr<CCollider>>::iterator iterEnd;

	iter = m_CollisionList.begin();
	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
		{
			m_CollisionList.erase(iter);
			return;
		}
	}
}

void CCollider::ClearCollisionList()
{
	std::list<CSharedPtr<CCollider>>::iterator iter;
	std::list<CSharedPtr<CCollider>>::iterator iterEnd;

	iter = m_CollisionList.begin();
	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		// 내 충돌체 정보를 갖고 있는 모든 대상에게서 자신을 지운다.
		(*iter)->DeleteCollisionList(this);
	}

	m_CollisionList.clear();
}

void CCollider::CallCollisionBegin(CCollider* Dest)
{
	if (m_CollisionBegin)
		m_CollisionBegin(this, Dest);
}

void CCollider::CallCollisionEnd(CCollider* Dest)
{
	if (m_CollisionEnd)
		m_CollisionEnd(this, Dest);
}

void CCollider::CallMouseCollisionBegin(const Vector2& MousePos)
{
	if (m_MouseCollisionBegin)
		m_MouseCollisionBegin(this, MousePos);
}

void CCollider::CallMouseCollisionEnd(const Vector2& MousePos)
{
	if (m_MouseCollisionEnd)
		m_MouseCollisionEnd(this, MousePos);
}

bool CCollider::Init()
{
	// 충돌체를 처음 만들면 일단 기본 충돌 채널을 갖고 있게 한다.
	m_Profile = CCollisionManager::GetInst()->FindProfile("Default");

	return true;
}

void CCollider::Update(float DeltaTime)
{
}

void CCollider::PostUpdate(float DeltaTime)
{
}

void CCollider::Render(HDC hDC, float DeltaTime)
{
}

bool CCollider::Collision(CCollider* Dest)
{
	return false;
}

bool CCollider::CollisionMouse(const Vector2& Mouse)
{
	return false;
}
