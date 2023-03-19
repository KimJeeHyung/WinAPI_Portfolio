#include "Input.h"
#include "Collision/CollisionManager.h"
#include "Resource/ResourceManager.h"
#include "GameObject/GameObject.h"

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_MouseLDown(false),
	m_MouseLPush(false),
	m_MouseLUp(false),
	m_MouseObj(nullptr)
{
}

CInput::~CInput()
{
	// �ڵ� ����� �� ������ ����� ����� �ݺ��� �� 
	// ���� �̸��� ������ �ٽ� ����� �� �ֱ� ����
	// (��� ������ ���� ������ ����� ������ ���ŵ�)
	{
		std::unordered_map<unsigned char, KeyState*>::iterator iter;
		std::unordered_map<unsigned char, KeyState*>::iterator iterEnd;

		iter = m_mapKeyState.begin();
		iterEnd = m_mapKeyState.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapKeyState.clear();
	}

	{
		std::unordered_map<std::string, BindKey*>::iterator iter;
		std::unordered_map<std::string, BindKey*>::iterator iterEnd;

		iter = m_mapBindKey.begin();
		iterEnd = m_mapBindKey.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (int)Input_Type::End; i++)
			{
				size_t Size = iter->second->vecFunction[i].size();

				for (size_t j = 0; j < Size; j++)
				{
					// vecFunction�� �־��� �Լ����� �����Ҵ����� �����߱� ������ �Ҹ��ڿ���
					// ��������� �Ѵ�.
					SAFE_DELETE(iter->second->vecFunction[i][j]);
				}
			}

			SAFE_DELETE(iter->second);
		}

		m_mapBindKey.clear();
	}
}

void CInput::ComputeWorldMousePos(const Vector2& CameraPos)
{
	// ���콺 ��ġ�� ī�޶� ��ġ�� ���� ���� ���� ���� ���콺 ��ǥ�� ���Ѵ�,
	m_MouseWorldPos = m_MousePos + CameraPos;
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddBindKey("MoveRight", 'D');
	AddBindKey("MoveLeft", 'A');
	AddBindKey("MoveUp", 'W');
	AddBindKey("MoveDown", 'S');
	AddBindKey("MoveRight2", VK_RIGHT);
	AddBindKey("MoveLeft2", VK_LEFT);
	AddBindKey("MoveUp2", VK_UP);
	AddBindKey("MoveDown2", VK_DOWN);
	AddBindKey("Jump", VK_SPACE);
	AddBindKey("A", 'U');
	AddBindKey("B", 'I');
	AddBindKey("C", 'O');
	AddBindKey("D", 'J');
	AddBindKey("Skill1", '1');
	AddBindKey("Skill2", '2');
	SetKeyCtrl("Skill2");
	AddBindKey("OpenTileMapEditor", 'T');
	SetKeyCtrl("OpenTileMapEditor");

	AddBindKey("Enter", VK_RETURN);

	AddBindKey("Cam1", VK_F1);
	AddBindKey("Cam2", VK_F2);

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;

	m_MouseProfile = CCollisionManager::GetInst()->FindProfile("Mouse");

	// ���콺 ������Ʈ �� �ִϸ��̼� ����(Frame ���)
	//std::vector<std::wstring> vecFileName;

	//for (int i = 0; i <= 10; i++)
	//{
	//	TCHAR FileName[MAX_PATH] = {};
	//	// %d�� i�� ���� ���ԵǾ� ���ڿ��� ��������� �ȴ�.
	//	wsprintf(FileName, TEXT("Mouse/%d.bmp"), i);
	//	vecFileName.push_back(FileName);
	//}

	//CResourceManager::GetInst()->CreateAniamtionSequence("MouseDefault", "MouseDefault",
	//	vecFileName, TEXTURE_PATH);

	//for (int i = 0; i < 11; i++)
	//{
	//	CResourceManager::GetInst()->AddAnimationFrame("MouseDefault", 0.f, 0.f, 32.f, 31.f);
	//}

	//CResourceManager::GetInst()->SetColorKey("MouseDefault", 255, 0, 255);

	//m_MouseObj = new CGameObject;

	//m_MouseObj->SetName("Mouse");
	//m_MouseObj->Init();
	//m_MouseObj->SetSize(32.f, 31.f);
	//m_MouseObj->CreateAnimation();
	//m_MouseObj->AddAnimation("MouseDefault", true);

	//// ���� ���콺 Ŀ���� ������ �Լ�
	//ShowCursor(FALSE);
	//m_ShowCursor = false;

	return true;
}

void CInput::Update(float DeltaTime)
{
	UpdateMouse(DeltaTime);

	UpdateKeyState(DeltaTime);

	UpdateBindKey(DeltaTime);
}

void CInput::PostUpdate(float DeltaTime)
{
	//m_MouseObj->PostUpdate(DeltaTime);
}

void CInput::Render(HDC hDC, float DeltaTime)
{
	//m_MouseObj->Render(hDC, DeltaTime);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT ptMouse;	// ���콺 ��ǥ�� �޴� ����ü

	// ���콺�� ��ũ�� ��ǥ(������ â�� �ƴ� ����� ��ü�� ���� ��ǥ)�� ���´�.
	GetCursorPos(&ptMouse);

	// ��ũ�� ��ǥ�� ������ ��ǥ�� �����Ѵ�.
	ScreenToClient(m_hWnd, &ptMouse);

	// ���콺 �̵���
	m_MouseMove.x = (float)ptMouse.x - m_MousePos.x;
	m_MouseMove.y = (float)ptMouse.y - m_MousePos.y;

	m_MousePos.x = (float)ptMouse.x;
	m_MousePos.y = (float)ptMouse.y;

	if (!m_ShowCursor)
	{
		// Ŭ���̾�Ʈ ������ ũ�⸦ ���Ѵ�.
		RECT rc = {};
		GetClientRect(m_hWnd, &rc);

		// ���콺�� Ŭ���̾�Ʈ ���� �ȿ� �ִٸ� Ŀ���� �����
		// �ۿ� �ִٸ� Ŀ���� �����ش�.
		if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
			rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
		{
			ShowCursor(FALSE);
		}
		else
		{
			ShowCursor(TRUE);
		}
	}

	// ���콺 ���� ��ư Ŭ�� ���¸� �ǽð����� ó���Ѵ�.
	// ���콺 ���� ��ư�� ������ ��
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		// Down�� Push�� ��� false��� ���� �� ���� ���̴�.
		if (!m_MouseLDown && !m_MouseLPush)
		{
			m_MouseLDown = true;
			m_MouseLPush = true;
		}
		// 1������ �� Push�� ����
		else
			m_MouseLDown = false;
	}
	// ���콺 ���� ��ư�� ���� ��
	else if (m_MouseLPush)
	{
		m_MouseLDown = false;
		m_MouseLPush = false;
		m_MouseLUp = true;
	}
	// 1������ �� Up�� false�� ����
	else if (m_MouseLUp)
		m_MouseLUp = false;

	// ���콺 ������Ʈ�� ��ġ�� ȭ�� �������� ���콺 ��ǥ�� �����ش�.
	//m_MouseObj->SetPos(m_MouseWorldPos.x, m_MouseWorldPos.y);
	//m_MouseObj->Update(DeltaTime);
}

void CInput::UpdateKeyState(float DeltaTime)
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_Ctrl = true;
	else
		m_Ctrl = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
		m_Alt = true;
	else
		m_Alt = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_Shift = true;
	else
		m_Shift = false;

	std::unordered_map<unsigned char, KeyState*>::iterator iter;
	std::unordered_map<unsigned char, KeyState*>::iterator iterEnd;

	iter = m_mapKeyState.begin();
	iterEnd = m_mapKeyState.end();

	for (; iter != iterEnd; ++iter)
	{
		bool KeyPush = false;

		if (GetAsyncKeyState(iter->second->key) & 0x8000)
		{
			KeyPush = true;
		}

		// Ű�� ������ ���
		if (KeyPush)
		{
			// Down�� Push�� ��� false��� �� Ű�� ���� ���� ���̴�.
			// �׷��Ƿ� �� �� true�� �����Ѵ�.
			if (!iter->second->Down && !iter->second->Push)
			{
				iter->second->Down = true;
				iter->second->Push = true;
			}
			// Down�� Push �� �� �ϳ��� true��� Down�� false�� �Ǿ�� �Ѵ�.
			// Push�� �̹� ������ true�� ����Ǿ���.
			else
				iter->second->Down = false;
		}
		// Ű�� �� �������� ��� Push�� true���
		// ���� �����ӿ� Ű�� ������ �ִٰ� ���� �������ٴ� ���̴�.
		else if (iter->second->Push)
		{
			iter->second->Up = true;
			iter->second->Down = false;
			iter->second->Push = false;
		}
		else if(iter->second->Up)
			iter->second->Up = false;
	}
}

void CInput::UpdateBindKey(float DeltaTime)
{
	std::unordered_map<std::string, BindKey*>::iterator iter;
	std::unordered_map<std::string, BindKey*>::iterator iterEnd;

	iter = m_mapBindKey.begin();
	iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; ++iter)
	{
		// Ű�� ������ ��(Down)
		// (����Ű�� ���� ������ Ű�� ��� �� ����Ű�� ���� �������� Ȯ��)
		if (iter->second->key->Down &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t Size = iter->second->vecFunction[(int)Input_Type::Down].size();

			for (size_t i = 0; i < Size; i++)
			{
				// �ش� Ű�� Down ���¿� ����� �Լ����� ȣ���Ѵ�.
				iter->second->vecFunction[(int)Input_Type::Down][i]->func();
			}
		}

		// Ű�� ������ ���� ��(Push)
		// (����Ű�� ���� ������ Ű�� ��� �� ����Ű�� ���� �������� Ȯ��)
		if (iter->second->key->Push &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t Size = iter->second->vecFunction[(int)Input_Type::Push].size();

			for (size_t i = 0; i < Size; i++)
			{
				// �ش� Ű�� Push ���¿� ����� �Լ����� ȣ���Ѵ�.
				iter->second->vecFunction[(int)Input_Type::Push][i]->func();
			}
		}

		// Ű�� ���� ��(Up)
		// (����Ű�� ���� ������ Ű�� ��� �� ����Ű�� ���� �������� Ȯ��)
		if (iter->second->key->Up &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t Size = iter->second->vecFunction[(int)Input_Type::Up].size();

			for (size_t i = 0; i < Size; i++)
			{
				// �ش� Ű�� Up ���¿� ����� �Լ����� ȣ���Ѵ�.
				iter->second->vecFunction[(int)Input_Type::Up][i]->func();
			}
		}
	}
}

void CInput::SetKeyCtrl(const std::string& Name, bool Ctrl)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Ctrl = Ctrl;
}

void CInput::SetKeyAlt(const std::string& Name, bool Alt)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Alt = Alt;
}

void CInput::SetKeyShift(const std::string& Name, bool Shift)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Shift = Shift;
}

KeyState* CInput::FindKeyState(unsigned char Key)
{
	std::unordered_map<unsigned char, KeyState*>::iterator iter;

	iter = m_mapKeyState.find(Key);

	// �� ã���� ���
	if(iter == m_mapKeyState.end())
		return nullptr;

	// iter->first : key
	// iter->second : value
	return iter->second;
}

BindKey* CInput::FindBindKey(const std::string& Name)
{
	std::unordered_map<std::string, BindKey*>::iterator iter;

	iter = m_mapBindKey.find(Name);

	// �� ã���� ���
	if (iter == m_mapBindKey.end())
		return nullptr;

	// iter->first : key
	// iter->second : value
	return iter->second;
}

bool CInput::AddBindKey(const std::string& Name, unsigned char Key)
{
	// ���� �̸����� BindKey�� ��ϵǾ� ���� ���
	if (FindBindKey(Name))
		return false;

	BindKey* NewKey = new BindKey;

	// �ش� Ű�� �̹� KeyState�� ��ϵǾ� �ִ����� ã�´�.
	KeyState* State = FindKeyState(Key);

	// ����� �� �Ǿ� ���� ��� KeyState�� ���� ����Ѵ�.
	if (!State)
	{
		State = new KeyState;
		State->key = Key;

		m_mapKeyState.insert(std::make_pair(Key, State));
	}

	NewKey->key = State;
	NewKey->Name = Name;

	m_mapBindKey.insert(std::make_pair(Name, NewKey));

	return true;
}

void CInput::ClearCallback()
{
	std::unordered_map<std::string, BindKey*>::iterator iter;
	std::unordered_map<std::string, BindKey*>::iterator iterEnd;

	iter = m_mapBindKey.begin();
	iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)Input_Type::End; i++)
		{
			size_t Size = iter->second->vecFunction[i].size();

			for (size_t j = 0; j < Size; j++)
			{
				SAFE_DELETE(iter->second->vecFunction[i][j]);
			}

			iter->second->vecFunction[i].clear();
		}
	}
}
