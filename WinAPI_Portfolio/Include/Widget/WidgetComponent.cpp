#include "WidgetComponent.h"
#include "Widget.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "WidgetWindow.h"
#include "../GameObject/GameObject.h"

CWidgetComponent::CWidgetComponent()
{
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& widget)
{
}

CWidgetComponent::~CWidgetComponent()
{
} 

Vector2 CWidgetComponent::GetPos() const
{
	return m_Pos + m_Owner->GetPos() - m_Scene->GetCamera()->GetPos();
}

float CWidgetComponent::GetBottom() const
{
	// 위젯 출력 시 정렬 기준을 잡기 위해 Bottom값을 구해서 리턴한다.
	Vector2 Pos = GetPos();

	float Bottom = Pos.y;

	if (m_Widget)
		Bottom += m_Widget->GetSize().y;

	return Bottom;
}

void CWidgetComponent::SetWidget(CWidget* Widget)
{
	m_Widget = Widget;
}

bool CWidgetComponent::Init()
{
	// 위젯 컴포넌트가 생성될 때 이 컴포넌트가 속한 씬에 자신을 추가해준다.
	m_Scene->AddWidgetComponent(this);

	return true;
}

void CWidgetComponent::Update(float DeltaTime)
{
	if (m_Widget)
		m_Widget->Update(DeltaTime);
}

void CWidgetComponent::PostUpdate(float DeltaTime)
{
	if (m_Widget)
		m_Widget->PostUpdate(DeltaTime);
}

void CWidgetComponent::Render(HDC hDC, float DeltaTime)
{
	if (m_Widget)
	{
		// 오브젝트와 카메라의 위치를 이용해 위젯이 출력될 위치를 정해준다.
		Vector2 Pos = m_Pos + m_Owner->GetPos() - m_Scene->GetCamera()->GetPos();

		m_Widget->Render(hDC, Pos, DeltaTime);
	}
}
