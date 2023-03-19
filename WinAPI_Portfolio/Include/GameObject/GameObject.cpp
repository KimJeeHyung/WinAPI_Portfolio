#include "GameObject.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Collision/Collider.h"
#include "../PathManager.h"
#include "TileMap.h"
#include "Tile.h"

CGameObject::CGameObject() :
	m_Scene(nullptr),
	m_Animation(nullptr),
	m_TimeScale(1.f),
	m_MoveSpeed(0.f),
	m_RenderLayer(ERender_Layer::Default),
	m_PhysicsSimulate(false),
	m_Ground(false),
	m_GravityAccel(10.f),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_SideWallCheck(false),
	m_Start(false)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& Obj) :
	CRef(Obj),
	m_Scene(nullptr),
	m_Pos(Obj.m_Pos),
	m_Size(Obj.m_Size),
	m_Pivot(Obj.m_Pivot),
	m_TimeScale(Obj.m_TimeScale),
	m_PhysicsSimulate(Obj.m_PhysicsSimulate),
	m_Ground(false),
	m_GravityAccel(Obj.m_GravityAccel),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(Obj.m_JumpVelocity),
	m_SideWallCheck(Obj.m_SideWallCheck),
	m_Start(false)
{
}

CGameObject::~CGameObject()
{
	// �浹ü ����
	{
		std::list<CSharedPtr<CCollider>>::iterator iter;
		std::list<CSharedPtr<CCollider>>::iterator iterEnd;

		iter = m_ColliderList.begin();
		iterEnd = m_ColliderList.end();

		for (; iter != iterEnd; ++iter)
		{
			// ������ ������ �ִ� �ڽ��� �浹ü�� ������
			// ���۷��� ī��Ʈ�� 0���� �����.(�� �ϸ� �޸� �� ����)
			(*iter)->ClearCollisionList();
		}
	}

	// ���� ������Ʈ ����
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->SetActive(false);
		}
	}

	if (m_Opponent)
		m_Opponent = nullptr;

	SAFE_DELETE(m_Animation);
}

CCollider* CGameObject::FindCollider(const std::string& Name)
{
	std::list<CSharedPtr<CCollider>>::iterator iter;
	std::list<CSharedPtr<CCollider>>::iterator iterEnd;

	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

#ifdef UNICODE

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

#else

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

#endif // UNICODE

bool CGameObject::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (!m_Texture)
		return false;

	m_Texture->SetColorKey(r, g, b, Index);

	return true;
}

bool CGameObject::SetColorKeyAll(unsigned char r, unsigned char g, unsigned char b)
{
	if (!m_Texture)
		return false;

	m_Texture->SetColorKeyAll(r, g, b);

	return true;
}

void CGameObject::CreateAnimation()
{
	m_Animation = new CAnimation;

	m_Animation->m_Owner = this;
	m_Animation->m_Scene = m_Scene;
}

void CGameObject::AddAnimation(const std::string& SequenceName, bool Loop, 
	float PlayTime, float PlayScale, bool Reverse)
{
	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetPlayTime(const std::string& Name, float PlayTime)
{
	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetPlayScale(const std::string& Name, float PlayScale)
{
	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetPlayLoop(const std::string& Name, bool Loop)
{
	m_Animation->SetPlayLoop(Name, Loop);
}

void CGameObject::SetPlayReverse(const std::string& Name, bool Reverse)
{
	m_Animation->SetPlayReverse(Name, Reverse);
}

void CGameObject::SetCurrentAniamtion(std::string& Name)
{
	m_Animation->SetCurrentAniamtion(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	m_Animation->ChangeAnimation(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

void CGameObject::MoveDir(const Vector2& Dir)
{
	m_Pos += Dir * m_MoveSpeed * DELTA_TIME * m_TimeScale;
}

void CGameObject::Move(const Vector2& MoveValue)
{
	m_Pos += MoveValue * m_TimeScale;
}

void CGameObject::Move(float Angle)
{
	Vector2 Dir;
	Dir.x = cosf(DegreeToRadian(Angle));
	Dir.y = sinf(DegreeToRadian(Angle));

	m_Pos += Dir * m_MoveSpeed * DELTA_TIME * m_TimeScale;
}

void CGameObject::Start()
{
	m_Start = true;

	// ������Ʈ�� ���� ��ġ�� ���� ��ġ�� ������ ó������ ������ �� �ֵ��� �Ѵ�.
	m_PrevPos = m_Pos;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	// �߷� ����
	if (!m_Ground && m_PhysicsSimulate)
	{
		// �������� �ð��� ���������ش�.
		m_FallTime += DeltaTime * m_GravityAccel;

		// 9.8m/s^2
		// t�� �� y��
		// V : �ӵ� A : ���ӵ� G : �߷�
		// y = V * A - 0.5 * G * t * t
		// 0 = -0.5GA^2 VA - y
		float Velocity = 0.f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);
	}

	// �ִϸ��̼� Update
	if (m_Animation)
		m_Animation->Update(DeltaTime * m_TimeScale);

	// �浹ü Update
	{
		std::list<CSharedPtr<CCollider>>::iterator iter;
		std::list<CSharedPtr<CCollider>>::iterator iterEnd;

		iter = m_ColliderList.begin();
		iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			// �浹ü�� �������ٸ� ��Ͽ��� ����� ���� �浹ü�� �Ѿ��.
			if (!(*iter)->GetActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			// �浹ü�� ��Ȱ��ȭ �Ǿ��ִٸ� �ٷ� ���� �浹ü�� �Ѿ��.
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}

	// ���� ������Ʈ Update
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			// ������ �������ٸ� ��Ͽ��� ����� ���� �������� �Ѿ��.
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}
			// ������ ��Ȱ��ȭ �Ǿ��ִٸ� �ٷ� ���� �������� �Ѿ��.
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	// ���� ��ġ���� ���� ��ġ�� ���� �̵��� ���
	m_Move = m_Pos - m_PrevPos;

	// �ִϸ��̼��� ���۵� ��� �̹��� ũ��� ����� �����ϱ� ������
	// PostUpdate���� ����� �ٽ� �����ֵ��� �Ѵ�.
	if (m_Animation)
	{
		CAnimationInfo* Current = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = Current->m_Sequence->GetFrame(Current->m_Frame);

		//m_Size = FrameData.End - FrameData.Start;
	}

	// �浹ü PostUpdate
	{
		std::list<CSharedPtr<CCollider>>::iterator iter;
		std::list<CSharedPtr<CCollider>>::iterator iterEnd;

		iter = m_ColliderList.begin();
		iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			// �浹ü�� �������ٸ� ����� ���� �浹ü�� �Ѿ��.
			if (!(*iter)->GetActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			// �浹ü�� ��Ȱ��ȭ �Ǿ��ִٸ� �ٷ� ���� �浹ü�� �Ѿ��.
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);

			m_Scene->GetCollision()->AddCollider(*iter);

			++iter;
		}
	}

	// ���� ������Ʈ PostUpdate
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			// ������ �������ٸ� ��Ͽ��� ����� ���� �������� �Ѿ��.
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}
			// ������ ��Ȱ��ȭ �Ǿ��ִٸ� �ٷ� ���� �������� �Ѿ��.
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);
			++iter;
		}
	}

	// �¿�� �����̴ٰ� ���� �ε����� ���
	if (m_Move.x != 0.f && m_SideWallCheck)
	{
		// ���������� �̵��� ��
		if (m_Move.x > 0.f)
		{
			CheckMoveRight();
		}
		// �������� �̵��� ��
		else
		{
			CheckMoveLeft();
		}
	}

	// �ٴڿ� ������Ų��.
	if (m_PhysicsSimulate && m_Move.y >= 0.f)
	{
		CTileMap* TileMap = m_Scene->GetTileMap();

		// ������Ʈ�� ���� ������ ��ġ�� ���� ��ġ�� Left, Right, Bottom�� �̿���
		// �� ������ �̵��� ���̸�ŭ�� �簢�� ������ ���Ѵ�.
		float PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
		float CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;

		float PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
		float CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;

		float PrevRight = m_PrevPos.x + (1.f - m_Pivot.x) * m_Size.x;
		float CurRight = m_Pos.x + (1.f - m_Pivot.x) * m_Size.x;

		float ResultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		float ResultRight = PrevRight > CurRight ? PrevRight : CurRight;
		float ResultTop = PrevBottom < CurBottom ? PrevBottom : CurBottom;
		float ResultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

		// ���� �簢���� �� �𼭸��� ��� �ִ� Ÿ���� �ε����� ���Ѵ�.
		int LeftIndex = TileMap->GetTileOriginIndexX(ResultLeft);
		int RightIndex = TileMap->GetTileOriginIndexX(ResultRight);
		int TopIndex = TileMap->GetTileOriginIndexY(ResultTop);
		int BottomIndex = TileMap->GetTileOriginIndexY(ResultBottom);

		LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
		TopIndex = TopIndex < 0 ? 0 : TopIndex;
		RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
		BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

		// ��� Ÿ�� �ε����� Ÿ�ϸ� �ȿ� ���� ���� �浹 üũ�� �Ѵ�.
		if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
			RightIndex >= 0 && BottomIndex >= 0)
		{
			bool Check = false;

			// ������ �Ʒ��� ���ʷ� �˻縦 �س�����.
			// �Ʒ��������� �ϰ� �Ǹ� ���� ���� ���� ��� �����ϰ� ó���Ź��� ���� �ֱ� �����̴�.
			for (int i = TopIndex; i <= BottomIndex; i++)
			{
				for (int j = LeftIndex; j <= RightIndex; j++)
				{
					CTile* Tile = TileMap->GetTile(j, i);

					// ���� Ÿ���� ������ ����
					if (!Tile)
						continue;

					// ���� Ÿ���� Top�� ������Ʈ�� ���� ��ġ�� Bottom���� ���� �־ ����
					if (Tile->GetPos().y < PrevBottom)
						continue;

					// ���� Ÿ���� �̵��Ұ� Ÿ���� ��� �ٴ��� ��Ҵٴ� ���̴�.
					if (Tile->GetOption() == ETile_Option::ImpossibleToMove)
					{
						Check = true;
						m_FallTime = 0.f;

						m_JumpCount = 0;

						// ���� Ÿ���� Top�� �� ������Ʈ�� Bottom������ �����Ѵ�.
						m_Pos.y = Tile->GetPos().y - (1.f - m_Pivot.y) * m_Size.y;
						m_Ground = true;
						m_Jump = false;
						break;
					}
				}

				// �ٴ��� ��Ҵٸ� �ݺ����� �������´�.
				if (Check)
					break;
			}

			// �ٴ��� ���� ���
			if (!Check)
			{
				// ���� ��� �ִ� ���¶�� ���� �������� ���°� �� ���̴�.
				if (m_Ground)
				{
					m_FallTime = 0.f;
					m_FallStartY = m_Pos.y;
				}

				m_Ground = false;
			}
		}
	}
}

void CGameObject::Render(HDC hDC, float DeltaTime)
{
	// ���� ������Ʈ�� ��ġ���� ī�޶��� ��ġ�� ���� ȭ�� �� ��µ� ��ġ�� �������ش�.
	Vector2 Pos;
	Vector2 CameraPos;
	Vector2 Resolution;

	// ������Ʈ���� m_Scene�� �ְų� ���� ��찡 �ֱ� ������
	// ������ ��쿡 �°� CameraPos�� Resolution�� �����ش�.
	if (m_Scene)
	{
		Pos = m_Pos - m_Scene->GetCamera()->GetPos();
		CameraPos = m_Scene->GetCamera()->GetPos();
		Resolution = m_Scene->GetCamera()->GetResolution();
	}
	else
	{
		CScene* Scene = CSceneManager::GetInst()->GetScene();
		Pos = m_Pos - Scene->GetCamera()->GetPos();
		CameraPos = Scene->GetCamera()->GetPos();
		Resolution = Scene->GetCamera()->GetResolution();
	}

	if (m_Animation)
	{
		CAnimationInfo* Current = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = Current->m_Sequence->GetFrame(Current->m_Frame);

		Vector2 Size = FrameData.End - FrameData.Start;

		Vector2 RenderLT;

		RenderLT = Pos - m_Pivot * Size;

		// ī�޶� ������ ���� �ʴ� ���� ��ġ
		Vector2 CullPos = m_Pos - m_Pivot * Size;

		// ������Ʈ �ø�
		// ��ü�� ���� X��ǥ�� ȭ�� ���� ����� ������� �ʴ´�.
		if (CullPos.x > CameraPos.x + Resolution.x)
			return;
		// ��ü�� ������ X��ǥ�� ī�޶� X��ǥ���� �۴ٸ� ������� �ʴ´�.
		else if (CullPos.x + Size.x < CameraPos.x)
			return;
		// ��ü�� ���� Y��ǥ�� ȭ�� ���� ����� ������� �ʴ´�.
		else if (CullPos.y > CameraPos.y + Resolution.y)
			return;
		// ��ü�� ������ Y��ǥ�� ī�޶� Y��ǥ���� �۴ٸ� ������� �ʴ´�.
		else if (CullPos.y + Size.y < CameraPos.y)
			return;

		// ��������Ʈ �ؽ�ó
		if (Current->m_Sequence->GetTextureType() == ETexture_Type::Sprite)
		{
			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
			if (Current->m_Sequence->GetTexture()->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, 
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(),
					(int)FrameData.Start.x, (int)FrameData.Start.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetColorKey());
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(),
					(int)FrameData.Start.x, (int)FrameData.Start.y, SRCCOPY);
			}
		}
		// ������ �ؽ�ó
		else
		{
			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
			if (Current->m_Sequence->GetTexture()->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(Current->m_Frame),
					(int)FrameData.Start.x, (int)FrameData.Start.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetColorKey());
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(Current->m_Frame),
					(int)FrameData.Start.x, (int)FrameData.Start.y, SRCCOPY);
			}
		}
	}
	else
	{
		if (m_Texture)
		{
			Vector2 RenderLT;

			RenderLT = Pos - m_Pivot * m_Size;

			// ī�޶� ������ ���� �ʴ� ���� ��ġ
			Vector2 CullPos = m_Pos - m_Pivot * m_Size;

			// ������Ʈ �ø�
			// ��ü�� ���� X��ǥ�� ȭ�� ���� ����� ������� �ʴ´�.
			if (CullPos.x > CameraPos.x + Resolution.x)
				return;
			// ��ü�� ������ X��ǥ�� ī�޶� X��ǥ���� �۴ٸ� ������� �ʴ´�.
			else if (CullPos.x + m_Size.x < CameraPos.x)
				return;
			// ��ü�� ���� Y��ǥ�� ȭ�� ���� ����� ������� �ʴ´�.
			else if (CullPos.y > CameraPos.y + Resolution.y)
				return;
			// ��ü�� ������ Y��ǥ�� ī�޶� Y��ǥ���� �۴ٸ� ������� �ʴ´�.
			else if (CullPos.y + m_Size.y < CameraPos.y)
				return;

			// �÷�Ű�� Ȱ��ȭ�Ǿ������� �ش� ���� ������� �ʴ´�.
			if (m_Texture->GetEnableColorKey())
			{
				if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
				{
					TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)m_Size.x, (int)m_Size.y,
						m_Texture->GetDC(), 0, 0, (int)m_Size.x, (int)m_Size.y, m_Texture->GetColorKey());
				}
				else
				{
				}
			}
			// �Ϲ� ��Ʈ�� ��� �Լ�
			else
			{
				if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
				{
					BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)m_Size.x, (int)m_Size.y,
						m_Texture->GetDC(), 0, 0, SRCCOPY);
				}
				else
				{
				}
			}
		}
	}

	std::list<CSharedPtr<CCollider>>::iterator iter;
	std::list<CSharedPtr<CCollider>>::iterator iterEnd;

	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		// �浹ü�� �������ٸ� ����� ���� �浹ü�� �Ѿ��.
		if (!(*iter)->GetActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		// �浹ü�� ��Ȱ��ȭ �Ǿ��ִٸ� �ٷ� ���� �浹ü�� �Ѿ��.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}

	// ����� ���� ������ ���� ��ġ�� �����Ѵ�.
	m_PrevPos = m_Pos;
}

// Damage��ŭ ���ظ� �Դ´�.
float CGameObject::InflictDamage(float Damage)
{
	return Damage;
}

// GameObject Ŭ������ ��� �������� �����Ѵ�.
void CGameObject::Save(FILE* File)
{
	CRef::Save(File);

	fwrite(&m_RenderLayer, sizeof(ERender_Layer), 1, File);
	fwrite(&m_PrevPos, sizeof(Vector2), 1, File);
	fwrite(&m_Move, sizeof(Vector2), 1, File);
	fwrite(&m_Pos, sizeof(Vector2), 1, File);
	fwrite(&m_Size, sizeof(Vector2), 1, File);
	fwrite(&m_Pivot, sizeof(Vector2), 1, File);

	bool	Texture = false;

	if (m_Texture)
		Texture = true;

	fwrite(&Texture, sizeof(bool), 1, File);

	if (m_Texture)
	{
		// �ؽ�ó ����
		m_Texture->Save(File);
	}

	bool	Animation = false;

	if (m_Animation)
		Animation = true;

	fwrite(&Animation, sizeof(bool), 1, File);

	if (m_Animation)
	{
		// �ִϸ��̼� ����
	}

	fwrite(&m_TimeScale, sizeof(float), 1, File);
	fwrite(&m_MoveSpeed, sizeof(float), 1, File);

	fwrite(&m_PhysicsSimulate, sizeof(bool), 1, File);
	fwrite(&m_Ground, sizeof(bool), 1, File);

	fwrite(&m_GravityAccel, sizeof(float), 1, File);
	fwrite(&m_FallTime, sizeof(float), 1, File);
	fwrite(&m_FallStartY, sizeof(float), 1, File);
	fwrite(&m_JumpVelocity, sizeof(float), 1, File);

	fwrite(&m_Jump, sizeof(bool), 1, File);
}

// GameObject Ŭ������ ��� �������� �ҷ��´�.(������ ������� �ҷ��� ��)
void CGameObject::Load(FILE* File)
{
	CRef::Load(File);

	fread(&m_RenderLayer, sizeof(ERender_Layer), 1, File);
	fread(&m_PrevPos, sizeof(Vector2), 1, File);
	fread(&m_Move, sizeof(Vector2), 1, File);
	fread(&m_Pos, sizeof(Vector2), 1, File);
	fread(&m_Size, sizeof(Vector2), 1, File);
	fread(&m_Pivot, sizeof(Vector2), 1, File);

	bool	Texture = false;

	fread(&Texture, sizeof(bool), 1, File);

	if (Texture)
	{
		// �ؽ�ó �ҷ�����
		m_Texture = m_Scene->GetSceneResource()->LoadTexture(File);
	}

	bool	Animation = false;

	fread(&Animation, sizeof(bool), 1, File);

	if (Animation)
	{
		// �ִϸ��̼� �ҷ�����
	}

	fread(&m_TimeScale, sizeof(float), 1, File);
	fread(&m_MoveSpeed, sizeof(float), 1, File);

	fread(&m_PhysicsSimulate, sizeof(bool), 1, File);
	fread(&m_Ground, sizeof(bool), 1, File);

	fread(&m_GravityAccel, sizeof(float), 1, File);
	fread(&m_FallTime, sizeof(float), 1, File);
	fread(&m_FallStartY, sizeof(float), 1, File);
	fread(&m_JumpVelocity, sizeof(float), 1, File);

	fread(&m_Jump, sizeof(bool), 1, File);
}

void CGameObject::SaveFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	Save(File);

	fclose(File);
}

void CGameObject::LoadFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;

	Load(File);

	fclose(File);
}

void CGameObject::SaveFileName(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CGameObject::LoadFileName(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CGameObject::CheckMoveRight()
{
	// ������Ʈ�� ���� ������ ��ġ�� ���� ��ġ�� �簢�� ������ ���Ѵ�.
	Vector2 PrevLT = m_PrevPos - m_Pivot * m_Size;
	Vector2 PrevRB = PrevLT + m_Size;

	Vector2 LT = m_Pos - m_Pivot * m_Size;
	Vector2 RB = LT + m_Size;

	CTileMap* TileMap = m_Scene->GetTileMap();

	// ���� ������ ��ġ�� ���� ��ġ�� ����
	// �� ������ �̵��� ���̸�ŭ�� �簢�� ������ ���Ѵ�.
	Vector2 ResultLT, ResultRB;
	ResultRB = RB;

	ResultLT.x = PrevRB.x;
	ResultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
	ResultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

	// ���� �簢���� �� �𼭸��� ��� �ִ� Ÿ���� �ε����� ���Ѵ�.
	int LeftIndex = TileMap->GetTileOriginIndexX(ResultLT.x);
	int TopIndex = TileMap->GetTileOriginIndexY(ResultLT.y);
	int RightIndex = TileMap->GetTileOriginIndexX(ResultRB.x);
	int BottomIndex = TileMap->GetTileOriginIndexY(ResultRB.y);

	LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
	TopIndex = TopIndex < 0 ? 0 : TopIndex;
	RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
	BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

	// ��� Ÿ�� �ε����� Ÿ�ϸ� �ȿ� ���� ���� �浹 üũ�� �Ѵ�.
	if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
		RightIndex >= 0 && BottomIndex >= 0)
	{
		bool SideCollision = false;

		for (int i = TopIndex; i <= BottomIndex; i++)
		{
			for (int j = LeftIndex; j <= RightIndex; j++)
			{
				CTile* Tile = TileMap->GetTile(j, i);

				// Ÿ���� �̵��Ұ� �ɼ��� �ƴϰų� ���̵� �ݸ����� ��Ȱ��ȭ�Ǿ� ������ ����
				if (Tile->GetOption() != ETile_Option::ImpossibleToMove ||
					!Tile->GetSideCollision())
					continue;

				Vector2 TilePos = Tile->GetPos();
				Vector2 TileSize = Tile->GetSize();

				// ������Ʈ�� ���� �̵� �� Ÿ�Ͽ� �ɸ��� ���� ������ �ʵ���
				// �ణ�� �������� �д�.
				if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
					continue;

				// ������Ʈ�� Right�� Ÿ���� Left�� �����Ÿ� �̻� �Ѿ�ٸ�
				// �浹�� Ȯ���� �ʿ䰡 ����.
				/*if (RB.x - TilePos.x > 8.f)
					continue;*/

				// ���� ��ġ�� �簢���� Ÿ���� �浹���Ѽ� �İ������� �Ǵ��Ѵ�.
				if (LT.x <= TilePos.x + TileSize.x && LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x && RB.y >= TilePos.y)
				{
					SideCollision = true;

					// �󸶳� �İ������� ���Ѵ�.(����� ������)
					float MoveX = TilePos.x - RB.x - 0.001f;

					// �İ�� �Ÿ���ŭ ������Ʈ�� �̵������ش�.(Ÿ�Ͽ� ���� �ʵ���)
					m_Pos.x += MoveX;
					//m_Move.x += MoveX;
					break;
				}
			}

			// �浹�� �ߴٸ� �ݺ����� �������´�.
			if (SideCollision)
				break;
		}
	}
}

void CGameObject::CheckMoveLeft()
{
	// ������Ʈ�� ���� ������ ��ġ�� ���� ��ġ�� �簢�� ������ ���Ѵ�.
	Vector2 PrevLT = m_PrevPos - m_Pivot * m_Size;
	Vector2 PrevRB = PrevLT + m_Size;

	Vector2 LT = m_Pos - m_Pivot * m_Size;
	Vector2 RB = LT + m_Size;

	CTileMap* TileMap = m_Scene->GetTileMap();

	// ���� ������ ��ġ�� ���� ��ġ�� ����
	// �� ������ �̵��� ���̸�ŭ�� �簢�� ������ ���Ѵ�.
	Vector2 ResultLT, ResultRB;
	ResultLT = LT;

	ResultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
	ResultRB.x = PrevLT.x;
	ResultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

	// ���� �簢���� �� �𼭸��� ��� �ִ� Ÿ���� �ε����� ���Ѵ�.
	int LeftIndex = TileMap->GetTileOriginIndexX(ResultLT.x);
	int TopIndex = TileMap->GetTileOriginIndexY(ResultLT.y);
	int RightIndex = TileMap->GetTileOriginIndexX(ResultRB.x);
	int BottomIndex = TileMap->GetTileOriginIndexY(ResultRB.y);

	LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
	TopIndex = TopIndex < 0 ? 0 : TopIndex;
	RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
	BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

	// ��� Ÿ�� �ε����� Ÿ�ϸ� �ȿ� ���� ���� �浹 üũ�� �Ѵ�.
	if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
		RightIndex >= 0 && BottomIndex >= 0)
	{
		bool SideCollision = false;

		for (int i = TopIndex; i <= BottomIndex; i++)
		{
			for (int j = RightIndex; j >= LeftIndex; j--)
			{
				CTile* Tile = TileMap->GetTile(j, i);

				// Ÿ���� �̵��Ұ� �ɼ��� �ƴϰų� ���̵� �ݸ����� ��Ȱ��ȭ�Ǿ� ������ ����
				if (Tile->GetOption() != ETile_Option::ImpossibleToMove ||
					!Tile->GetSideCollision())
					continue;

				Vector2 TilePos = Tile->GetPos();
				Vector2 TileSize = Tile->GetSize();

				// ������Ʈ�� ���� �̵� �� Ÿ�Ͽ� �ɸ��� ���� ������ �ʵ���
				// �ణ�� �������� �д�.
				if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
					continue;

				// ������Ʈ�� Left�� Ÿ���� Right�� �����Ÿ� �̻� �Ѿ�ٸ�
				// �浹�� Ȯ���� �ʿ䰡 ����.
				/*if (TilePos.x + TileSize.x - LT.x > 8.f)
					continue;*/

				// ���� ��ġ�� �簢���� Ÿ���� �浹���Ѽ� �İ������� �Ǵ��Ѵ�.
				if (LT.x <= TilePos.x + TileSize.x && LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x && RB.y >= TilePos.y)
				{
					SideCollision = true;

					// �󸶳� �İ������� ���Ѵ�.(����� ������)
					float MoveX = TilePos.x + TileSize.x - LT.x + 0.001f;

					// �İ�� �Ÿ���ŭ ������Ʈ�� �̵������ش�.(Ÿ�Ͽ� ���� �ʵ���)
					m_Pos.x += MoveX;
					//m_Move.x += MoveX;
					break;
				}
			}

			// �浹�� �ߴٸ� �ݺ����� �������´�.
			if (SideCollision)
				break;
		}
	}
}
