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

		// 가지고 있는 UI들의 Update를 실행한다.
		(*iter)->Update(DeltaTime);
		++iter;
	}

	//// 가지고 있는 UI들의 Update를 실행한다.
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

		// 가지고 있는 UI들의 PostUpdate를 실행한다.
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	//// 가지고 있는 UI들의 PostUpdate를 실행한다.
	//size_t Size = m_vecWidget.size();

	//for (size_t i = 0; i < Size; i++)
	//{
	//	m_vecWidget[i]->PostUpdate(DeltaTime);
	//}
}

void CWidgetWindow::Render(HDC hDC, float DeltaTime)
{
	size_t Size = m_vecWidget.size();

	// 위젯이 2개 이상이라면 Z값에 따라 출력 순서를 정렬해준다.
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

		// 가지고 있는 UI들의 Render를 실행한다.
		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}

	//// 가지고 있는 위젯들의 Render를 실행한다.
	//for (size_t i = 0; i < Size; i++)
	//{
	//	m_vecWidget[i]->Render(hDC, DeltaTime);
	//}
}

void CWidgetWindow::SortCollision()
{
	// 위젯 윈도우에 위젯이 2개 이상 있을 경우 
	// Z값에 따른 마우스 충돌 순서를 정하기 위해 정렬을 해준다.
	if (m_vecWidget.size() > 1)
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CWidgetWindow::SortCollisionWidget);
}

bool CWidgetWindow::CollisionMouse(class CWidget** Widget, const Vector2& Pos)
{
	// 위젯 윈도우와 충돌하지 않는 경우를 걸러낸다.
	if (Pos.x < m_Pos.x)
		return false;
	else if (Pos.y < m_Pos.y)
		return false;
	else if (Pos.x > m_Pos.x + m_Size.x)
		return false;
	else if (Pos.y > m_Pos.y + m_Size.y)
		return false;

	// 위젯 윈도우와 충돌을 했다면 이제 해당 윈도우가 가지고 있는 위젯과의 충돌을 확인한다.
	size_t WidgetCount = m_vecWidget.size();

	for (size_t i = 0; i < WidgetCount; i++)
	{
		if (m_vecWidget[i]->CollisionMouse(Pos))
		{
			// 충돌한 위젯을 전달한다.
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
