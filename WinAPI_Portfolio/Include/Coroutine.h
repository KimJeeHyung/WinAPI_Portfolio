#pragma once
#include <coroutine> // �ڷ�ƾ�� ����ϱ� ���� ���

class CCoroutine
{
public:
	// ��Ģ 1. C++���� ���ǵ� ��Ģ�� ������ promise_type�̶�� �̸��� Ÿ���� ���ǵǾ�� �Ѵ�.
	struct promise_type
	{
		// ����� ���� "�ڷ�ƾ ��ȯ ��ü"�� ��ȯ�Ѵ�.
		CCoroutine get_return_object()
		{
			return CCoroutine{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		// �ڷ�ƾ ���� ���� �� ȣ��. awaitable ��ü�� ��ȯ�Ѵ�.
		auto initial_suspend() { return std::suspend_always{}; }

		// co_return�� ����ϴ� ��� ����.
		void return_void() { return; }

		// �ڷ�ƾ ���� �� ȣ��.
		auto final_suspend() noexcept { return std::suspend_always{}; }

		// �ڷ�ƾ ���� �� ���� �߻� �� ȣ��.
		void unhandled_exception() { std::exit(1); }
	};

	// ��Ģ 2. std::coroutine_handle<promise_type> Ÿ���� ��� ������ �־�� �Ѵ�.
	std::coroutine_handle<promise_type> co_handler;

	// ��Ģ 3. std::coroutine_handle<promise_type> �� ���ڷ� �޾�
	// ��� ������ �ʱ�ȭ�ϴ� �����ڰ� �־�� �Ѵ�.
	CCoroutine(std::coroutine_handle<promise_type> handler) : co_handler(handler) {}

	// ��Ģ 4. �Ҹ��ڿ��� std::coroutine_handle<promise_type> Ÿ����
	// �ڷ�ƾ �ڵ鷯 ��� ������ destroy�� ȣ���ؾ� �Ѵ�.
	~CCoroutine()
	{
		if (true == (bool)co_handler)
		{
			co_handler.destroy();
		}
	}
};