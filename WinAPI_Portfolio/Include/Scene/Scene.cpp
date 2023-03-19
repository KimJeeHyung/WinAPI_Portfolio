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
		// ���ҽ��� ����� ���� GameObject���� �����.
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
			// ������Ʈ�� Start �Լ��� ����
			if (!(*iter)->m_Start)
			{
				(*iter)->Start();
			}

			if (!(*iter)->GetActive())
			{
				// ����Ʈ���� �����ϴ� ���� SharedPtr�� �Ҹ��ڰ� ȣ��Ǿ�
				// ī��Ʈ�� �����Ѵ�.
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
		// ���������찡 ���ŵǾ��ٸ� ��Ͽ��� ���ش�.
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		// ���������찡 ��Ȱ��ȭ�Ǿ� �ִٸ� �ٷ� �Ѿ��.
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
			// ������Ʈ�� Start �Լ��� ����
			if (!(*iter)->m_Start)
			{
				(*iter)->Start();
			}

			if (!(*iter)->GetActive())
			{
				// ����Ʈ���� �����ϴ� ���� SharedPtr�� �Ҹ��ڰ� ȣ��Ǿ�
				// ī��Ʈ�� �����Ѵ�.
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
		// ���������찡 ���ŵǾ��ٸ� ��Ͽ��� ���ش�.
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		// ���������찡 ��Ȱ��ȭ�Ǿ� �ִٸ� �ٷ� �Ѿ��.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	// PostUpdate�� ���� �� ���� �������� �浹 ó���� ����
	// ���� �����츦 SceneCollision ��ü�� �߰��Ѵ�.
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
		// ��� ����� �����Ѵ�. (Y����)
		// ������Ʈ�� ���� ���̾�� Y��ǥ�� ���� �׸��� ������ �޶�����.
		m_ObjList[i].sort(CScene::SortY);

		std::list<CSharedPtr<CGameObject>>::iterator iter;
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd;

		iter = m_ObjList[i].begin();
		iterEnd = m_ObjList[i].end();

		for (; iter != iterEnd;)
		{
			// ������Ʈ�� Start �Լ��� ����
			if (!(*iter)->m_Start)
			{
				(*iter)->Start();
			}

			if (!(*iter)->GetActive())
			{
				// ����Ʈ���� �����ϴ� ���� SharedPtr�� �Ҹ��ڰ� ȣ��Ǿ�
				// ī��Ʈ�� �����Ѵ�.
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

	// ���� ������Ʈ ���
	// ���ŵ� ���� ������Ʈ�� �����Ѵ�.
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			// ������ �������ٸ� ��Ͽ��� ����� ���� �������� �Ѿ��.
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}

			++iter;
		}

		// �����Ѵ�.
		m_WidgetComponentList.sort(SortYWidgetComponent);

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			// ������ �������ٸ� ��Ͽ��� ����� ���� �������� �Ѿ��.
			if (!(*iter)->GetEnable())
				continue;
			
			(*iter)->Render(hDC, DeltaTime);
		}
	}

	// ��������� ��ü�� ��µ� ���Ŀ� UI�� ����Ѵ�.
	if (m_vecWidgetWindow.size() > 1)
		std::sort(m_vecWidgetWindow.begin(), m_vecWidgetWindow.end(), CScene::SortWiget);

	std::vector<CSharedPtr<CWidgetWindow>>::iterator iter;
	std::vector<CSharedPtr<CWidgetWindow>>::iterator iterEnd;

	iter = m_vecWidgetWindow.begin();
	iterEnd = m_vecWidgetWindow.end();

	for (; iter != iterEnd;)
	{
		// ���������찡 ���ŵǾ��ٸ� ��Ͽ��� ���ش�.
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidgetWindow.erase(iter);
			iterEnd = m_vecWidgetWindow.end();
			continue;
		}
		// ���������찡 ��Ȱ��ȭ�Ǿ� �ִٸ� �ٷ� �Ѿ��.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}

	// UI�� ����� ���Ŀ� ���콺�� ����Ѵ�.
}

bool CScene::SortY(const CSharedPtr<CGameObject>& Src, 
	const CSharedPtr<CGameObject>& Dest)
{
	// �߹� �������� ������ �����ؾ� �ϱ� ������ �߹��� Y���� �����ش�.
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
	// Z���� Ŭ���� ���߿� �׷������� �����Ѵ�.
	return Src->GetZOrder() < Dest->GetZOrder();
}
