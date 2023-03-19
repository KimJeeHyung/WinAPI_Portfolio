#pragma once
#include "Character.h"
class COpponent :
    public CCharacter
{
	friend class CScene;

protected:
	COpponent();
	COpponent(const COpponent& Obj);
	virtual ~COpponent();

private:
	int m_PlayerDir;				// �÷��̾ �ٶ󺸴� ����(1�̸� ������, -1�̸� ����)
	int m_PrevDir;					// �� ������ ���� ����
	std::vector<std::string> m_vecSequenceKey[2];	// �ִϸ��̼� ������ ���� Ű�� �迭

	bool m_Attack;					// �÷��̾ ���� ������ ����
	bool m_Idle;
	bool m_Walk;
	bool m_MoveBack;
	bool m_MoveFront;
	bool m_CrouchStart;				// �÷��̾ �� �ڼ����� ���� �ڼ��� ��ȯ ������
	bool m_CrouchStay;				// �÷��̾ ���� ���¸� �����ϴ���
	bool m_CrouchToStand;			// �÷��̾ ���� �ڼ����� �� �ڼ��� ��ȯ ������
	bool m_PrevState;
	bool m_Land;
	bool m_Movable;
	bool m_Jumpable;
	bool m_JumpVoiceReady;
	bool m_Block;

	//std::vector<SkillCoolDownInfo> m_vecCoolDown;	// ��ų ��Ÿ�� ����
	int m_HP;						// �÷��̾� ���� ü��
	int m_HPMax;					// �÷��̾� �ִ� ü��

	class CColliderBox* m_Body;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual float InflictDamage(float Damage);

private:
	void InitAnimation();

private:
	// Ű �Է¿� ���� ������ �Լ���
	// (�Լ� �����ͷ� ȣ���ϹǷ� private�̵� public�̵� �������)
	void MoveFront();
	void Crouch();
	void CrouchStay();
	void CrouchEnd();
	void GunRotation();
	void GunRotationInv();
	void JumpKey();
	void JumpKeyUp();

private:
	// �ִϸ��̼� Notify
	void Attack();
	void WalkSound();

	// �ִϸ��̼�EndFunction
	void AttackEnd();
	void ReturnToRightIdle();
	void ReturnToLeftIdle();
	void MoveStop();
	void CrouchToIdle();
	void Land();

private:
	// �浹 �� ȣ��� �Լ�
	void Push(CCollider* Src, CCollider* Dest);
	void PushEnd(CCollider* Src, CCollider* Dest);
	void CollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
};

