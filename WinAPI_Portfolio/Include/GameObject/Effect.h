#pragma once
#include "GameObject.h"
class CEffect :
    public CGameObject
{
	friend class CScene;

protected:
	CEffect();
	CEffect(const CEffect& Obj);
	virtual ~CEffect();

private:
	EEffect_Type	m_EffectType;	//이펙트 종류(일회성, 지속시간, 루프)
	float			m_Duration;		// 이펙트 지속시간
	float			m_Time;			// 지속시간 체크를 위한 타이머

public:
	EEffect_Type GetEffectType() const
	{
		return m_EffectType;
	}

	void SetDuration(float Duration)
	{
		m_Duration = Duration;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

private:
	void AnimationEnd();
};

