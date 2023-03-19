#pragma once

#include "../GameInfo.h"

// �����Ϳ��� ������ Ÿ�� ������
enum class ETile_Brush
{
	Frame,	// Ÿ�� �ؽ�ó�� ������
	Option	// Ÿ���� �ɼ�(������ �� �ִ��� ������)
};

/*Ÿ�ϸ� ������ ���� ������ ���̾�α�*/
class CEditDlg
{
	friend class CEditScene;

public:
	CEditDlg();
	~CEditDlg();

private:
	class CEditScene*	m_Scene;					// �� ���̾�α׸� ���� �ִ� ������ ��
	HWND				m_hDlg;						// ���̾�α� �������� �ڵ鷯
	HWND				m_hTextureListBox;			// �ؽ�ó �̸��� �����ִ� ����Ʈ�ڽ��� �ڵ鷯
	HWND				m_hTextureFrameListBox;		// �ؽ�ó ������ ������ �����ִ� ����Ʈ�ڽ��� �ڵ鷯
	HWND				m_hBrushTypeCombo;			// ������ Ÿ�� �����͸� �����ϴ� �޺��ڽ��� �ڵ鷯
	HWND				m_hOptionCombo;				// Ÿ���� �ɼ��� �����ϴ� �޺��ڽ��� �ڵ鷯
	HWND				m_hRenderCheckBox;			// Ÿ���� ������� �����ϴ� üũ�ڽ��� �ڵ鷯
	HWND				m_hSideCollisionCheckBox;	// ������ �� �ִ� Ÿ������ �����ϴ� üũ�ڽ��� �ڵ鷯
	int					m_SelectTextureIndex;		// �ؽ�ó ����Ʈ�ڽ����� ���� ���õ� ���� �ε���
	int					m_SelectTextureFrameIndex;	// ������ ����Ʈ�ڽ����� ���� ���õ� ���� �ε���
	TCHAR				m_SelectTextureName[128];	// ����Ʈ�ڽ����� ���� �ؽ�ó �̸��� �����ϴ� ���ڿ�
	CSharedPtr<class CTexture> m_SelectTexture;
	std::vector<std::vector<AnimationFrameData>> m_vecTextureFrame;

	ETile_Brush		m_BrushType;
	ETile_Option	m_SelectOption;

public:
	bool GetCheckRender() const
	{
		// Ÿ�� ��� üũ�ڽ��� üũ�Ǿ� �ִ��� Ȯ���ϰ� �� ����� �����Ѵ�.
		return SendMessage(m_hRenderCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	bool GetCheckSideCollision() const
	{
		// Ÿ�� �浹 üũ�ڽ��� üũ�Ǿ� �ִ��� Ȯ���ϰ� �� ����� �����Ѵ�.
		return SendMessage(m_hSideCollisionCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	ETile_Brush GetBrushType() const
	{
		return m_BrushType;
	}

	ETile_Option GetSelectOption() const
	{
		return m_SelectOption;
	}

	const AnimationFrameData* GetTileFrameData() const
	{
		// �ؽ�ó �Ǵ� �������� ���õ��� �ʾ����� nullptr����
		if (m_SelectTextureIndex == -1 || m_SelectTextureFrameIndex == -1)
			return nullptr;

		return &m_vecTextureFrame[m_SelectTextureIndex][m_SelectTextureFrameIndex];
	}

public:
	bool Init();
	void CreateTileMap();
	void LoadTexture();
	void SelectTexture();
	void AddFrame();
	void SelectFrame();
	void SelectBrushType();
	void SelectOption();
	void Save();
	void Load();
	void LoadTextureBack();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

