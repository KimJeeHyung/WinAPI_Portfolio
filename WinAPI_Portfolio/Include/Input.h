#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

enum class Input_Type
{
	Down,
	Push,
	Up,
	End
};

struct KeyState
{
	unsigned char key;
	bool Down;	// 누르기 시작할 때
	bool Push;	// 누르고 있을 때
	bool Up;	// 누르고 있던 키가 올라올 때

	KeyState() :
		key(0),
		Down(false),
		Push(false),
		Up(false)
	{
	}
};

// 키와 연결할 객체와 그 객체의 함수
struct BindFunction
{
	void* Obj;
	std::function<void()> func;

	BindFunction() :
		Obj(nullptr)
	{
	}
};

struct BindKey
{
	// 이름
	std::string Name;
	// 어떤 키를 사용하는지
	KeyState* key;
	// 조합키 사용 여부
	bool Ctrl;
	bool Alt;
	bool Shift;

	// 키의 입력 타입(업, 다운, 푸시)에 따라 사용할 함수를 저장하는 배열
	std::vector<BindFunction*> vecFunction[(int)Input_Type::End];

	BindKey() :
		key(nullptr),
		Ctrl(false),
		Alt(false),
		Shift(false)
	{
	}
};

class CInput
{
private:
	std::unordered_map<unsigned char, KeyState*>	m_mapKeyState;
	std::unordered_map<std::string, BindKey*>		m_mapBindKey;
	// 조합키를 누르고 있는지 확인하는 변수
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	HWND m_hWnd;

	Vector2 m_MousePos;			// 윈도우 창에서의 마우스 위치
	Vector2 m_MouseWorldPos;	// 월드 공간(게임 화면 내)에서의 마우스 위치
	Vector2 m_MouseMove;		// 마우스 이동량
	bool	m_MouseLDown;		// 마우스 왼쪽 버튼을 누를 때
	bool	m_MouseLPush;		// 마우스 왼쪽 버튼을 누르고 있을 때
	bool	m_MouseLUp;			// 마우스 왼쪽 버튼을 뗄 때
	CollisionProfile*	m_MouseProfile;	// 마우스 충돌 정보
	CSharedPtr<class CGameObject>	m_MouseObj;	// 애니메이션을 쉽게 넣을 수 있기 때문에 UI가 아닌 GameObject로 만들었다.
	bool	m_ShowCursor;		// 컴퓨터의 실제 마우스 커서를 보여줄지 여부

public:
	bool GetMouseLDown() const
	{
		return m_MouseLDown;
	}

	bool GetMouseLPush() const
	{
		return m_MouseLPush;
	}

	bool GetMouseLUp() const
	{
		return m_MouseLUp;
	}

	CollisionProfile* GetMouseProfile() const
	{
		return m_MouseProfile;
	}

	const Vector2 GetMousePos() const
	{
		return m_MousePos;
	}

	const Vector2 GetMouseWorldPos() const
	{
		return m_MouseWorldPos;
	}

	const Vector2 GetMouseMove() const
	{
		return m_MouseMove;
	}

public:
	void ComputeWorldMousePos(const Vector2& CameraPos);

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState(float DeltaTime);	// 키의 상태를 체크하는 함수(조합키, 업다운푸시 여부)
	void UpdateBindKey(float DeltaTime);	// 누른 키의 입력 상태에 따라 등록한 함수를 호출하는 함수

	// Name으로 등록한 키에 조합키를 같이 누르도록 설정해주는 함수
	void SetKeyCtrl(const std::string& Name, bool Ctrl = true);
	void SetKeyAlt(const std::string& Name, bool Alt = true);
	void SetKeyShift(const std::string& Name, bool Shift = true);

	KeyState*	FindKeyState(unsigned char Key);
	BindKey*	FindBindKey(const std::string& Name);

public:
	void SetCursor(bool ShowCursor)
	{
		m_ShowCursor = ShowCursor;
	}

	bool AddBindKey(const std::string& Name, unsigned char Key);	// Name과 키를 묶어 트리에 저장하는 함수
	void ClearCallback();

public:
	// Object의 함수와 Input의 키를 연결시켜주는 함수
	// 연결 후 KeyName에 매칭된 키를 입력하면 Object의 Func이 실행된다.
	template <typename T>
	void AddBindFunction(const std::string& KeyName, Input_Type Type,
		T* Object, void(T::* Func)())
	{
		BindKey* Key = FindBindKey(KeyName);

		if (!Key)
			return;

		BindFunction* Function = new BindFunction;

		Function->Obj = Object;
		// 멤버함수를 등록할 때는 함수주소, 객체주소를 둘다 등록해야 한다.
		Function->func = std::bind(Func, Object);

		// 키의 입력 타입에 따라 배열에 함수를 등록한다.
		Key->vecFunction[(int)Type].push_back(Function);
	}

	DECLARE_SINGLE(CInput)
};

