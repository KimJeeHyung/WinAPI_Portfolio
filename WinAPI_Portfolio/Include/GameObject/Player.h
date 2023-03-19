#pragma once
#include "Character.h"

struct SkillCoolDownInfo
{
	float	CoolDown;		// ��Ÿ�� �ð�
	bool	CoolDownEnable;	// ��Ÿ�� ������ üũ
};

enum class EPlayerKey
{
	A,
	B
};

class CPlayer :
    public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();

private:
	int m_PlayerDir;				// �÷��̾ �ٶ󺸴� ����(1�̸� ������, -1�̸� ����)
	int m_PrevDir;					// �� ������ ���� ����
	std::vector<std::string> m_vecSequenceKey[2];	// �ִϸ��̼� ������ ���� Ű�� �迭
	
	bool m_Attack;					// �÷��̾ ���� ������ ����
	bool m_AttackCancel;			// ���� �߰��� ĵ���ϰ� �ٸ� ����� �� �� �ִ��� ����
	bool m_Idle;					// �⺻ ����
	bool m_Walk;					// �ȴ� ������
	bool m_MoveBack;				// �ڷ� �ȴ� ������
	bool m_MoveFront;				// ������ �ȴ� ������
	bool m_CrouchStart;				// �� �ڼ����� ���� �ڼ��� ��ȯ ������
	bool m_CrouchStay;				// ���� ���¸� �����ϴ���
	bool m_CrouchToStand;			// ���� �ڼ����� �� �ڼ��� ��ȯ ������
	bool m_Land;					// ���� ������
	bool m_Movable;					// �¿� �̵��� ��������
	bool m_Jumpable;				// ������ �� �ִ���
	bool m_JumpVoiceReady;			// ���� ������ ����� �� �ִ���
	bool m_Block;					// ��밡 ���� ���� �ִ���
	bool m_MultiHit;

	std::vector<SkillCoolDownInfo> m_vecCoolDown;	// ��ų ��Ÿ�� ����
	int m_HP;						// �÷��̾� ���� ü��
	int m_HPMax;					// �÷��̾� �ִ� ü��

	class CColliderBox* m_Body;		// ���� �浹ü

	int m_AttackLevel;				// ���� �ܰ�(ȿ����, ���� ���� ��)

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
	void Crouch();
	void CrouchStay();
	void CrouchEnd();
	void MoveFront();
	void MoveBack();
	void JumpKey();
	void JumpKeyUp();
	bool Attack();
	void AttackA();
	void AttackB();
	void AttackC();
	void AttackD();

private:
	// �ִϸ��̼� Notify
	void AttackCollision();
	void AttackCollisionOff();
	void WalkSound();
	void RagnaSE();
	void RagnaVoice();
	
	// �ִϸ��̼�EndFunction
	void AttackEnd();
	void MoveStop();
	void CrouchToIdle();
	void Land();

private:
	// �浹 �� ȣ��� �Լ�
	void Push(CCollider* Src, CCollider* Dest);
	void PushEnd(CCollider* Src, CCollider* Dest);
	void AttackCollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
};

