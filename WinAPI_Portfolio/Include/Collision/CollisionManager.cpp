#include "CollisionManager.h"
#include "ColliderBox.h"
#include "ColliderCircle.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	std::unordered_map<std::string, CollisionProfile*>::iterator iter;
	std::unordered_map<std::string, CollisionProfile*>::iterator iterEnd;

	iter = m_mapProfile.begin();
	iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CCollisionManager::Init()
{
	CreateProfile("Default", ECollision_Channel::Default, true);	// 마지막 인자가 없으므로 모든 채널과 충돌
	CreateProfile("Player", ECollision_Channel::Player, true, ECollision_Interaction::Ignore);
	CreateProfile("Opponent", ECollision_Channel::Opponent, true, ECollision_Interaction::Ignore);
	CreateProfile("PlayerAttack", ECollision_Channel::PlayerAttack, true, ECollision_Interaction::Ignore);
	CreateProfile("OpponentAttack", ECollision_Channel::OpponentAttack, true, ECollision_Interaction::Ignore);
	CreateProfile("Mouse", ECollision_Channel::Mouse, true, ECollision_Interaction::Collision);

	// 플레이어는 기본, 적, 적 공격과만 충돌한다.
	SetCollisionInteraction("Player", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("Player", ECollision_Channel::Opponent, ECollision_Interaction::Collision);
	SetCollisionInteraction("Player", ECollision_Channel::OpponentAttack, ECollision_Interaction::Collision);
	SetCollisionInteraction("Player", ECollision_Channel::Mouse, ECollision_Interaction::Collision);

	// 적은 기본, 플레이어, 플레이어 공격과만 충돌한다.
	SetCollisionInteraction("Opponent", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("Opponent", ECollision_Channel::Player, ECollision_Interaction::Collision);
	SetCollisionInteraction("Opponent", ECollision_Channel::PlayerAttack, ECollision_Interaction::Collision);
	SetCollisionInteraction("Opponent", ECollision_Channel::Mouse, ECollision_Interaction::Collision);

	// 플레이어 공격은 기본, 적과만 충돌한다.
	SetCollisionInteraction("PlayerAttack", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("PlayerAttack", ECollision_Channel::Opponent, ECollision_Interaction::Collision);

	// 적 공격은 기본, 플레이어와만 충돌한다.
	SetCollisionInteraction("OpponentAttack", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("OpponentAttack", ECollision_Channel::Player, ECollision_Interaction::Collision);

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& Name, ECollision_Channel Channel,
	bool Enable, ECollision_Interaction BaseInteraction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new CollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->Enable = Enable;
	Profile->vecCollisionInteraction.resize((size_t)ECollision_Channel::Max);

	for (int i = 0; i < (int)ECollision_Channel::Max; i++)
	{
		Profile->vecCollisionInteraction[i] = BaseInteraction;
	}

	m_mapProfile.insert(std::make_pair(Name, Profile));

	return true;
}

// 충돌체 상호작용 종류를 바꿔주는 함수
bool CCollisionManager::SetCollisionInteraction(const std::string& Name,
	ECollision_Channel Channel, ECollision_Interaction Interaction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	// Channel과 충돌할 때의 상호작용을 Interaction으로 바꿔준다.
	Profile->vecCollisionInteraction[(int)Channel] = Interaction;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	std::unordered_map<std::string, CollisionProfile*>::iterator iter;
	iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}

bool CCollisionManager::CollisionBoxToBox(Vector2& HitPoint, CColliderBox* Src,
	CColliderBox* Dest)
{
	if (CollisionBoxToBox(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		// 맞은 쪽의 히트포인트도 때린 쪽과 똑같이 맞춰준다.(안 하면 엉뚱한데로 나옴)
		Dest->m_HitPoint = HitPoint;
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionCircleToCircle(Vector2& HitPoint, CColliderCircle* Src,
	CColliderCircle* Dest)
{
	if (CollisionCircleToCircle(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		// 맞은 쪽의 히트포인트도 때린 쪽과 똑같이 맞춰준다.(안 하면 엉뚱한데로 나옴)
		Dest->m_HitPoint = HitPoint;
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBoxToCircle(Vector2& HitPoint, CColliderBox* Src,
	CColliderCircle* Dest)
{
	if (CollisionBoxToCircle(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		// 맞은 쪽의 히트포인트도 때린 쪽과 똑같이 맞춰준다.(안 하면 엉뚱한데로 나옴)
		Dest->m_HitPoint = HitPoint;
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPointToBox(Vector2& HitPoint, const Vector2& Src,
	CColliderBox* Dest)
{
	if (CollisionPointToBox(HitPoint, Src, Dest->GetInfo()))
	{
		// 맞은 쪽의 히트포인트도 때린 쪽과 똑같이 맞춰준다.(안 하면 엉뚱한데로 나옴)
		Dest->m_HitPoint = HitPoint;
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPointToCircle(Vector2& HitPoint, const Vector2& Src,
	CColliderCircle* Dest)
{
	if (CollisionPointToCircle(HitPoint, Src, Dest->GetInfo()))
	{
		// 맞은 쪽의 히트포인트도 때린 쪽과 똑같이 맞춰준다.(안 하면 엉뚱한데로 나옴)
		Dest->m_HitPoint = HitPoint;
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBoxToBox(Vector2& HitPoint, const BoxInfo& Src,
	const BoxInfo& Dest)
{
	// 충돌이 되지 않는 경우를 모두 걸러낸다.
	if (Src.LT.x > Dest.RB.x)
		return false;
	else if (Src.LT.y > Dest.RB.y)
		return false;
	else if (Src.RB.x < Dest.LT.x)
		return false;
	else if (Src.RB.y < Dest.LT.y)
		return false;

	// 충돌한 두 사각형의 꼭지점 좌표를 비교해 
	// 겹치는 부분의 사각형의 Left Right Top Bottom 위치를 구한 후
	// 그 사각형의 중심을 HitPoint로 지정한다.
	float Left = Src.LT.x > Dest.LT.x ? Src.LT.x : Dest.LT.x;
	float Top = Src.LT.y > Dest.LT.y ? Src.LT.y : Dest.LT.y;
	float Right = Src.RB.x < Dest.RB.x ? Src.RB.x : Dest.RB.x;
	float Bottom = Src.RB.y < Dest.RB.y ? Src.RB.y : Dest.RB.y;

	HitPoint.x = (Left + Right) / 2.f;
	HitPoint.y = (Top + Bottom) / 2.f;

	return true;
}

bool CCollisionManager::CollisionCircleToCircle(Vector2& HitPoint,
	const CircleInfo& Src, const CircleInfo& Dest)
{
	// 두 원형 충돌체의 중점 사이의 거리를 구한다.
	float Dist = Src.Center.Distance(Dest.Center);

	// 중점 사이의 거리가 두 원형 충돌체의 반지름을 합한 값보다 작으면 충돌한 것이다.
	bool Result = Dist <= Src.Radius + Dest.Radius;

	// 두 중점을 잇는 직선의 가운데를 히트포인트로 지정한다.
	// (정확히 두 충돌체가 겹치는 부분의 중심은 아니지만 정밀하게 계산하려면 복잡하므로)
	HitPoint = (Src.Center + Dest.Center) / 2.f;

	return Result;
}

bool CCollisionManager::CollisionBoxToCircle(Vector2& HitPoint, const BoxInfo& Src,
	const CircleInfo& Dest)
{
	// 1. 원의 중점이 사각형의 좌, 우 영역 혹은 위, 아래 영역에 존재하는지 판단한다.
	if ((Src.LT.x <= Dest.Center.x && Dest.Center.x <= Src.RB.x) ||
		(Src.LT.y <= Dest.Center.y && Dest.Center.y <= Src.RB.y))
	{
		// 사각형을 원의 반지름만큼 확장한다.
		BoxInfo RC = Src;
		RC.LT.x -= Dest.Radius;
		RC.LT.y -= Dest.Radius;
		RC.RB.x += Dest.Radius;
		RC.RB.y += Dest.Radius;

		// 확장된 사각형 안에 원의 중점이 들어온다면 충돌된 것이다.
		// (충돌되지 않는 조건을 찾는게 더 효율적이다)
		if (RC.LT.x > Dest.Center.x)
			return false;
		else if (RC.LT.y > Dest.Center.y)
			return false;
		else if (RC.RB.x < Dest.Center.x)
			return false;
		else if (RC.RB.y < Dest.Center.y)
			return false;

		// 히트포인트를 구한다.
		// (원 충돌체를 내접원으로 갖는 가상의 사각형을 만들어서 
		// 그 사각형과 사각형 충돌체와의 교집합의 중심을 히트포인트로 지정한다)
		BoxInfo CircleBox;
		CircleBox.LT = Dest.Center - Dest.Radius;
		CircleBox.RB = Dest.Center + Dest.Radius;

		float Left = Src.LT.x > CircleBox.LT.x ? Src.LT.x : CircleBox.LT.x;
		float Top = Src.LT.y > CircleBox.LT.y ? Src.LT.y : CircleBox.LT.y;
		float Right = Src.RB.x < CircleBox.RB.x ? Src.RB.x : CircleBox.RB.x;
		float Bottom = Src.RB.y < CircleBox.RB.y ? Src.RB.y : CircleBox.RB.y;

		HitPoint.x = (Left + Right) / 2.f;
		HitPoint.y = (Top + Bottom) / 2.f;

		return true;
	}

	// 2. 원이 확장된 사각형 안에 들어와도 실제로는 사각형 충돌체와 충돌하지 않는 구역이
	// 있기 때문에 그 부분을 확인한다.
	// 사각형 충돌체의 4개의 꼭지점 위치를 구한다.
	Vector2 Pos[4] =
	{
		Src.LT,
		Vector2(Src.RB.x, Src.LT.y),
		Vector2(Src.LT.x, Src.RB.y),
		Src.RB
	};

	// 4개의 점 중 하나라도 원 안에 들어온다면 충돌한 것이다.
	for (int i = 0; i < 4; i++)
	{
		float Dist = Dest.Center.Distance(Pos[i]);

		if (Dist <= Dest.Radius)
		{
			// 히트포인트 계산
			BoxInfo CircleBox;
			CircleBox.LT = Dest.Center - Dest.Radius;
			CircleBox.RB = Dest.Center + Dest.Radius;

			float Left = Src.LT.x > CircleBox.LT.x ? Src.LT.x : CircleBox.LT.x;
			float Top = Src.LT.y > CircleBox.LT.y ? Src.LT.y : CircleBox.LT.y;
			float Right = Src.RB.x < CircleBox.RB.x ? Src.RB.x : CircleBox.RB.x;
			float Bottom = Src.RB.y < CircleBox.RB.y ? Src.RB.y : CircleBox.RB.y;

			HitPoint.x = (Left + Right) / 2.f;
			HitPoint.y = (Top + Bottom) / 2.f;

			return true;
		}
	}

	return false;
}

bool CCollisionManager::CollisionPointToBox(Vector2& HitPoint, const Vector2& Src,
	const BoxInfo& Dest)
{
	// 점이 사각형 안에 들어오지 않았다면 충돌하지 않은 것이다.
	if (Dest.LT.x > Src.x)
		return false;
	else if (Dest.LT.y > Src.y)
		return false;
	else if (Dest.RB.x < Src.x)
		return false;
	else if (Dest.RB.y < Src.y)
		return false;

	// 히트포인트는 점 위치 그대로.
	HitPoint = Src;

	return true;
}

bool CCollisionManager::CollisionPointToCircle(Vector2& HitPoint, const Vector2& Src,
	const CircleInfo& Dest)
{
	// 점과 원의 중점 사이의 거리를 구한다.
	float Dist = Dest.Center.Distance(Src);

	// 거리가 원의 반지름보다 작거나 같으면 충돌한 것이다.
	if (Dist <= Dest.Radius)
	{
		// 히트포인트는 점 위치 그대로.
		HitPoint = Src;

		return true;
	}

	return false;
}
