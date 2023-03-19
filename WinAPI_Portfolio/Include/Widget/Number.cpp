#include "Number.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"

CNumber::CNumber() :
	m_Number(0),
	m_Zero(false)
{
}

CNumber::CNumber(const CNumber& widget) :
	CWidget(widget)
{
}

CNumber::~CNumber()
{
}

void CNumber::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CNumber::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#ifdef UNICODE

void CNumber::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CNumber::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#else

void CNumber::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CNumber::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

#endif // UNICODE

void CNumber::SetColorKey(unsigned char r, unsigned char g, unsigned char b)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b);
}

bool CNumber::Init()
{
	return true;
}

void CNumber::Update(float DeltaTime)
{
	if (m_Number == 0)
	{
		m_vecNumber.clear();
		m_vecNumber.push_back(0);
		m_vecNumber.push_back(0);
	}
	else
	{
		// 숫자를 각 자리별로 분할한다.
		int Number = m_Number;

		std::stack<int> NumberStack;

		/*
		123 % 10 = 3	3 push		123 / 10 = 12	
		12 % 10 = 2		2 push		12 / 10 = 1		
		1 % 10 = 1		1 push		1 / 10 = 0		
		이런 식으로 일의 자리 숫자부터 차례로 스택에 들어가게 된다.
		*/
		while (Number > 0)
		{
			NumberStack.push(Number % 10);
			// 원본 숫자가 한자리수일 때 앞에 0을 붙여준다.(m_Zero에 따라 붙이지 않게 할 수도 있음)
			if (m_Number < 10 && m_Zero)
				NumberStack.push(0);
			Number /= 10;
		}

		m_vecNumber.clear();

		while (!NumberStack.empty())
		{
			// 스택에서 꺼낼 땐 반대로 나오므로 맨 앞자리 숫자부터 차례로
			// 배열에 들어가게 된다.
			m_vecNumber.push_back(NumberStack.top());
			NumberStack.pop();
		}
	}
}

void CNumber::PostUpdate(float DeltaTime)
{
}

void CNumber::Render(HDC hDC, float DeltaTime)
{
	if (!m_vecNumber.empty())
	{
		Vector2 RenderPos = m_Pos + m_Owner->GetPos();

		// 텍스처가 있으면 해당 텍스처 크기로 출력한다.
		if (m_Texture)
		{
			size_t Size = m_vecNumber.size();

			// 배열에 있는 숫자를 모두 출력해야 하므로 배열의 크기만큼 반복한다.
			for (size_t i = 0; i < Size; i++)
			{
				// 스프라이트 텍스처
				if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
				{
					// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
					if (m_Texture->GetEnableColorKey())
					{
						// 숫자 텍스처를 쓸 때는 반드시 0 ~ 9를 가로로 이어붙인 것을 써야 한다.
						// 숫자가 2자리수 이상일 때 겹쳐서 출력되지 않도록 텍스처 크기만큼 x값을 더해준다.
						TransparentBlt(hDC, (int)(RenderPos.x + i * m_Size.x),
							(int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(),
							(int)(m_vecNumber[i] * m_Size.x), 0,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetColorKey());
					}
					// 일반 비트맵 출력 함수
					else
					{
						BitBlt(hDC, (int)(RenderPos.x + i * m_Size.x), (int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(),
							(int)(m_vecNumber[i] * m_Size.x), 0, SRCCOPY);
					}
				}
				// 프레임 텍스처
				else
				{
					// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
					if (m_Texture->GetEnableColorKey())
					{
						TransparentBlt(hDC, (int)(RenderPos.x + i * m_Size.x),
							(int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(m_vecNumber[i]),
							0, 0, (int)m_Size.x, (int)m_Size.y,
							m_Texture->GetColorKey());
					}
					// 일반 비트맵 출력 함수
					else
					{
						BitBlt(hDC, (int)(RenderPos.x + i * m_Size.x), (int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(m_vecNumber[i]), 0, 0, SRCCOPY);
					}
				}
			}
		}
	}
}

void CNumber::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	if (!m_vecNumber.empty())
	{
		Vector2 RenderPos = Pos;

		// 텍스처가 있으면 해당 텍스처 크기로 출력한다.
		if (m_Texture)
		{
			size_t Size = m_vecNumber.size();

			// 배열에 있는 숫자를 모두 출력해야 하므로 배열의 크기만큼 반복한다.
			for (size_t i = 0; i < Size; i++)
			{
				// 스프라이트 텍스처
				if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
				{
					// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
					if (m_Texture->GetEnableColorKey())
					{
						// 숫자 텍스처를 쓸 때는 반드시 0 ~ 9를 가로로 이어붙인 것을 써야 한다.
						// 숫자가 2자리수 이상일 때 겹쳐서 출력되지 않도록 텍스처 크기만큼 x값을 더해준다.
						TransparentBlt(hDC, (int)(RenderPos.x + i * m_Size.x),
							(int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(),
							(int)(m_vecNumber[i] * m_Size.x), 0,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetColorKey());
					}
					// 일반 비트맵 출력 함수
					else
					{
						BitBlt(hDC, (int)(RenderPos.x + i * m_Size.x), (int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(),
							(int)(m_vecNumber[i] * m_Size.x), 0, SRCCOPY);
					}
				}
				// 프레임 텍스처
				else
				{
					// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
					if (m_Texture->GetEnableColorKey())
					{
						TransparentBlt(hDC, (int)(RenderPos.x + i * m_Size.x),
							(int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(m_vecNumber[i]),
							0, 0, (int)m_Size.x, (int)m_Size.y,
							m_Texture->GetColorKey());
					}
					// 일반 비트맵 출력 함수
					else
					{
						BitBlt(hDC, (int)(RenderPos.x + i * m_Size.x), (int)RenderPos.y,
							(int)m_Size.x, (int)m_Size.y,
							m_Texture->GetDC(m_vecNumber[i]), 0, 0, SRCCOPY);
					}
				}
			}
		}
	}
}