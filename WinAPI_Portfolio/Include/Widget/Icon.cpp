#include "Icon.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Sound/Sound.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"
#include "../Input.h"

CIcon::CIcon() :
	m_ButtonState(EButton_State::Normal),
	m_StateData{}
{
}

CIcon::CIcon(const CIcon& widget) :
	CWidget(widget)
{
}

CIcon::~CIcon()
{
}

void CIcon::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CIcon::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#ifdef UNICODE

void CIcon::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CIcon::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#else

void CIcon::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CIcon::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#endif // UNICODE


void CIcon::SetButtonStateData(EButton_State State, const Vector2& Start, const Vector2& End)
{
	m_StateData[(int)State].Start = Start;
	m_StateData[(int)State].End = End;
}

void CIcon::SetSound(EButton_Sound_State State, const std::string& Name)
{
	m_StateSound[(int)State] = m_Scene->GetSceneResource()->FindSound(Name);
}

bool CIcon::Init()
{
	return true;
}

void CIcon::Update(float DeltaTime)
{
	m_Size = m_StateData[(int)m_ButtonState].End - m_StateData[(int)m_ButtonState].Start;

	// 아이콘이 비활성화 상태가 아닐 때만 클릭 처리를 한다.
	if (m_ButtonState != EButton_State::Disable)
	{
		// 마우스가 올라가 있을 때만 클릭이 된다.
		if (m_MouseHovered)
		{
			// 마우스 왼쪽 버튼을 눌렀다면 아이콘 상태를 클릭으로 바꿔준다.
			if (CInput::GetInst()->GetMouseLDown())
				m_ButtonState = EButton_State::Click;
			// 아이콘이 클릭 상태면서 마우스 왼쪽 버튼을 뗄 경우에 
			// 아이콘 기능이 동작하도록 한다.
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseLUp())
			{
				if (m_StateSound[(int)EButton_Sound_State::Click])
					m_StateSound[(int)EButton_Sound_State::Click]->Play();

				if (m_Callback[(int)EButton_Sound_State::Click])
					m_Callback[(int)EButton_Sound_State::Click]();

				// 기능이 동작한 후 다시 MouseHovered 상태로 바꿔준다.
				m_ButtonState = EButton_State::MouseHovered;
			}
			// 아이콘이 클릭 상태일 때 마우스 왼쪽 버튼을 계속 누르고 있다면
			// 아이콘 상태를 클릭으로 계속 유지한다.
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseLPush())
			{
				m_ButtonState = EButton_State::Click;

				// 클릭을 유지한 채로 마우스를 움직이면
				// 아이콘도 같이 움직이게 한다.
				Vector2 Move = CInput::GetInst()->GetMouseMove();
				m_Pos += Move;
			}
			// 위 경우가 모두 아니라면 그냥 마우스만 올라가 있는 상태이다.
			else
				m_ButtonState = EButton_State::MouseHovered;
		}
		// 마우스가 올라가 있지 않으면 Normal 상태이다.
		else
		{
			m_ButtonState = EButton_State::Normal;
		}
	}
}

void CIcon::PostUpdate(float DeltaTime)
{
}

void CIcon::Render(HDC hDC, float DeltaTime)
{
	Vector2 RenderPos = m_Pos + m_Owner->GetPos();

	// 텍스처가 있으면 해당 텍스처 크기로 출력하고
	// 없으면 기본 사각형으로 출력한다.
	if (m_Texture)
	{
		if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
		{
			// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
			if (m_Texture->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)m_StateData[(int)m_ButtonState].Start.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetColorKey());
			}
			// 일반 비트맵 출력 함수
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)(int)m_StateData[(int)m_ButtonState].Start.y, SRCCOPY);
			}
		}
		else
		{
		}
	}
	else
	{
		Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y,
			(int)(RenderPos.x + m_Size.x), (int)(RenderPos.y + m_Size.y));
	}
}

void CIcon::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	Vector2 RenderPos = Pos;

	// 텍스처가 있으면 해당 텍스처 크기로 출력하고
	// 없으면 기본 사각형으로 출력한다.
	if (m_Texture)
	{
		if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
		{
			// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
			if (m_Texture->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)m_StateData[(int)m_ButtonState].Start.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetColorKey());
			}
			// 일반 비트맵 출력 함수
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)(int)m_StateData[(int)m_ButtonState].Start.y, SRCCOPY);
			}
		}
		else
		{
		}
	}
	else
	{
		Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y,
			(int)(RenderPos.x + m_Size.x), (int)(RenderPos.y + m_Size.y));
	}
}

void CIcon::CollisionMouseHoveredCallback(const Vector2& Pos)
{
	CWidget::CollisionMouseHoveredCallback(Pos);

	if (m_StateSound[(int)EButton_Sound_State::MouseHovered])
		m_StateSound[(int)EButton_Sound_State::MouseHovered]->Play();

	if (m_Callback[(int)EButton_Sound_State::MouseHovered])
		m_Callback[(int)EButton_Sound_State::MouseHovered];
}

void CIcon::CollisionMouseReleaseCallback()
{
	CWidget::CollisionMouseReleaseCallback();
}