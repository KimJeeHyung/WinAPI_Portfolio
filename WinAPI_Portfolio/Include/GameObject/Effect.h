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
	EEffect_Type	m_EffectType;	//����Ʈ ����(��ȸ��, ���ӽð�, ����)
	float			m_Duration;		// ����Ʈ ���ӽð�
	float			m_Time;			// ���ӽð� üũ�� ���� Ÿ�̸�

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

