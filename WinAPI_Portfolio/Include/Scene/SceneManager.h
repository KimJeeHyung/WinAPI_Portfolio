#pragma once

#include "../GameInfo.h"
#include "../SingletonMacro.h"

class CSceneManager
{
private:
	class CScene* m_Scene;		// ���� ��
	class CScene* m_NextScene;	// �� ��ȯ�� ���� ���� ��

public:
	class CScene* GetScene() const
	{
		return m_Scene;
	}

public:
	bool Init();
	// ��ȯŸ���� bool�� �ϴ� ������ Scene�� ��ü�Ǵ��� �ƴ�����
	// �Ǵ��ϱ� ���ؼ��̴�.
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);

private:
	bool ChangeScene();

public:
	template <typename T>
	bool CreateScene()
	{
		T* Scene = new T;

		if (!Scene->Init())
		{
			SAFE_DELETE(Scene);
			return false;
		}

		if (!m_Scene)
			m_Scene = (CScene*)Scene;
		// �̹� ���� ������ ���� ���� �����д�.
		else
			m_NextScene = (CScene*)Scene;

		return true;
	}

	DECLARE_SINGLE(CSceneManager)
};

