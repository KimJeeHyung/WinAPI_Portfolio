#include "Button.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Sound/Sound.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"
#include "../Input.h"

CButton::CButton() :
	m_ButtonState(EButton_State::Normal),
	m_StateData{}
{
}

CButton::CButton(const CButton& widget) :
	CWidget(widget)
{
}

CButton::~CButton()
{
}

void CButton::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CButton::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#ifdef UNICODE

void CButton::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CButton::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#else

void CButton::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CButton::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#endif // UNICODE


void CButton::SetColorKey(unsigned char r, unsigned char g, unsigned char b)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b);
}

void CButton::SetButtonStateData(EButton_State State, const Vector2& Start, const Vector2& End)
{
	m_StateData[(int)State].Start = Start;
	m_StateData[(int)State].End = End;
}

void CButton::SetSound(EButton_Sound_State State, const std::string& Name)
{
	m_StateSound[(int)State] = m_Scene->GetSceneResource()->FindSound(Name);
}

bool CButton::Init()
{
	return true;
}

void CButton::Update(float DeltaTime)
{
	m_Size = m_StateData[(int)m_ButtonState].End - m_StateData[(int)m_ButtonState].Start;

	// ��ư�� ��Ȱ��ȭ ���°� �ƴ� ���� Ŭ�� ó���� �Ѵ�.
	if (m_ButtonState != EButton_State::Disable)
	{
		// ���콺�� �ö� ���� ���� Ŭ���� �ȴ�.
		if (m_MouseHovered)
		{
			// ���콺 ���� ��ư�� �����ٸ� ��ư ���¸� Ŭ������ �ٲ��ش�.
			if(CInput::GetInst()->GetMouseLDown())
				m_ButtonState = EButton_State::Click;
			// ��ư�� Ŭ�� ���¸鼭 ���콺 ���� ��ư�� �� ��쿡 
			// ��ư ����� �����ϵ��� �Ѵ�.
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseLUp())
			{
				if (m_StateSound[(int)EButton_Sound_State::Click])
					m_StateSound[(int)EButton_Sound_State::Click]->Play();

				if (m_Callback[(int)EButton_Sound_State::Click])
					m_Callback[(int)EButton_Sound_State::Click]();

				// ����� ������ �� �ٽ� MouseHovered ���·� �ٲ��ش�.
				m_ButtonState = EButton_State::MouseHovered;
			}
			// ��ư�� Ŭ�� ������ �� ���콺 ���� ��ư�� ��� ������ �ִٸ�
			// ��ư ���¸� Ŭ������ ��� �����Ѵ�.
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseLPush())
				m_ButtonState = EButton_State::Click;
			// �� ��찡 ��� �ƴ϶�� �׳� ���콺�� �ö� �ִ� �����̴�.
			else
				m_ButtonState = EButton_State::MouseHovered;
		}
		// ���콺�� �ö� ���� ������ Normal �����̴�.
		else
		{
			m_ButtonState = EButton_State::Normal;
		}
	}
}

void CButton::PostUpdate(float DeltaTime)
{
}

void CButton::Render(HDC hDC, float DeltaTime)
{
	Vector2 RenderPos = m_Pos + m_Owner->GetPos();

	// �ؽ�ó�� ������ �ش� �ؽ�ó ũ��� ����ϰ�
	// ������ �⺻ �簢������ ����Ѵ�.
	if (m_Texture)
	{
		// ��������Ʈ �ؽ�ó
		if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
		{
			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
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
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)(int)m_StateData[(int)m_ButtonState].Start.y, SRCCOPY);
			}
		}
		// ������ �ؽ�ó
		else
		{
			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
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
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)m_StateData[(int)m_ButtonState].Start.y, SRCCOPY);
			}
		}
	}
	else
	{
		Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y, 
			(int)(RenderPos.x + m_Size.x), (int)(RenderPos.y + m_Size.y));
	}
}

void CButton::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	Vector2 RenderPos = Pos;

	// �ؽ�ó�� ������ �ش� �ؽ�ó ũ��� ����ϰ�
	// ������ �⺻ �簢������ ����Ѵ�.
	if (m_Texture)
	{
		// ��������Ʈ �ؽ�ó
		if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
		{
			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
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
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)(int)m_StateData[(int)m_ButtonState].Start.y, SRCCOPY);
			}
		}
		// ������ �ؽ�ó
		else
		{
			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
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
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					(int)m_StateData[(int)m_ButtonState].Start.x,
					(int)m_StateData[(int)m_ButtonState].Start.y, SRCCOPY);
			}
		}
	}
	else
	{
		Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y,
			(int)(RenderPos.x + m_Size.x), (int)(RenderPos.y + m_Size.y));
	}
}

void CButton::CollisionMouseHoveredCallback(const Vector2& Pos)
{
	CWidget::CollisionMouseHoveredCallback(Pos);

	if (m_StateSound[(int)EButton_Sound_State::MouseHovered])
		m_StateSound[(int)EButton_Sound_State::MouseHovered]->Play();

	if (m_Callback[(int)EButton_Sound_State::MouseHovered])
		m_Callback[(int)EButton_Sound_State::MouseHovered];
}

void CButton::CollisionMouseReleaseCallback()
{
	CWidget::CollisionMouseReleaseCallback();
}
