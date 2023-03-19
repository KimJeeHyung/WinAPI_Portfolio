#pragma once

#include "AnimationInfo.h"

class CAnimation
{
	friend class CGameObject;

private:
	CAnimation();
	~CAnimation();

private:
	class CGameObject* m_Owner;	// 자신을 갖고 있는 게임오브젝트
	class CScene* m_Scene;		// 자신이 속해있는 씬
	std::unordered_map<std::string, CAnimationInfo*> m_mapAnimation;	// 애니메이션 목록
	CAnimationInfo* m_CurrentAnimation;	// 현재 재생되고 있는 애니메이션
	std::function<void()> m_CurrentAnimationEndFunc; // 애니메이션이 끝날 때 호출될 함수

public:
	void AddAnimation(const std::string& SequenceName, bool Loop = true,
		float PlayTime = 1.f, float PlayScale = 1.f, bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetPlayLoop(const std::string& Name, bool Loop);
	void SetPlayReverse(const std::string& Name, bool Reverse);
	void SetCurrentAniamtion(std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void Update(float DeltaTime);

public:
	void ResetAnimation(const std::string& Name);

private:
	CAnimationInfo* FindInfo(const std::string& Name);

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void(T::* Func)())
	{
		CAnimationInfo* Info = FindInfo(Name);

		if (!Info)
			return;

		Info->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)())
	{
		CAnimationInfo* Info = FindInfo(Name);

		if (!Info)
			return;

		Info->AddNotify<T>(Frame, Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)(int))
	{
		CAnimationInfo* Info = FindInfo(Name);

		if (!Info)
			return;

		Info->AddNotify<T>(Frame, Obj, Func, std::placeholders::_1);
	}

	template <typename T>
	void SetCurrentAnimationEndFunction(T* Obj, void(T::* Func)())
	{
		m_CurrentAnimationEndFunc = std::bind(Func, Obj);
	}
};

