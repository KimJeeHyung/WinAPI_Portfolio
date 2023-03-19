#include "WidgetWindow.h"

CWidgetWindow::CWidgetWindow() :
	m_Scene(nullptr),
	m_ZOrder(0)
{
}

CWidgetWindow::~CWidgetWindow()
{
}

bool CWidgetWindow::Init()
{
	return true;
}

void CWidgetWindow::Update(float DeltaTime)
{
	std::vector<CSharedPtr<CWidget>>::iterator iter;
	std::vector<CSharedPtr<CWidget>>::iterator iterEnd;

	iter = m_vecWidget.begin();
	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		// ������ �ִ� UI���� Update�� �����Ѵ�.
		(*iter)->Update(DeltaTime);
		++iter;
	}

	//// ������ �ִ� UI���� Update�� �����Ѵ�.
	//size_t Size = m_vecWidget.size();

	//for (size_t i = 0; i < Size; i++)
	//{
	//	m_vecWidget[i]->Update(DeltaTime);
	//}
}

void CWidgetWindow::PostUpdate(float DeltaTime)
{
	std::vector<CSharedPtr<CWidget>>::iterator iter;
	std::vector<CSharedPtr<CWidget>>::iterator iterEnd;

	iter = m_vecWidget.begin();
	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		// ������ �ִ� UI���� PostUpdate�� �����Ѵ�.
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	//// ������ �ִ� UI���� PostUpdate�� �����Ѵ�.
	//size_t Size = m_vecWidget.size();

	//for (size_t i = 0; i < Size; i++)
	//{
	//	m_vecWidget[i]->PostUpdate(DeltaTime);
	//}
}

void CWidgetWindow::Render(HDC hDC, float DeltaTime)
{
	size_t Size = m_vecWidget.size();

	// ������ 2�� �̻��̶�� Z���� ���� ��� ������ �������ش�.
	if (Size > 1)
	{
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CWidgetWindow::SortRenderWidget);
	}

	std::vector<CSharedPtr<CWidget>>::iterator iter;
	std::vector<CSharedPtr<CWidget>>::iterator iterEnd;

	iter = m_vecWidget.begin();
	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		// ������ �ִ� UI���� Render�� �����Ѵ�.
		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}

	//// ������ �ִ� �������� Render�� �����Ѵ�.
	//for (size_t i = 0; i < Size; i++)
	//{
	//	m_vecWidget[i]->Render(hDC, DeltaTime);
	//}
}

void CWidgetWindow::SortCollision()
{
	// ���� �����쿡 ������ 2�� �̻� ���� ��� 
	// Z���� ���� ���콺 �浹 ������ ���ϱ� ���� ������ ���ش�.
	if (m_vecWidget.size() > 1)
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CWidgetWindow::SortCollisionWidget);
}

bool CWidgetWindow::CollisionMouse(class CWidget** Widget, const Vector2& Pos)
{
	// ���� ������� �浹���� �ʴ� ��츦 �ɷ�����.
	if (Pos.x < m_Pos.x)
		return false;
	else if (Pos.y < m_Pos.y)
		return false;
	else if (Pos.x > m_Pos.x + m_Size.x)
		return false;
	else if (Pos.y > m_Pos.y + m_Size.y)
		return false;

	// ���� ������� �浹�� �ߴٸ� ���� �ش� �����찡 ������ �ִ� �������� �浹�� Ȯ���Ѵ�.
	size_t WidgetCount = m_vecWidget.size();

	for (size_t i = 0; i < WidgetCount; i++)
	{
		if (m_vecWidget[i]->CollisionMouse(Pos))
		{
			// �浹�� ������ �����Ѵ�.
			*Widget = m_vecWidget[i];
			return true;
		}
	}

	return false;
}

bool CWidgetWindow::SortCollisionWidget(const CSharedPtr<class CWidget>& Src,
	const CSharedPtr<class CWidget>& Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}

bool CWidgetWindow::SortRenderWidget(const CSharedPtr<class CWidget>& Src, const CSharedPtr<class CWidget>& Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}
