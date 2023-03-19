#pragma once

#include "../GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	std::vector<class CCollider*>		m_vecCollider;
	std::vector<class CWidgetWindow*>	m_vecWidgetWindow;
	class CCollider*	m_MouseCollision;
	class CWidget*		m_MouseCollisionWidget;

public:
	void AddCollider(class CCollider* Collider);
	void AddWidgetWindow(class CWidgetWindow* Window);
	void CollisionMouse(float DeltaTime);
	void Collision(float DeltaTime);

private:
	// 겹쳐있는 오브젝트들과 마우스 충돌 시 제일 위에 그려진 오브젝트하고만
	// 충돌을 해야 하므로 Y소팅을 위한 함수가 필요하다.
	static bool SortY(class CCollider* Src, class CCollider* Dest);
	static bool SortWidget(class CWidgetWindow* Src, class CWidgetWindow* Dest);
};

