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
	bool Down;	// ������ ������ ��
	bool Push;	// ������ ���� ��
	bool Up;	// ������ �ִ� Ű�� �ö�� ��

	KeyState() :
		key(0),
		Down(false),
		Push(false),
		Up(false)
	{
	}
};

// Ű�� ������ ��ü�� �� ��ü�� �Լ�
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
	// �̸�
	std::string Name;
	// � Ű�� ����ϴ���
	KeyState* key;
	// ����Ű ��� ����
	bool Ctrl;
	bool Alt;
	bool Shift;

	// Ű�� �Է� Ÿ��(��, �ٿ�, Ǫ��)�� ���� ����� �Լ��� �����ϴ� �迭
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
	// ����Ű�� ������ �ִ��� Ȯ���ϴ� ����
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	HWND m_hWnd;

	Vector2 m_MousePos;			// ������ â������ ���콺 ��ġ
	Vector2 m_MouseWorldPos;	// ���� ����(���� ȭ�� ��)������ ���콺 ��ġ
	Vector2 m_MouseMove;		// ���콺 �̵���
	bool	m_MouseLDown;		// ���콺 ���� ��ư�� ���� ��
	bool	m_MouseLPush;		// ���콺 ���� ��ư�� ������ ���� ��
	bool	m_MouseLUp;			// ���콺 ���� ��ư�� �� ��
	CollisionProfile*	m_MouseProfile;	// ���콺 �浹 ����
	CSharedPtr<class CGameObject>	m_MouseObj;	// �ִϸ��̼��� ���� ���� �� �ֱ� ������ UI�� �ƴ� GameObject�� �������.
	bool	m_ShowCursor;		// ��ǻ���� ���� ���콺 Ŀ���� �������� ����

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
	void UpdateKeyState(float DeltaTime);	// Ű�� ���¸� üũ�ϴ� �Լ�(����Ű, ���ٿ�Ǫ�� ����)
	void UpdateBindKey(float DeltaTime);	// ���� Ű�� �Է� ���¿� ���� ����� �Լ��� ȣ���ϴ� �Լ�

	// Name���� ����� Ű�� ����Ű�� ���� �������� �������ִ� �Լ�
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

	bool AddBindKey(const std::string& Name, unsigned char Key);	// Name�� Ű�� ���� Ʈ���� �����ϴ� �Լ�
	void ClearCallback();

public:
	// Object�� �Լ��� Input�� Ű�� ��������ִ� �Լ�
	// ���� �� KeyName�� ��Ī�� Ű�� �Է��ϸ� Object�� Func�� ����ȴ�.
	template <typename T>
	void AddBindFunction(const std::string& KeyName, Input_Type Type,
		T* Object, void(T::* Func)())
	{
		BindKey* Key = FindBindKey(KeyName);

		if (!Key)
			return;

		BindFunction* Function = new BindFunction;

		Function->Obj = Object;
		// ����Լ��� ����� ���� �Լ��ּ�, ��ü�ּҸ� �Ѵ� ����ؾ� �Ѵ�.
		Function->func = std::bind(Func, Object);

		// Ű�� �Է� Ÿ�Կ� ���� �迭�� �Լ��� ����Ѵ�.
		Key->vecFunction[(int)Type].push_back(Function);
	}

	DECLARE_SINGLE(CInput)
};

