#pragma once

#include "../GameInfo.h"
#include "../SingletonMacro.h"

class CSceneManager
{
private:
	class CScene* m_Scene;		// 현재 씬
	class CScene* m_NextScene;	// 씬 전환을 위한 다음 씬

public:
	class CScene* GetScene() const
	{
		return m_Scene;
	}

public:
	bool Init();
	// 반환타입을 bool로 하는 이유는 Scene이 교체되는지 아닌지를
	// 판단하기 위해서이다.
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
		// 이미 씬이 있으면 다음 씬에 만들어둔다.
		else
			m_NextScene = (CScene*)Scene;

		return true;
	}

	DECLARE_SINGLE(CSceneManager)
};

