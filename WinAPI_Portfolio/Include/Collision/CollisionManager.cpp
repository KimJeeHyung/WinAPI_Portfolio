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
	CreateProfile("Default", ECollision_Channel::Default, true);	// ������ ���ڰ� �����Ƿ� ��� ä�ΰ� �浹
	CreateProfile("Player", ECollision_Channel::Player, true, ECollision_Interaction::Ignore);
	CreateProfile("Opponent", ECollision_Channel::Opponent, true, ECollision_Interaction::Ignore);
	CreateProfile("PlayerAttack", ECollision_Channel::PlayerAttack, true, ECollision_Interaction::Ignore);
	CreateProfile("OpponentAttack", ECollision_Channel::OpponentAttack, true, ECollision_Interaction::Ignore);
	CreateProfile("Mouse", ECollision_Channel::Mouse, true, ECollision_Interaction::Collision);

	// �÷��̾�� �⺻, ��, �� ���ݰ��� �浹�Ѵ�.
	SetCollisionInteraction("Player", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("Player", ECollision_Channel::Opponent, ECollision_Interaction::Collision);
	SetCollisionInteraction("Player", ECollision_Channel::OpponentAttack, ECollision_Interaction::Collision);
	SetCollisionInteraction("Player", ECollision_Channel::Mouse, ECollision_Interaction::Collision);

	// ���� �⺻, �÷��̾�, �÷��̾� ���ݰ��� �浹�Ѵ�.
	SetCollisionInteraction("Opponent", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("Opponent", ECollision_Channel::Player, ECollision_Interaction::Collision);
	SetCollisionInteraction("Opponent", ECollision_Channel::PlayerAttack, ECollision_Interaction::Collision);
	SetCollisionInteraction("Opponent", ECollision_Channel::Mouse, ECollision_Interaction::Collision);

	// �÷��̾� ������ �⺻, ������ �浹�Ѵ�.
	SetCollisionInteraction("PlayerAttack", ECollision_Channel::Default, ECollision_Interaction::Collision);
	SetCollisionInteraction("PlayerAttack", ECollision_Channel::Opponent, ECollision_Interaction::Collision);

	// �� ������ �⺻, �÷��̾�͸� �浹�Ѵ�.
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

// �浹ü ��ȣ�ۿ� ������ �ٲ��ִ� �Լ�
bool CCollisionManager::SetCollisionInteraction(const std::string& Name,
	ECollision_Channel Channel, ECollision_Interaction Interaction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	// Channel�� �浹�� ���� ��ȣ�ۿ��� Interaction���� �ٲ��ش�.
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
		// ���� ���� ��Ʈ����Ʈ�� ���� �ʰ� �Ȱ��� �����ش�.(�� �ϸ� �����ѵ��� ����)
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
		// ���� ���� ��Ʈ����Ʈ�� ���� �ʰ� �Ȱ��� �����ش�.(�� �ϸ� �����ѵ��� ����)
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
		// ���� ���� ��Ʈ����Ʈ�� ���� �ʰ� �Ȱ��� �����ش�.(�� �ϸ� �����ѵ��� ����)
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
		// ���� ���� ��Ʈ����Ʈ�� ���� �ʰ� �Ȱ��� �����ش�.(�� �ϸ� �����ѵ��� ����)
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
		// ���� ���� ��Ʈ����Ʈ�� ���� �ʰ� �Ȱ��� �����ش�.(�� �ϸ� �����ѵ��� ����)
		Dest->m_HitPoint = HitPoint;
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBoxToBox(Vector2& HitPoint, const BoxInfo& Src,
	const BoxInfo& Dest)
{
	// �浹�� ���� �ʴ� ��츦 ��� �ɷ�����.
	if (Src.LT.x > Dest.RB.x)
		return false;
	else if (Src.LT.y > Dest.RB.y)
		return false;
	else if (Src.RB.x < Dest.LT.x)
		return false;
	else if (Src.RB.y < Dest.LT.y)
		return false;

	// �浹�� �� �簢���� ������ ��ǥ�� ���� 
	// ��ġ�� �κ��� �簢���� Left Right Top Bottom ��ġ�� ���� ��
	// �� �簢���� �߽��� HitPoint�� �����Ѵ�.
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
	// �� ���� �浹ü�� ���� ������ �Ÿ��� ���Ѵ�.
	float Dist = Src.Center.Distance(Dest.Center);

	// ���� ������ �Ÿ��� �� ���� �浹ü�� �������� ���� ������ ������ �浹�� ���̴�.
	bool Result = Dist <= Src.Radius + Dest.Radius;

	// �� ������ �մ� ������ ����� ��Ʈ����Ʈ�� �����Ѵ�.
	// (��Ȯ�� �� �浹ü�� ��ġ�� �κ��� �߽��� �ƴ����� �����ϰ� ����Ϸ��� �����ϹǷ�)
	HitPoint = (Src.Center + Dest.Center) / 2.f;

	return Result;
}

bool CCollisionManager::CollisionBoxToCircle(Vector2& HitPoint, const BoxInfo& Src,
	const CircleInfo& Dest)
{
	// 1. ���� ������ �簢���� ��, �� ���� Ȥ�� ��, �Ʒ� ������ �����ϴ��� �Ǵ��Ѵ�.
	if ((Src.LT.x <= Dest.Center.x && Dest.Center.x <= Src.RB.x) ||
		(Src.LT.y <= Dest.Center.y && Dest.Center.y <= Src.RB.y))
	{
		// �簢���� ���� ��������ŭ Ȯ���Ѵ�.
		BoxInfo RC = Src;
		RC.LT.x -= Dest.Radius;
		RC.LT.y -= Dest.Radius;
		RC.RB.x += Dest.Radius;
		RC.RB.y += Dest.Radius;

		// Ȯ��� �簢�� �ȿ� ���� ������ ���´ٸ� �浹�� ���̴�.
		// (�浹���� �ʴ� ������ ã�°� �� ȿ�����̴�)
		if (RC.LT.x > Dest.Center.x)
			return false;
		else if (RC.LT.y > Dest.Center.y)
			return false;
		else if (RC.RB.x < Dest.Center.x)
			return false;
		else if (RC.RB.y < Dest.Center.y)
			return false;

		// ��Ʈ����Ʈ�� ���Ѵ�.
		// (�� �浹ü�� ���������� ���� ������ �簢���� ���� 
		// �� �簢���� �簢�� �浹ü���� �������� �߽��� ��Ʈ����Ʈ�� �����Ѵ�)
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

	// 2. ���� Ȯ��� �簢�� �ȿ� ���͵� �����δ� �簢�� �浹ü�� �浹���� �ʴ� ������
	// �ֱ� ������ �� �κ��� Ȯ���Ѵ�.
	// �簢�� �浹ü�� 4���� ������ ��ġ�� ���Ѵ�.
	Vector2 Pos[4] =
	{
		Src.LT,
		Vector2(Src.RB.x, Src.LT.y),
		Vector2(Src.LT.x, Src.RB.y),
		Src.RB
	};

	// 4���� �� �� �ϳ��� �� �ȿ� ���´ٸ� �浹�� ���̴�.
	for (int i = 0; i < 4; i++)
	{
		float Dist = Dest.Center.Distance(Pos[i]);

		if (Dist <= Dest.Radius)
		{
			// ��Ʈ����Ʈ ���
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
	// ���� �簢�� �ȿ� ������ �ʾҴٸ� �浹���� ���� ���̴�.
	if (Dest.LT.x > Src.x)
		return false;
	else if (Dest.LT.y > Src.y)
		return false;
	else if (Dest.RB.x < Src.x)
		return false;
	else if (Dest.RB.y < Src.y)
		return false;

	// ��Ʈ����Ʈ�� �� ��ġ �״��.
	HitPoint = Src;

	return true;
}

bool CCollisionManager::CollisionPointToCircle(Vector2& HitPoint, const Vector2& Src,
	const CircleInfo& Dest)
{
	// ���� ���� ���� ������ �Ÿ��� ���Ѵ�.
	float Dist = Dest.Center.Distance(Src);

	// �Ÿ��� ���� ���������� �۰ų� ������ �浹�� ���̴�.
	if (Dist <= Dest.Radius)
	{
		// ��Ʈ����Ʈ�� �� ��ġ �״��.
		HitPoint = Src;

		return true;
	}

	return false;
}
