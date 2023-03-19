#include "ProgressBar.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"

CProgressBar::CProgressBar() :
	m_Dir(EProgressBar_Dir::LeftToRight),
	m_Value(1.f)
{
}

CProgressBar::CProgressBar(const CProgressBar& widget) :
	CWidget(widget)
{
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::SetTexture(EProgressBar_Texture_Type Type, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture[(int)Type] = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CProgressBar::SetTextureFullPath(EProgressBar_Texture_Type Type,
	const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	m_Texture[(int)Type] = m_Scene->GetSceneResource()->FindTexture(Name);
}

#ifdef UNICODE

void CProgressBar::SetTexture(EProgressBar_Texture_Type Type, const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture[(int)Type] = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CProgressBar::SetTextureFullPath(EProgressBar_Texture_Type Type, 
	const std::string& Name, const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture[(int)Type] = m_Scene->GetSceneResource()->FindTexture(Name);
}

#else

void CProgressBar::SetTexture(EProgressBar_Texture_Type Type, const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture[(int)Type] = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CProgressBar::SetTextureFullPath(EProgressBar_Texture_Type Type, 
	const std::string& Name, const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture[(int)Type] = m_Scene->GetSceneResource()->FindTexture(Name);
}

#endif // UNICODE

void CProgressBar::SetColorKey(EProgressBar_Texture_Type Type, 
	unsigned char r, unsigned char g, unsigned char b)
{
	if (m_Texture[(int)Type])
		m_Texture[(int)Type]->SetColorKey(r, g, b);
}

bool CProgressBar::Init()
{
	return true;
}

void CProgressBar::Update(float DeltaTime)
{
	m_BarSize = m_Size;
	m_BarPos = m_Pos;

	// 막대가 채워지는 방향에 따라 막대의 크기와 위치를 조절한다.
	switch (m_Dir)
	{
	case EProgressBar_Dir::LeftToRight:
		m_BarSize.x = m_Value * m_Size.x;
		break;
	case EProgressBar_Dir::RightToLeft:
		m_BarSize.x = m_Value * m_Size.x;
		m_BarPos.x = m_Pos.x + (1.f - m_Value) * m_Size.x;
		break;
	case EProgressBar_Dir::BottomToTop:
		m_BarSize.y = m_Value * m_Size.y;
		m_BarPos.y = m_Pos.y + (1.f - m_Value) * m_Size.y;
		break;
	case EProgressBar_Dir::TopToBottom:
		m_BarSize.y = m_Value * m_Size.y;
		break;
	case EProgressBar_Dir::End:
		break;
	}
}

void CProgressBar::PostUpdate(float DeltaTime)
{
}

void CProgressBar::Render(HDC hDC, float DeltaTime)
{
	Vector2 RenderPos = m_BarPos + m_Owner->GetPos();

	for (int i = 0; i < (int)EProgressBar_Texture_Type::End; ++i)
	{
		// 텍스처가 있으면 해당 텍스처 크기로 출력한다.
		if (m_Texture[i])
		{
			if (m_Texture[i]->GetTextureType() == ETexture_Type::Sprite)
			{
				// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
				if (m_Texture[i]->GetEnableColorKey())
				{
					TransparentBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
						(int)m_BarSize.x, (int)m_BarSize.y,
						m_Texture[i]->GetDC(),
						0, 0, (int)m_BarSize.x, (int)m_BarSize.y,
						m_Texture[i]->GetColorKey());
				}
				// 일반 비트맵 출력 함수
				else
				{
					BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
						(int)m_BarSize.x, (int)m_BarSize.y,
						m_Texture[i]->GetDC(),
						0, 0, SRCCOPY);
				}
			}
			else
			{
			}
		}
	}
}

void CProgressBar::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	Vector2 RenderPos = Pos;

	for (int i = 0; i < (int)EProgressBar_Texture_Type::End; ++i)
	{
		// 텍스처가 있으면 해당 텍스처 크기로 출력한다.
		if (m_Texture[i])
		{
			if (m_Texture[i]->GetTextureType() == ETexture_Type::Sprite)
			{
				// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
				if (m_Texture[i]->GetEnableColorKey())
				{
					TransparentBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
						(int)m_BarSize.x, (int)m_BarSize.y,
						m_Texture[i]->GetDC(),
						0, 0, (int)m_BarSize.x, (int)m_BarSize.y,
						m_Texture[i]->GetColorKey());
				}
				// 일반 비트맵 출력 함수
				else
				{
					BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
						(int)m_BarSize.x, (int)m_BarSize.y,
						m_Texture[i]->GetDC(),
						0, 0, SRCCOPY);
				}
			}
			else
			{
			}
		}
	}
}