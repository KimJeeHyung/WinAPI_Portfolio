#pragma once

#include "../GameInfo.h"

// Notify : 애니메이션의 특정 부분(프레임)에서 호출할 함수를 등록하는 시스템
struct AnimationNotify
{
	int Frame;	// 애니메이션의 몇번째 프레임에 등록할지
	bool Call;	// 함수가 호출됐는지 여부(중복 호출 방지)
	std::function<void()> Function;	// 등록할 함수

	AnimationNotify() :
		Frame(0),
		Call(false)
	{
	}
};

/*
애니메이션 시퀀스를 하나 가지고 있으면서 해당 애니메이션을 총 몇 초에 걸쳐서 한번
재생을 할 것인지, 이 애니메이션이 계속 반복해서 재생하는지 등을 정의해두는 객체이다.
*/

class CAnimationInfo
{
	friend class CAnimation;
	friend class CGameObject;

private:
	CAnimationInfo();
	~CAnimationInfo();

private:
	CSharedPtr<class CAnimationSequence> m_Sequence;
	int		m_Frame;		// 현재 애니메이션의 프레임
	float	m_Time;			// 애니메이션 프레임을 증가시켜주기 위한 시간값
	float	m_FrameTime;	// 한 프레임이 증가하기 위한 시간
	float	m_PlayTime;		// 모션 한번이 동작되기 위한 시간
	float	m_PlayScale;	// 재생속도 배율
	bool	m_Loop;			// 반복해서 재생해야 하는지 여부
	bool	m_Reverse;		// 역재생 여부
	std::function<void()> m_EndFunction;	// 애니메이션이 끝날 때마다 호출할 함수 등록
	std::vector<AnimationNotify*> m_vecNotify;	// 애니메이션 노티파이 배열

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	// 애니메이션의 Frame번째 프레임에서 Obj객체의 Func함수를 호출하도록
	// 노티파이를 추가한다.
	template <typename T>
	void AddNotify(int Frame, T* Obj, void(T::* Func)())
	{
		AnimationNotify* Notify = new AnimationNotify;

		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}
};

