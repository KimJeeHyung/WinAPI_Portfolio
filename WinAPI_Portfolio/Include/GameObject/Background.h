#pragma once
#include "GameObject.h"
class CBackground :
    public CGameObject
{
	friend class CScene;

protected:
	CBackground();
	CBackground(const CBackground& Obj);
	virtual ~CBackground();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

