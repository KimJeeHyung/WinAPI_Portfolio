#pragma once
#include "../GameInfo.h"

class CTile
{
	friend class CTileMap;

private:
	CTile();
	~CTile();

private:
	class CScene*	m_Scene;		// �� Ÿ���� ���� ��
	class CTileMap* m_Owner;		// �� Ÿ���� ���� �ִ� Ÿ�ϸ�
	CSharedPtr<class CTexture> m_Texture;
	ETile_Option	m_Option;		// Ÿ���� �Ӽ�
	Vector2			m_Pos;
	Vector2			m_Size;
	int				m_IndexX;			// ���� �ε���(���� �� ��°����)
	int				m_IndexY;			// ���� �ε���(���� �� ��°����)
	int				m_Index;			// ���� �ε���(Ÿ�ϸʿ��� �� ��°����)
	int				m_TileFrame;		// Ÿ�� �ִϸ��̼ǿ� ����� ������ ����
	Vector2			m_StartFrame;		// Ÿ�� �ִϸ��̼ǿ� ����� ���� ��ġ
	Vector2			m_EndFrame;			// Ÿ�� �ִϸ��̼ǿ� ����� �� ��ġ
	bool			m_Render;			// Ÿ���� ������� ����
	bool			m_SideCollision;	// Ÿ���� �浹�ϰ� ���� ����

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

