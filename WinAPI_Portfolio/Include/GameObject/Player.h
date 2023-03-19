#pragma once
#include "Character.h"

struct SkillCoolDownInfo
{
	float	CoolDown;		// 쿨타임 시간
	bool	CoolDownEnable;	// 쿨타임 중인지 체크
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
	int m_PlayerDir;				// 플레이어가 바라보는 방향(1이면 오른쪽, -1이면 왼쪽)
	int m_PrevDir;					// 한 프레임 이전 방향
	std::vector<std::string> m_vecSequenceKey[2];	// 애니메이션 순서를 위한 키값 배열
	
	bool m_Attack;					// 플레이어가 공격 중인지 여부
	bool m_AttackCancel;			// 공격 중간에 캔슬하고 다른 기술을 쓸 수 있는지 여부
	bool m_Idle;					// 기본 상태
	bool m_Walk;					// 걷는 중인지
	bool m_MoveBack;				// 뒤로 걷는 중인지
	bool m_MoveFront;				// 앞으로 걷는 중인지
	bool m_CrouchStart;				// 선 자세에서 앉은 자세로 전환 중인지
	bool m_CrouchStay;				// 앉은 상태를 유지하는지
	bool m_CrouchToStand;			// 앉은 자세에서 선 자세로 전환 중인지
	bool m_Land;					// 착지 중인지
	bool m_Movable;					// 좌우 이동이 가능한지
	bool m_Jumpable;				// 점프할 수 있는지
	bool m_JumpVoiceReady;			// 점프 음성을 재생할 수 있는지
	bool m_Block;					// 상대가 앞을 막고 있는지
	bool m_MultiHit;

	std::vector<SkillCoolDownInfo> m_vecCoolDown;	// 스킬 쿨타임 정보
	int m_HP;						// 플레이어 현재 체력
	int m_HPMax;					// 플레이어 최대 체력

	class CColliderBox* m_Body;		// 몸통 충돌체

	int m_AttackLevel;				// 공격 단계(효과음, 음성 구분 용)

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
	// 애니메이션 Notify
	void AttackCollision();
	void AttackCollisionOff();
	void WalkSound();
	void RagnaSE();
	void RagnaVoice();
	
	// 애니메이션EndFunction
	void AttackEnd();
	void MoveStop();
	void CrouchToIdle();
	void Land();

private:
	// 충돌 시 호출될 함수
	void Push(CCollider* Src, CCollider* Dest);
	void PushEnd(CCollider* Src, CCollider* Dest);
	void AttackCollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
};

