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
	int m_PlayerDir;				// 플레이어가 바라보는 방향(1이면 오른쪽, -1이면 왼쪽)
	int m_PrevDir;					// 한 프레임 이전 방향
	std::vector<std::string> m_vecSequenceKey[2];	// 애니메이션 순서를 위한 키값 배열

	bool m_Attack;					// 플레이어가 공격 중인지 여부
	bool m_Idle;
	bool m_Walk;
	bool m_MoveBack;
	bool m_MoveFront;
	bool m_CrouchStart;				// 플레이어가 선 자세에서 앉은 자세로 전환 중인지
	bool m_CrouchStay;				// 플레이어가 앉은 상태를 유지하는지
	bool m_CrouchToStand;			// 플레이어가 앉은 자세에서 선 자세로 전환 중인지
	bool m_PrevState;
	bool m_Land;
	bool m_Movable;
	bool m_Jumpable;
	bool m_JumpVoiceReady;
	bool m_Block;

	//std::vector<SkillCoolDownInfo> m_vecCoolDown;	// 스킬 쿨타임 정보
	int m_HP;						// 플레이어 현재 체력
	int m_HPMax;					// 플레이어 최대 체력

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
	// 키 입력에 따라 실행할 함수들
	// (함수 포인터로 호출하므로 private이든 public이든 상관없음)
	void MoveFront();
	void Crouch();
	void CrouchStay();
	void CrouchEnd();
	void GunRotation();
	void GunRotationInv();
	void JumpKey();
	void JumpKeyUp();

private:
	// 애니메이션 Notify
	void Attack();
	void WalkSound();

	// 애니메이션EndFunction
	void AttackEnd();
	void ReturnToRightIdle();
	void ReturnToLeftIdle();
	void MoveStop();
	void CrouchToIdle();
	void Land();

private:
	// 충돌 시 호출될 함수
	void Push(CCollider* Src, CCollider* Dest);
	void PushEnd(CCollider* Src, CCollider* Dest);
	void CollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
};

