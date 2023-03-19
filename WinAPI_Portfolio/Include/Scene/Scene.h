#pragma once

#include "../GameInfo.h"
#include "../Widget/WidgetWindow.h"
#include "../Widget/WidgetComponent.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

private:
	class CSceneResource*	m_Resource;
	class CCamera*			m_Camera;
	class CSceneCollision*	m_Collision;

protected:
	std::list<CSharedPtr<class CGameObject>> m_ObjList[(int)ERender_Layer::Max];	// 씬에 배치할 오브젝트 목록
	std::list<CSharedPtr<CWidgetComponent>> m_WidgetComponentList;		// 출력을 위한 위젯 컴포넌트 목록
	CSharedPtr<class CGameObject>	m_Player;
	CSharedPtr<class CTileMap>		m_TileMap;

	std::vector<CSharedPtr<CWidgetWindow>> m_vecWidgetWindow;	// 씬에 배치할 위젯(UI) 목록

public:
	class CSceneCollision* GetCollision() const
	{
		return m_Collision;
	}

	class CCamera* GetCamera() const
	{
		return m_Camera;
	}

	class CSceneResource* GetSceneResource() const
	{
		return m_Resource;
	}

	class CGameObject* GetPlayer() const
	{
		return m_Player;
	}

	class CTileMap* GetTileMap() const
	{
		return m_TileMap;
	}

	void SetTileMap(class CTileMap* TileMap);
	void SetPlayer(class CGameObject* Player);
	void AddWidgetComponent(CWidgetComponent* Widget)
	{
		m_WidgetComponentList.push_back(Widget);
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

public:
	template <typename T>
	T* CreateObject(const std::string& Name = "GameObject")
	{
		T* Obj = new T;

		Obj->SetName(Name);
		// 생성한 오브젝트에 자신이 소속된 씬을 알려준다.
		Obj->m_Scene = this;

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_ObjList[(int)Obj->GetRenderLayer()].push_back((CGameObject*)Obj);

		return Obj;
	}

	template <typename T>
	T* CreateWidgetWindow(const std::string& Name = "Window")
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->m_Scene = this;

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_vecWidgetWindow.push_back(Window);

		return (T*)Window;
	}

	template <typename T>
	T* FindWidget(const std::string& Name)
	{
		size_t Size = m_vecWidgetWindow.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (m_vecWidgetWindow[i]->GetName() == Name)
				return (T*)m_vecWidgetWindow[i].Get();
		}

		return nullptr;
	}

private:
	// 씬에 있는 오브젝트들의 Y소팅 시 정렬 기준을 정해주는 함수
	// (전역함수처럼 쓸 수 있도록 static으로 선언)
	static bool SortY(const CSharedPtr<class CGameObject>& Src,
		const CSharedPtr<class CGameObject>& Dest);
	static bool SortYWidgetComponent(const CSharedPtr<class CWidgetComponent>& Src,
		const CSharedPtr<class CWidgetComponent>& Dest);
	static bool SortWiget(const CSharedPtr<class CWidgetWindow>& Src,
		const CSharedPtr<class CWidgetWindow>& Dest);
};

