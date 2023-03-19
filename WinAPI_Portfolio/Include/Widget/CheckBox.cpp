#include "CheckBox.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Sound/Sound.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"
#include "../Input.h"

CCheckBox::CCheckBox() :
	m_ButtonState(EButton_State::Normal),
	m_StateData{},
	m_Check(false)
{
}

CCheckBox::CCheckBox(const CCheckBox& widget) :
	CWidget(widget)
{
}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CCheckBox::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#ifdef UNICODE

void CCheckBox::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CCheckBox::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#else

void CCheckBox::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CCheckBox::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#endif // UNICODE


void CCheckBox::SetButtonStateData(EButton_State State, const Vector2& Start, const Vector2& End)
{
	m_StateData[(int)State].Start = Start;
	m_StateData[(int)State].End = End;
}

void CCheckBox::SetSound(EButton_Sound_State State, const std::string& Name)
{
	m_StateSound[(int)State] = m_Scene->GetSceneResource()->FindSound(Name);
}

bool CCheckBox::Init()
{
	return true;
}

void CCheckBox::Update(float DeltaTime)
{
	m_Size = m_StateData[(int)m_ButtonState].End - m_StateData[(int)m_ButtonState].Start;

	// üũ�ڽ��� ��Ȱ��ȭ ���°� �ƴ� ���� Ŭ�� ó���� �Ѵ�.
	if (m_ButtonState != EButton_State::Disable)
	{
		// ���콺�� �ö� ���� ���� Ŭ���� �ȴ�.
		if (m_MouseHovered)
		{
			// ���콺 ���� üũ�ڽ��� �����ٸ� üũ�ڽ� ���¸� Ŭ������ �ٲ��ش�.
			if (CInput::GetInst()->GetMouseLDown())
			{
				if (m_Check)
					m_ButtonState = EButton_State::Checked;
				else
					m_ButtonState = EButton_State::Click;
			}
			// üũ�ڽ��� Ŭ�� ���¸鼭 ���콺 ���� ��ư�� �� ��쿡 
			// üũ ���ΰ� �޶�����.
			else if ((m_ButtonState == EButton_State::Click ||
				m_ButtonState == EButton_State::Checked) &&
				CInput::GetInst()->GetMouseLUp())
			{
				if (m_StateSound[(int)EButton_Sound_State::Click])
					m_StateSound[(int)EButton_Sound_State::Click]->Play();

				if (m_Callback[(int)EButton_Sound_State::Click])
					m_Callback[(int)EButton_Sound_State::Click]();

				// ���� üũ�� ���¶�� �ٽ� MouseHovered�� �ٲٰ�
				// üũ ���θ� false�� �ٲ۴�.
				if (m_Check)
				{
					m_ButtonState = EButton_State::MouseHovered;
					m_Check = false;
				}
				// ���� üũ���� ���� ���¶�� üũ�ڽ� ���¸� üũ������ �ٲٰ�
				// üũ ���θ� true�� �ٲ۴�.
				else
				{
					m_ButtonState = EButton_State::Checked;
					m_Check = true;
				}
			}
			// üũ�ڽ��� Ŭ�� ������ �� ���콺 ���� ��ư�� ��� ������ �ִٸ�
			// üũ ���ο� ���� üũ�ڽ� ���¸� Ŭ�� �Ǵ� üũ������ ��� �����Ѵ�.
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseLPush())
				m_ButtonState = EButton_State::Click;
			else if (m_ButtonState == EButton_State::Checked &&
				CInput::GetInst()->GetMouseLPush())
				m_ButtonState = EButton_State::Checked;
			// �� ��찡 ��� �ƴ϶�� �׳� ���콺�� �ö� �ִ� �����̴�.
			// üũ ���ο� ���� üũ�ڽ� ���°� �޶�����.
			else
			{
				if (m_Check)
					m_ButtonState = EButton_State::Checked;
				else
					m_ButtonState = EButton_State::MouseHovered;
			}
		}
		// ���콺�� �ö� ���� ������ Normal �����̴�.
		// üũ ���ο� ���� üũ�ڽ� ���°� �޶�����.
		else
		{
			if (m_Check)
				m_ButtonState = EButton_State::Checked;
			else
				m_ButtonState = EButton_State::Normal;
		}
	}
}

void CCheckBox::PostUpdate(float DeltaTime)
{
}

void CCheckBox::Render(HDC hDC, float DeltaTime)
{
	Vector2 RenderPos = m_Pos + m_Owner->GetPos();

	// �ؽ�ó�� ������ �ش� �ؽ�ó ũ��� ����ϰ�
	// ������ �⺻ �簢������ ����Ѵ�.
	if (m_Texture)
	{
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

void CCheckBox::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	Vector2 RenderPos = Pos;

	// �ؽ�ó�� ������ �ش� �ؽ�ó ũ��� ����ϰ�
	// ������ �⺻ �簢������ ����Ѵ�.
	if (m_Texture)
	{
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

void CCheckBox::CollisionMouseHoveredCallback(const Vector2& Pos)
{
	CWidget::CollisionMouseHoveredCallback(Pos);

	if (m_StateSound[(int)EButton_Sound_State::MouseHovered])
		m_StateSound[(int)EButton_Sound_State::MouseHovered]->Play();

	if (m_Callback[(int)EButton_Sound_State::MouseHovered])
		m_Callback[(int)EButton_Sound_State::MouseHovered];
}

void CCheckBox::CollisionMouseReleaseCallback()
{
	CWidget::CollisionMouseReleaseCallback();
}
