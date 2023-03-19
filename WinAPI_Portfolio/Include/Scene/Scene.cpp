#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "SceneResource.h"
#include "Camera.h"
#include "SceneCollision.h"
#include "../GameObject/TileMap.h"

CScene::CScene()
{
	m_Resource = new CSceneResource;
	m_Camera = new CCamera;
	m_Collision = new CSceneCollision;
}

CScene::~CScene()
{
	for (int i = 0; i < (int)ERender_Layer::Max; ++i)
	{
		// 리소스를 지우기 전에 GameObject부터 지운다.
		m_ObjList[i].clear();
	}

	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Camera);
	SAFE_DELETE(m_Resource);
}

void CScene::SetTileMap(CTileMap* TileMap)
{
	m_TileMap = TileMap;
}

void CScene::SetPlayer(CGameObject* Player)
{
	m_Player = Player;
}

bool CScene::Init()
{
	return true;
}

void CScene::Update(float DeltaTime)
{
	for (int i = 0; i < (int)ERender_Layer::Max; ++i)
	{
		std::list<CSharedPtr<CGameObject>>::iterator iter;
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd;

		iter = m_ObjList[i].begin();
		iterEnd = m_ObjList[i].end();

		for (; iter != iterEnd;)
		{
			// 오브젝트의 Start 함수를 실행
			if (!(*iter)->m_Start)
			{
				(*iter)->Start();
			}

			if (!(*iter)->GetActive())
			{
				// 리스트에서 제거하는 순간 SharedPtr의 소멸자가 호출되어
				// 카운트가 감소한다.
				iter = m_ObjList[i].erase(iter);
				iterEnd = m_ObjList[i].end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);

			++iter;
		}
	}

	std::vector<CSharedPtr<CWidgetWindow>>::iterator iter;
	std::vector<CSharedPtr<CWidgetWindow>>::iterator iterEnd;

	iter = m_vecWidgetWindow.begin();
	iterEnd = m_vecWidgetWindow.end();

	for (; iter != iterEnd;)
	{
		// 위젯윈도우가 제거되었다면 목록에서 없앤다.
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		// 위젯윈도우가 비활성화되어 있다면 바로 넘어간다.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_Camera->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	for (int i = 0; i < (int)ERender_Layer::Max; ++i)
	{
		std::list<CSharedPtr<CGameObject>>::iterator iter;
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd;

		iter = m_ObjList[i].begin();
		iterEnd = m_ObjList[i].end();

		for (; iter != iterEnd;)
		{
			// 오브젝트의 Start 함수를 실행
			if (!(*iter)->m_Start)
			{
				(*iter)->Start();
			}

			if (!(*iter)->GetActive())
			{
				// 리스트에서 제거하는 순간 SharedPtr의 소멸자가 호출되어
				// 카운트가 감소한다.
				iter = m_ObjList[i].erase(iter);
				iterEnd = m_ObjList[i].end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);

			++iter;
		}
	}

	std::vector<CSharedPtr<CWidgetWindow>>::iterator iter;
	std::vector<CSharedPtr<CWidgetWindow>>::iterator iterEnd;

	iter = m_vecWidgetWindow.begin();
	iterEnd = m_vecWidgetWindow.end();

	for (; iter != iterEnd;)
	{
		// 위젯윈도우가 제거되었다면 목록에서 없앤다.
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		// 위젯윈도우가 비활성화되어 있다면 바로 넘어간다.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	// PostUpdate가 끝난 후 위젯 윈도우의 충돌 처리를 위해
	// 위젯 윈도우를 SceneCollision 객체에 추가한다.
	iter = m_vecWidgetWindow.begin();
	iterEnd = m_vecWidgetWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		m_Collision->AddWidgetWindow(*iter);
		++iter;
	}

	m_Collision->CollisionMouse(DeltaTime);

	m_Collision->Collision(DeltaTime);
}

void CScene::Render(HDC hDC, float DeltaTime)
{
	for (int i = 0; i < (int)ERender_Layer::Max; ++i)
	{
		// 출력 목록을 정렬한다. (Y소팅)
		// 오브젝트가 속한 레이어와 Y좌표에 따라 그리는 순서가 달라진다.
		m_ObjList[i].sort(CScene::SortY);

		std::list<CSharedPtr<CGameObject>>::iterator iter;
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd;

		iter = m_ObjList[i].begin();
		iterEnd = m_ObjList[i].end();

		for (; iter != iterEnd;)
		{
			// 오브젝트의 Start 함수를 실행
			if (!(*iter)->m_Start)
			{
				(*iter)->Start();
			}

			if (!(*iter)->GetActive())
			{
				// 리스트에서 제거하는 순간 SharedPtr의 소멸자가 호출되어
				// 카운트가 감소한다.
				iter = m_ObjList[i].erase(iter);
				iterEnd = m_ObjList[i].end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Render(hDC, DeltaTime);

			++iter;
		}
	}

	// 위젯 컴포넌트 출력
	// 제거될 위젯 컴포넌트는 제거한다.
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			// 위젯이 없어졌다면 목록에서 지우고 다음 위젯으로 넘어간다.
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}

			++iter;
		}

		// 정렬한다.
		m_WidgetComponentList.sort(SortYWidgetComponent);

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			// 위젯이 없어졌다면 목록에서 지우고 다음 위젯으로 넘어간다.
			if (!(*iter)->GetEnable())
				continue;
			
			(*iter)->Render(hDC, DeltaTime);
		}
	}

	// 월드공간의 물체가 출력된 이후에 UI를 출력한다.
	if (m_vecWidgetWindow.size() > 1)
		std::sort(m_vecWidgetWindow.begin(), m_vecWidgetWindow.end(), CScene::SortWiget);

	std::vector<CSharedPtr<CWidgetWindow>>::iterator iter;
	std::vector<CSharedPtr<CWidgetWindow>>::iterator iterEnd;

	iter = m_vecWidgetWindow.begin();
	iterEnd = m_vecWidgetWindow.end();

	for (; iter != iterEnd;)
	{
		// 위젯윈도우가 제거되었다면 목록에서 없앤다.
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		// 위젯윈도우가 비활성화되어 있다면 바로 넘어간다.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}

	// UI를 출력한 이후에 마우스를 출력한다.
}

bool CScene::SortY(const CSharedPtr<CGameObject>& Src, 
	const CSharedPtr<CGameObject>& Dest)
{
	// 발밑 기준으로 정렬을 진행해야 하기 때문에 발밑의 Y값을 구해준다.
	float SrcY = Src->GetPos().y + (1.f - Src->GetPivot().y) * Src->GetSize().y;
	float DestY = Dest->GetPos().y + (1.f - Dest->GetPivot().y) * Dest->GetSize().y;

	return SrcY < DestY;
}

bool CScene::SortYWidgetComponent(const CSharedPtr<class CWidgetComponent>& Src,
	const CSharedPtr<class CWidgetComponent>& Dest)
{
	return Src->GetBottom() < Dest->GetBottom();
}

bool CScene::SortWiget(const CSharedPtr<class CWidgetWindow>& Src, const CSharedPtr<class CWidgetWindow>& Dest)
{
	// Z값이 클수록 나중에 그려지도록 정렬한다.
	return Src->GetZOrder() < Dest->GetZOrder();
}
