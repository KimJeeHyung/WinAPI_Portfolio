#pragma once
#include <coroutine> // 코루틴을 사용하기 위한 헤더

class CCoroutine
{
public:
	// 규칙 1. C++에서 정의된 규칙을 구현한 promise_type이라는 이름의 타입이 정의되어야 한다.
	struct promise_type
	{
		// 사용자 정의 "코루틴 반환 객체"를 반환한다.
		CCoroutine get_return_object()
		{
			return CCoroutine{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		// 코루틴 최초 실행 시 호출. awaitable 객체를 반환한다.
		auto initial_suspend() { return std::suspend_always{}; }

		// co_return을 사용하는 경우 구현.
		void return_void() { return; }

		// 코루틴 종료 시 호출.
		auto final_suspend() noexcept { return std::suspend_always{}; }

		// 코루틴 수행 중 예외 발생 시 호출.
		void unhandled_exception() { std::exit(1); }
	};

	// 규칙 2. std::coroutine_handle<promise_type> 타입의 멤버 변수가 있어야 한다.
	std::coroutine_handle<promise_type> co_handler;

	// 규칙 3. std::coroutine_handle<promise_type> 을 인자로 받아
	// 멤버 변수를 초기화하는 생성자가 있어야 한다.
	CCoroutine(std::coroutine_handle<promise_type> handler) : co_handler(handler) {}

	// 규칙 4. 소멸자에서 std::coroutine_handle<promise_type> 타입의
	// 코루틴 핸들러 멤버 변수의 destroy를 호출해야 한다.
	~CCoroutine()
	{
		if (true == (bool)co_handler)
		{
			co_handler.destroy();
		}
	}
};