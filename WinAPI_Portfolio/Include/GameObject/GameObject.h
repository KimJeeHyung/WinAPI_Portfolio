#pragma once

#include "../Ref.h"
#include "../Animation/Animation.h"
#include "../Widget/WidgetComponent.h"

class CGameObject :
	public CRef
{
	friend class CScene;
	friend class CInput;	// ���콺 ������Ʈ ������ CInput Ŭ�������� �� �� �ְ� ��

protected:
	CGameObject();
	CGameObject(const CGameObject& Obj);
	virtual ~CGameObject();

protected:
	class CScene* m_Scene;	// �ڽ��� �Ҽӵ� ��

protected:
	ERender_Layer	m_RenderLayer;	// �ڽ��� ���� ���̾� �׷�
	Vector2			m_PrevPos;		// �� ������ ���� ��ġ
	Vector2			m_Move;			// �̵���(0�̸� �������� �ʴ� ����)
	Vector2			m_Pos;
	Vector2			m_Size;
	Vector2			m_Pivot;
	CSharedPtr<class CTexture> m_Texture;
	CAnimation*		m_Animation;
	float			m_TimeScale;
	float			m_MoveSpeed;
	std::list<CSharedPtr<class CCollider>>	m_ColliderList;			// �浹ü ����Ʈ
	std::list<CSharedPtr<CWidgetComponent>> m_WidgetComponentList;	// ���� ������Ʈ ����Ʈ
	bool			m_PhysicsSimulate;	// ���� ������ ���� ����
	bool			m_Ground;			// ���� ��� �ִ� �������� ����
	float			m_GravityAccel;		// �߷� ���ӵ�
	float			m_FallTime;			// �������µ� �ɸ��� �ð�
	float			m_FallStartY;		// �������� ������ �� Y��
	bool			m_Jump;				// ���� ���� ������
	float			m_JumpVelocity;		// ���� �ӵ�
	int				m_JumpCount;		// ���� Ƚ��
	bool			m_SideWallCheck;	// ���� �������� ����(�¿� �浹)
	bool			m_Start;			// ������Ʈ�� �ʱ�ȭ �� �����ߴ��� ����
	CGameObject*	m_Opponent;		// ��� ĳ����
	bool			m_Playable;			// ���� �������� ����

public:
	class CCollider* FindCollider(const std::string& Name);

	ERender_Layer GetRenderLayer() const
	{
		return m_RenderLayer;
	}

	float GetTimeScale() const
	{
		return m_TimeScale;
	}

	const Vector2& GetPos() const
	{
		return m_Pos;
	}

	const Vector2& GetSize() const
	{
		return m_Size;
	}

	const Vector2& GetPivot() const
	{
		return m_Pivot;
	}

	CGameObject* GetOpponent() const
	{
		return m_Opponent;
	}

	const Vector2& GetMove() const
	{
		return m_Move;
	}

public:
	void SetPlayable(bool Playable)
	{
		m_Playable = Playable;
	}

	void SetOpponent(CGameObject* Opponent)
	{
		m_Opponent = Opponent;
	}

	void SetSideWallCheck(bool Check)
	{
		m_SideWallCheck = Check;
	}

	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}

	void SetGravityAccel(float Accel)
	{
		m_GravityAccel = Accel;
	}

	void SetJumpVelocity(float JumpVelocity)
	{
		m_JumpVelocity = JumpVelocity;
	}

	void Jump()
	{
		if (m_JumpCount == 2)
			return;

		else if (m_JumpCount == 1)
		{
			m_FallTime = 0.f;
			m_FallStartY = m_Pos.y;
		}

		++m_JumpCount;

		if (!m_Jump)
		{
			m_Jump = true;
			m_Ground = false;

			m_FallTime = 0.f;
			m_FallStartY = m_Pos.y;
		}
	}

	void SetTimeScale(float Scale)
	{
		m_TimeScale = Scale;
	}

	virtual void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	virtual void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}

	void SetPivot(const Vector2& Pivot)
	{
		m_Pivot = Pivot;
	}

public:
	void SetTexture(const std::string& Name);
	void SetTexture(class CTexture* Texture);
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);

#ifdef UNICODE

	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const std::vector<std::wstring>& vecFullPath);

#else

	void SetTexture(const std::string& Name, const std::vector<std::string>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const std::vector<std::string>& vecFullPath);

#endif // UNICODE

	bool SetColorKey(unsigned char r, unsigned char g, unsigned char b,
		int Index = 0);
	bool SetColorKeyAll(unsigned char r, unsigned char g, unsigned char b);

public:
	void CreateAnimation();
	void AddAnimation(const std::string& SequenceName, bool Loop = true,
		float PlayTime = 1.f, float PlayScale = 1.f, bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetPlayLoop(const std::string& Name, bool Loop);
	void SetPlayReverse(const std::string& Name, bool Reverse);
	void SetCurrentAniamtion(std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);

public:	// �̵� �Լ���
	void MoveDir(const Vector2& Dir);
	void Move(const Vector2& MoveValue);
	void Move(float Angle);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);	// Update�� ��� ���ư� �� �����ϴ� �Լ�
	virtual void Render(HDC hDC, float DeltaTime);
	virtual float InflictDamage(float Damage);
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	void SaveFullPath(const char* FullPath);
	void LoadFullPath(const char* FullPath);
	void SaveFileName(const char* FileName, const std::string& PathName);
	void LoadFileName(const char* FileName, const std::string& PathName);

private:
	void CheckMoveRight();
	void CheckMoveLeft();

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void(T::* Func)())
	{
		if (m_Animation)
			m_Animation->SetEndFunction<T>(Name, Obj, Func);
	}

	template <typename T>
	void SetCurrentAnimationEndFunction(T* Obj, void(T::* Func)())
	{
		if (m_Animation)
			m_Animation->SetCurrentAnimationEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)())
	{
		if (m_Animation)
			m_Animation->AddNotify<T>(Name, Frame, Obj, Func);
	}

	template <typename T>
	T* AddCollider(const std::string& Name)
	{
		T* Collider = new T;

		Collider->SetName(Name);
		Collider->m_Owner = this;
		Collider->m_Scene = m_Scene;

		if (!Collider->Init())
		{
			SAFE_DELETE(Collider);
			return nullptr;
		}

		m_ColliderList.push_back(Collider);

		return Collider;
	}

	CWidgetComponent* FindWidgetComponent(const std::string& Name)
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}

		return nullptr;
	}

	template <typename T>
	CWidgetComponent* CreateWidgetComponent(const std::string& Name)
	{
		CWidgetComponent* Widget = FindWidgetComponent(Name);

		if (Widget)
			return Widget;

		Widget = new CWidgetComponent;

		Widget->m_Owner = this;
		Widget->m_Scene = m_Scene;
		Widget->SetName(Name);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		Widget->CreateWidget<T>(Name);

		m_WidgetComponentList.push_back(Widget);

		return Widget;
	}
};

