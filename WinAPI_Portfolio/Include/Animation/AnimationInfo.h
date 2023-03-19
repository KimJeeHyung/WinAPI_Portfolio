#pragma once

#include "../GameInfo.h"

// Notify : �ִϸ��̼��� Ư�� �κ�(������)���� ȣ���� �Լ��� ����ϴ� �ý���
struct AnimationNotify
{
	int Frame;	// �ִϸ��̼��� ���° �����ӿ� �������
	bool Call;	// �Լ��� ȣ��ƴ��� ����(�ߺ� ȣ�� ����)
	std::function<void()> Function;	// ����� �Լ�

	AnimationNotify() :
		Frame(0),
		Call(false)
	{
	}
};

/*
�ִϸ��̼� �������� �ϳ� ������ �����鼭 �ش� �ִϸ��̼��� �� �� �ʿ� ���ļ� �ѹ�
����� �� ������, �� �ִϸ��̼��� ��� �ݺ��ؼ� ����ϴ��� ���� �����صδ� ��ü�̴�.
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
	int		m_Frame;		// ���� �ִϸ��̼��� ������
	float	m_Time;			// �ִϸ��̼� �������� ���������ֱ� ���� �ð���
	float	m_FrameTime;	// �� �������� �����ϱ� ���� �ð�
	float	m_PlayTime;		// ��� �ѹ��� ���۵Ǳ� ���� �ð�
	float	m_PlayScale;	// ����ӵ� ����
	bool	m_Loop;			// �ݺ��ؼ� ����ؾ� �ϴ��� ����
	bool	m_Reverse;		// ����� ����
	std::function<void()> m_EndFunction;	// �ִϸ��̼��� ���� ������ ȣ���� �Լ� ���
	std::vector<AnimationNotify*> m_vecNotify;	// �ִϸ��̼� ��Ƽ���� �迭

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	// �ִϸ��̼��� Frame��° �����ӿ��� Obj��ü�� Func�Լ��� ȣ���ϵ���
	// ��Ƽ���̸� �߰��Ѵ�.
	template <typename T>
	void AddNotify(int Frame, T* Obj, void(T::* Func)())
	{
		AnimationNotify* Notify = new AnimationNotify;

		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}
};

