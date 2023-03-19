#pragma once
#include "../GameInfo.h"

class CTile
{
	friend class CTileMap;

private:
	CTile();
	~CTile();

private:
	class CScene*	m_Scene;		// 이 타일이 속한 씬
	class CTileMap* m_Owner;		// 이 타일을 갖고 있는 타일맵
	CSharedPtr<class CTexture> m_Texture;
	ETile_Option	m_Option;		// 타일의 속성
	Vector2			m_Pos;
	Vector2			m_Size;
	int				m_IndexX;			// 가로 인덱스(가로 몇 번째인지)
	int				m_IndexY;			// 세로 인덱스(세로 몇 번째인지)
	int				m_Index;			// 실제 인덱스(타일맵에서 몇 번째인지)
	int				m_TileFrame;		// 타일 애니메이션에 사용할 프레임 정보
	Vector2			m_StartFrame;		// 타일 애니메이션에 사용할 시작 위치
	Vector2			m_EndFrame;			// 타일 애니메이션에 사용할 끝 위치
	bool			m_Render;			// 타일을 출력할지 여부
	bool			m_SideCollision;	// 타일을 충돌하게 할지 여부

public:
	bool GetSideCollision() const
	{
		return m_SideCollision;
	}

	ETile_Option GetOption() const
	{
		return m_Option;
	}

	const Vector2& GetPos() const
	{
		return m_Pos;
	}

	const Vector2& GetSize() const
	{
		return m_Size;
	}

public:
	void SetRender(bool Render)
	{
		m_Render = Render;
	}

	void SetSideCollision(bool Side)
	{
		m_SideCollision = Side;
	}

	void SetTileInfo(const Vector2& Pos, const Vector2& Size,
		int IndexX, int IndexY, int Index, class CTexture* Texture);
	void SetFrame(const Vector2& Start, const Vector2& End)
	{
		m_StartFrame = Start;
		m_EndFrame = End;
	}

	void SetTileOption(ETile_Option Option)
	{
		m_Option = Option;
	}

public:
	void Render(HDC hDC);
	void Save(FILE* File);
	void Load(FILE* File);
};

