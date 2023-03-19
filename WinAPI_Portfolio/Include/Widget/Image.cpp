#include "Image.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"

CImage::CImage()
{
}

CImage::CImage(const CImage& widget) :
	CWidget(widget)
{
}

CImage::~CImage()
{
}

void CImage::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#ifdef UNICODE

void CImage::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CImage::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#else

void CImage::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CImage::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#endif // UNICODE

void CImage::SetColorKey(unsigned char r, unsigned char g, unsigned char b)
{
	if(m_Texture)
		m_Texture->SetColorKey(r, g, b);
}

bool CImage::Init()
{
	return true;
}

void CImage::Update(float DeltaTime)
{
}

void CImage::PostUpdate(float DeltaTime)
{
}

void CImage::Render(HDC hDC, float DeltaTime)
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
					0, 0, (int)m_Size.x, (int)m_Size.y,
					m_Texture->GetColorKey());
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					0, 0, SRCCOPY);
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
					0, 0, (int)m_Size.x, (int)m_Size.y,
					m_Texture->GetColorKey());
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(), 0, 0, SRCCOPY);
			}
		}
	}
	else
	{
		Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y,
			(int)(RenderPos.x + m_Size.x), (int)(RenderPos.y + m_Size.y));
	}
}

void CImage::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
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
					0, 0, (int)m_Size.x, (int)m_Size.y,
					m_Texture->GetColorKey());
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(),
					0, 0, SRCCOPY);
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
					0, 0, (int)m_Size.x, (int)m_Size.y,
					m_Texture->GetColorKey());
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderPos.x, (int)RenderPos.y,
					(int)m_Size.x, (int)m_Size.y,
					m_Texture->GetDC(), 0, 0, SRCCOPY);
			}
		}
	}
	else
	{
		Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y,
			(int)(RenderPos.x + m_Size.x), (int)(RenderPos.y + m_Size.y));
	}
}