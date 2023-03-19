#pragma once

#include "../GameObject/GameObject.h"

class CCamera
{
	friend class CScene;

private:
	CCamera();
	~CCamera();

private:
	Vector2 m_Pos;						// ī�޶� ��ġ
	Vector2 m_Resolution;				// ������ â�� �ػ�
	Vector2 m_WorldResolution;			// ��ü ������ �ػ�
	CSharedPtr<CGameObject> m_Target;	// ī�޶� ����ٴ� Ÿ��(�ݵ�� ������ �ʿ�� ����)
	CSharedPtr<CGameObject> m_Target2;	// ī�޶� ����ٴ� Ÿ��(�ݵ�� ������ �ʿ�� ����)
	Vector2 m_TargetPos;				// ī�޶� ����ٴ� Ÿ��(������Ʈ�� �ƴ� ��ġ��)
	Vector2 m_TargetOffset;				// Ÿ�����κ��� �󸶳� ������ ������
	Vector2 m_TargetPivot;				// Ÿ���� ȭ�鿡�� ��ũ�Ѹ��� ���� ����

	float	m_DefaultY;
	float	m_MaxDistance;
	Vector2 m_PrevTargetPos;

public:
	Vector2 GetPos() const
	{
		return m_Pos;
	}

	Vector2 GetResolution() const
	{
		return m_Resolution;
	}

	Vector2 GetWorldResolution() const
	{
		return m_WorldResolution;
	}

	CGameObject* GetTarget() const
	{
		return m_Target;
	}

	Vector2 GetTargetOffset() const
	{
		return m_TargetOffset;
	}

	Vector2 GetTargetPivot() const
	{
		return m_TargetPivot;
	}

public:
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetResolution(const Vector2& Resolution)
	{
		m_Resolution = Resolution;
	}

	void SetResolution(float x, float y)
	{
		m_Resolution = Vector2(x, y);
	}

	void SetWorldResolution(const Vector2& WorldResolution)
	{
		m_WorldResolution = WorldResolution;
	}

	void SetWorldResolution(float x, float y)
	{
		m_WorldResolution = Vector2(x, y);
	}

	void SetTargetOffset(const Vector2& TargetOffset)
	{
		m_TargetOffset = TargetOffset;
	}

	void SetTargetOffset(float x, float y)
	{
		m_TargetOffset = Vector2(x, y);
	}

	void SetTargetPivot(const Vector2& TargetPivot)
	{
		m_TargetPivot = TargetPivot;
	}

	void SetTargetPivot(float x, float y)
	{
		m_TargetPivot = Vector2(x, y);
	}

	void SetTargets(CGameObject* Target, CGameObject* Target2)
	{
		m_Target = Target;
		m_Target2 = Target2;
	}

	void SetTarget(const Vector2& TargetPos)
	{
		m_TargetPos = TargetPos;
	}

	void SetDefaultY(float y)
	{
		m_DefaultY = y;
	}

public:
	void Update(float DeltaTime);
};

