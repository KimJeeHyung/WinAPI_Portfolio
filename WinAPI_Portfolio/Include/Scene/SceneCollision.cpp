#include "SceneCollision.h"
#include "../Collision/Collider.h"
#include "../Input.h"
#include "../Widget/WidgetWindow.h"
#include "../Widget/Widget.h"

CSceneCollision::CSceneCollision() :
	m_MouseCollision(nullptr),
	m_MouseCollisionWidget(nullptr)
{
	m_vecCollider.reserve(500);
}

CSceneCollision::~CSceneCollision()
{
}

void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_vecCollider.push_back(Collider);
}

void CSceneCollision::AddWidgetWindow(CWidgetWindow* Window)
{
	m_vecWidgetWindow.push_back(Window);
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	size_t WindowCount = m_vecWidgetWindow.size();

	bool MouseCollision = false;	// 현재 마우스와 충돌한 물체가 있는지 여부.

	// 위젯 윈도우가 하나라도 마우스와 충돌할 때만 충돌 처리를 하게 해서
	// 성능 최적화를 해준다.
	if (WindowCount >= 1)
	{
		// 위젯 윈도우가 2개 이상이면 정렬해준다.
		if (WindowCount > 1)
			std::sort(m_vecWidgetWindow.begin(), m_vecWidgetWindow.end(), CSceneCollision::SortWidget);

		Vector2 MousePos = CInput::GetInst()->GetMousePos();

		CWidget* result = nullptr;

		for (size_t i = 0; i < WindowCount; i++)
		{
			m_vecWidgetWindow[i]->SortCollision();

			if (m_vecWidgetWindow[i]->CollisionMouse(&result, MousePos))
			{
				// 기존에 충돌되던 위젯이 있을 경우 마우스와의 충돌을 해제한다.
				if (m_MouseCollisionWidget && m_MouseCollisionWidget != result)
					m_MouseCollisionWidget->CollisionMouseReleaseCallback();

				// 위젯이 충돌되었는데 기존에 마우스와 충돌되던 충돌체가 있을 경우
				if (m_MouseCollision)
				{
					m_MouseCollision->SetMouseCollision(false);
					m_MouseCollision->CallMouseCollisionEnd(CInput::GetInst()->GetMouseWorldPos());
					m_MouseCollision = nullptr;
				}

				m_MouseCollisionWidget = result;

				MouseCollision = true;

				break;
			}
		}
	}

	// 마우스가 UI와 충돌하지 않았을 때만 물체와 충돌 처리를 한다.(성능 최적화)
	if (!MouseCollision)
	{
		if (m_MouseCollisionWidget)
		{
			m_MouseCollisionWidget->CollisionMouseReleaseCallback();
			m_MouseCollisionWidget = nullptr;
		}

		size_t Size = m_vecCollider.size();

		if (Size >= 1)
		{
			// 충돌체가 2개 이상이라면 Y정렬을 해준다.
			if (Size > 1)
				std::sort(m_vecCollider.begin(), m_vecCollider.end(), CSceneCollision::SortY);

			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
			CollisionProfile* MouseProfile = CInput::GetInst()->GetMouseProfile();

			// 충돌체 배열의 첫번째 인덱스부터 마지막 인덱스까지 반복하면서 
			// 나머지 인덱스와 충돌을 했는지 모두 체크한다.
			for (size_t i = 0; i < Size; i++)
			{
				CCollider* Dest = m_vecCollider[i];

				// 충돌 Profile을 가져와서 충돌시켜야 하는 물체인지 판단한다.
				const CollisionProfile* SrcProfile = MouseProfile;
				const CollisionProfile* DestProfile = Dest->GetProfile();

				// Profile에는 다른 채널과 상호작용을 어떻게 해야되는지가 정해져있다.
				// 그러므로 상대방 채널번째의 상호작용을 얻어와서 무시인지 충돌인지를 알아내야 한다.
				ECollision_Interaction SrcInteraction = SrcProfile->vecCollisionInteraction[(int)DestProfile->Channel];
				ECollision_Interaction DestInteraction = DestProfile->vecCollisionInteraction[(int)SrcProfile->Channel];

				// 둘 중 하나라도 무시하기로 되어 있다면 충돌을 멈춘다.
				if (SrcInteraction == ECollision_Interaction::Ignore ||
					DestInteraction == ECollision_Interaction::Ignore)
					continue;

				// 충돌을 체크한다.
				if (Dest->CollisionMouse(MouseWorldPos))
				{
					MouseCollision = true;

					// 충돌했는데 마우스 충돌이 false라면
					// 이제 막 충돌을 시작했다는 뜻이다.
					if (!Dest->GetMouseCollision())
					{
						// 기존에 충돌되던 물체가 있을 경우 마우스와의 충돌을 해제한다.
						if (m_MouseCollision)
							m_MouseCollision->SetMouseCollision(false);

						Dest->SetMouseCollision(true);

						// 충돌 시작 함수를 호출한다.
						Dest->CallMouseCollisionBegin(MouseWorldPos);
					}

					m_MouseCollision = Dest;

					// 충돌한 물체가 있을 경우 다른 충돌체와는 충돌할 필요가 없으므로
					// 바로 빠져나간다.
					break;
				}
				// 충돌을 하지 않고 있는데 충돌체 목록에 상대방이 있다면
				// 충돌이 되던 물체가 서로 떨어졌다는 뜻이다.
				else if (Dest->GetMouseCollision())
				{
					m_MouseCollision->SetMouseCollision(false);
					m_MouseCollision = nullptr;
				}
			}

			// MouseCollision 변수가 false를 유지하고 있다면 어떤 물체와도 충돌이 안 되었다는 것이다.
			// 그러므로 이 경우에는 기존에 충돌되고 있던 물체가 있다면 지금은 충돌이 안 되는 것이기
			// 때문에 예외처리를 해주어야 한다.
			if (!MouseCollision)
			{
				// 기존에 충돌되던 물체가 있을 경우 마우스와의 충돌을 해제한다.
				if (m_MouseCollision)
				{
					m_MouseCollision->SetMouseCollision(false);
					m_MouseCollision = nullptr;
				}
			}
		}
	}
}

void CSceneCollision::Collision(float DeltaTime)
{
	size_t Size = m_vecCollider.size();

	// 충돌체가 두개 이상이어야 충돌 체크를 할 수 있다.
	if (Size > 1)
	{
		// 충돌체 배열의 첫번째 인덱스부터 마지막 - 1번째 인덱스까지 반복하면서 
		// 나머지 인덱스와 충돌을 했는지 모두 체크한다.
		for (size_t i = 0; i < Size - 1; i++)
		{
			CCollider* Src = m_vecCollider[i];

			for (size_t j = i + 1; j < Size; j++)
			{
				CCollider* Dest = m_vecCollider[j];

				// 두 충돌체가 같은 오브젝트에 속해 있다면 넘어간다.
				if (Src->GetOwner() == Dest->GetOwner())
					continue;

				// 충돌 Profile을 가져와서 충돌시켜야 하는 물체인지 판단한다.
				const CollisionProfile* SrcProfile = Src->GetProfile();
				const CollisionProfile* DestProfile = Dest->GetProfile();

				// Profile에는 다른 채널과 상호작용을 어떻게 해야되는지가 정해져있다.
				// 그러므로 상대방 채널번째의 상호작용을 얻어와서 무시인지 충돌인지를 알아내야 한다.
				ECollision_Interaction SrcInteraction = SrcProfile->vecCollisionInteraction[(int)DestProfile->Channel];
				ECollision_Interaction DestInteraction = DestProfile->vecCollisionInteraction[(int)SrcProfile->Channel];

				// 둘 중 하나라도 무시하기로 되어 있다면 충돌을 멈춘다.
				if (SrcInteraction == ECollision_Interaction::Ignore ||
					DestInteraction == ECollision_Interaction::Ignore)
					continue;

				// 충돌을 체크한다.
				if (Src->Collision(Dest))
				{
					// 충돌했는데 충돌체 목록에 없다면
					// 이제 막 충돌을 시작했다는 뜻이다.
					if (!Src->CheckCollisionList(Dest))
					{
						// 서로에게 상대방 충돌체를 충돌체 목록에 등록한다.
						Src->AddCollisionList(Dest);
						Dest->AddCollisionList(Src);

						// 충돌 시작 함수를 호출한다.
						Src->CallCollisionBegin(Dest);
						Dest->CallCollisionBegin(Src);
					}
				}
				// 충돌을 하지 않고 있는데 충돌체 목록에 상대방이 있다면
				// 충돌이 되던 물체가 서로 떨어졌다는 뜻이다.
				else if (Src->CheckCollisionList(Dest))
				{
					// 서로 목록에서 충돌체를 지워준다.
					Src->DeleteCollisionList(Dest);
					Dest->DeleteCollisionList(Src);

					// 충돌 종료 함수를 호출한다.
					Src->CallCollisionEnd(Dest);
					Dest->CallCollisionEnd(Src);
				}
			}
		}
	}

	// 매 프레임마다 충돌 처리를 하기 위해 목록을 비워준다.
	m_vecCollider.clear();
	m_vecWidgetWindow.clear();
}

bool CSceneCollision::SortY(CCollider* Src, CCollider* Dest)
{
	// 제일 위쪽에 그려진 오브젝트를 찾기 위해 내림차순(>)으로 정렬한다.
	return Src->GetBottom() > Dest->GetBottom();
}

bool CSceneCollision::SortWidget(CWidgetWindow* Src, CWidgetWindow* Dest)
{
	// 제일 위쪽에 그려진 UI를 찾기 위해 내림차순(>)으로 정렬한다.
	return Src->GetZOrder() > Dest->GetZOrder();
}
