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
	// 충돌체 제거
	{
		std::list<CSharedPtr<CCollider>>::iterator iter;
		std::list<CSharedPtr<CCollider>>::iterator iterEnd;

		iter = m_ColliderList.begin();
		iterEnd = m_ColliderList.end();

		for (; iter != iterEnd; ++iter)
		{
			// 상대방이 가지고 있는 자신의 충돌체를 지워서
			// 레퍼런스 카운트를 0으로 만든다.(안 하면 메모리 릭 남음)
			(*iter)->ClearCollisionList();
		}
	}

	// 위젯 컴포넌트 제거
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

	// 오브젝트의 이전 위치를 현재 위치로 지정해 처음부터 떨어질 수 있도록 한다.
	m_PrevPos = m_Pos;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	// 중력 적용
	if (!m_Ground && m_PhysicsSimulate)
	{
		// 떨어지는 시간을 누적시켜준다.
		m_FallTime += DeltaTime * m_GravityAccel;

		// 9.8m/s^2
		// t초 후 y값
		// V : 속도 A : 가속도 G : 중력
		// y = V * A - 0.5 * G * t * t
		// 0 = -0.5GA^2 VA - y
		float Velocity = 0.f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);
	}

	// 애니메이션 Update
	if (m_Animation)
		m_Animation->Update(DeltaTime * m_TimeScale);

	// 충돌체 Update
	{
		std::list<CSharedPtr<CCollider>>::iterator iter;
		std::list<CSharedPtr<CCollider>>::iterator iterEnd;

		iter = m_ColliderList.begin();
		iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			// 충돌체가 없어졌다면 목록에서 지우고 다음 충돌체로 넘어간다.
			if (!(*iter)->GetActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			// 충돌체가 비활성화 되어있다면 바로 다음 충돌체로 넘어간다.
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}

	// 위젯 컴포넌트 Update
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			// 위젯이 없어졌다면 목록에서 지우고 다음 위젯으로 넘어간다.
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}
			// 위젯이 비활성화 되어있다면 바로 다음 위젯으로 넘어간다.
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
	// 현재 위치에서 이전 위치를 빼서 이동량 계산
	m_Move = m_Pos - m_PrevPos;

	// 애니메이션이 동작될 경우 이미지 크기로 사이즈를 결정하기 때문에
	// PostUpdate에서 사이즈를 다시 구해주도록 한다.
	if (m_Animation)
	{
		CAnimationInfo* Current = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = Current->m_Sequence->GetFrame(Current->m_Frame);

		//m_Size = FrameData.End - FrameData.Start;
	}

	// 충돌체 PostUpdate
	{
		std::list<CSharedPtr<CCollider>>::iterator iter;
		std::list<CSharedPtr<CCollider>>::iterator iterEnd;

		iter = m_ColliderList.begin();
		iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			// 충돌체가 없어졌다면 지우고 다음 충돌체로 넘어간다.
			if (!(*iter)->GetActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			// 충돌체가 비활성화 되어있다면 바로 다음 충돌체로 넘어간다.
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

	// 위젯 컴포넌트 PostUpdate
	{
		std::list<CSharedPtr<CWidgetComponent>>::iterator iter;
		std::list<CSharedPtr<CWidgetComponent>>::iterator iterEnd;

		iter = m_WidgetComponentList.begin();
		iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			// 위젯이 없어졌다면 목록에서 지우고 다음 위젯으로 넘어간다.
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}
			// 위젯이 비활성화 되어있다면 바로 다음 위젯으로 넘어간다.
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);
			++iter;
		}
	}

	// 좌우로 움직이다가 벽에 부딪혔을 경우
	if (m_Move.x != 0.f && m_SideWallCheck)
	{
		// 오른쪽으로 이동할 때
		if (m_Move.x > 0.f)
		{
			CheckMoveRight();
		}
		// 왼쪽으로 이동할 때
		else
		{
			CheckMoveLeft();
		}
	}

	// 바닥에 착지시킨다.
	if (m_PhysicsSimulate && m_Move.y >= 0.f)
	{
		CTileMap* TileMap = m_Scene->GetTileMap();

		// 오브젝트의 이전 프레임 위치와 현재 위치의 Left, Right, Bottom을 이용해
		// 한 프레임 이동한 차이만큼의 사각형 영역을 구한다.
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

		// 구한 사각형의 각 모서리에 닿고 있는 타일의 인덱스를 구한다.
		int LeftIndex = TileMap->GetTileOriginIndexX(ResultLeft);
		int RightIndex = TileMap->GetTileOriginIndexX(ResultRight);
		int TopIndex = TileMap->GetTileOriginIndexY(ResultTop);
		int BottomIndex = TileMap->GetTileOriginIndexY(ResultBottom);

		LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
		TopIndex = TopIndex < 0 ? 0 : TopIndex;
		RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
		BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

		// 모든 타일 인덱스가 타일맵 안에 있을 때만 충돌 체크를 한다.
		if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
			RightIndex >= 0 && BottomIndex >= 0)
		{
			bool Check = false;

			// 위에서 아래로 차례로 검사를 해나간다.
			// 아래에서부터 하게 되면 위에 벽이 있을 경우 무시하고 처리돼버릴 수도 있기 때문이다.
			for (int i = TopIndex; i <= BottomIndex; i++)
			{
				for (int j = LeftIndex; j <= RightIndex; j++)
				{
					CTile* Tile = TileMap->GetTile(j, i);

					// 현재 타일이 없으면 무시
					if (!Tile)
						continue;

					// 현재 타일의 Top이 오브젝트의 이전 위치의 Bottom보다 위에 있어도 무시
					if (Tile->GetPos().y < PrevBottom)
						continue;

					// 현재 타일이 이동불가 타일일 경우 바닥을 밟았다는 뜻이다.
					if (Tile->GetOption() == ETile_Option::ImpossibleToMove)
					{
						Check = true;
						m_FallTime = 0.f;

						m_JumpCount = 0;

						// 현재 타일의 Top을 이 오브젝트의 Bottom값으로 지정한다.
						m_Pos.y = Tile->GetPos().y - (1.f - m_Pivot.y) * m_Size.y;
						m_Ground = true;
						m_Jump = false;
						break;
					}
				}

				// 바닥을 밟았다면 반복문을 빠져나온다.
				if (Check)
					break;
			}

			// 바닥이 없을 경우
			if (!Check)
			{
				// 땅을 밟고 있던 상태라면 이제 떨어지는 상태가 될 것이다.
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
	// 실제 오브젝트의 위치에서 카메라의 위치를 빼서 화면 상에 출력될 위치를 지정해준다.
	Vector2 Pos;
	Vector2 CameraPos;
	Vector2 Resolution;

	// 오브젝트마다 m_Scene이 있거나 없는 경우가 있기 때문에
	// 각각의 경우에 맞게 CameraPos와 Resolution을 구해준다.
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

		// 카메라 적용을 받지 않는 원래 위치
		Vector2 CullPos = m_Pos - m_Pivot * Size;

		// 오브젝트 컬링
		// 물체의 왼쪽 X좌표가 화면 밖을 벗어나면 출력하지 않는다.
		if (CullPos.x > CameraPos.x + Resolution.x)
			return;
		// 물체의 오른쪽 X좌표가 카메라 X좌표보다 작다면 출력하지 않는다.
		else if (CullPos.x + Size.x < CameraPos.x)
			return;
		// 물체의 왼쪽 Y좌표가 화면 밖을 벗어나면 출력하지 않는다.
		else if (CullPos.y > CameraPos.y + Resolution.y)
			return;
		// 물체의 오른쪽 Y좌표가 카메라 Y좌표보다 작다면 출력하지 않는다.
		else if (CullPos.y + Size.y < CameraPos.y)
			return;

		// 스프라이트 텍스처
		if (Current->m_Sequence->GetTextureType() == ETexture_Type::Sprite)
		{
			// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
			if (Current->m_Sequence->GetTexture()->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, 
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(),
					(int)FrameData.Start.x, (int)FrameData.Start.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetColorKey());
			}
			// 일반 비트맵 출력 함수
			else
			{
				BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(),
					(int)FrameData.Start.x, (int)FrameData.Start.y, SRCCOPY);
			}
		}
		// 프레임 텍스처
		else
		{
			// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
			if (Current->m_Sequence->GetTexture()->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(Current->m_Frame),
					(int)FrameData.Start.x, (int)FrameData.Start.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetColorKey());
			}
			// 일반 비트맵 출력 함수
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

			// 카메라 적용을 받지 않는 원래 위치
			Vector2 CullPos = m_Pos - m_Pivot * m_Size;

			// 오브젝트 컬링
			// 물체의 왼쪽 X좌표가 화면 밖을 벗어나면 출력하지 않는다.
			if (CullPos.x > CameraPos.x + Resolution.x)
				return;
			// 물체의 오른쪽 X좌표가 카메라 X좌표보다 작다면 출력하지 않는다.
			else if (CullPos.x + m_Size.x < CameraPos.x)
				return;
			// 물체의 왼쪽 Y좌표가 화면 밖을 벗어나면 출력하지 않는다.
			else if (CullPos.y > CameraPos.y + Resolution.y)
				return;
			// 물체의 오른쪽 Y좌표가 카메라 Y좌표보다 작다면 출력하지 않는다.
			else if (CullPos.y + m_Size.y < CameraPos.y)
				return;

			// 컬러키가 활성화되어있으면 해당 색깔만 출력하지 않는다.
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
			// 일반 비트맵 출력 함수
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
		// 충돌체가 없어졌다면 지우고 다음 충돌체로 넘어간다.
		if (!(*iter)->GetActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		// 충돌체가 비활성화 되어있다면 바로 다음 충돌체로 넘어간다.
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}

	// 출력이 끝날 때마다 이전 위치를 갱신한다.
	m_PrevPos = m_Pos;
}

// Damage만큼 피해를 입는다.
float CGameObject::InflictDamage(float Damage)
{
	return Damage;
}

// GameObject 클래스의 멤버 변수들을 저장한다.
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
		// 텍스처 저장
		m_Texture->Save(File);
	}

	bool	Animation = false;

	if (m_Animation)
		Animation = true;

	fwrite(&Animation, sizeof(bool), 1, File);

	if (m_Animation)
	{
		// 애니메이션 저장
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

// GameObject 클래스의 멤버 변수들을 불러온다.(저장한 순서대로 불러올 것)
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
		// 텍스처 불러오기
		m_Texture = m_Scene->GetSceneResource()->LoadTexture(File);
	}

	bool	Animation = false;

	fread(&Animation, sizeof(bool), 1, File);

	if (Animation)
	{
		// 애니메이션 불러오기
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
	// 오브젝트의 이전 프레임 위치와 현재 위치의 사각형 영역을 구한다.
	Vector2 PrevLT = m_PrevPos - m_Pivot * m_Size;
	Vector2 PrevRB = PrevLT + m_Size;

	Vector2 LT = m_Pos - m_Pivot * m_Size;
	Vector2 RB = LT + m_Size;

	CTileMap* TileMap = m_Scene->GetTileMap();

	// 이전 프레임 위치와 현재 위치를 비교해
	// 한 프레임 이동한 차이만큼의 사각형 영역을 구한다.
	Vector2 ResultLT, ResultRB;
	ResultRB = RB;

	ResultLT.x = PrevRB.x;
	ResultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
	ResultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

	// 구한 사각형의 각 모서리에 닿고 있는 타일의 인덱스를 구한다.
	int LeftIndex = TileMap->GetTileOriginIndexX(ResultLT.x);
	int TopIndex = TileMap->GetTileOriginIndexY(ResultLT.y);
	int RightIndex = TileMap->GetTileOriginIndexX(ResultRB.x);
	int BottomIndex = TileMap->GetTileOriginIndexY(ResultRB.y);

	LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
	TopIndex = TopIndex < 0 ? 0 : TopIndex;
	RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
	BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

	// 모든 타일 인덱스가 타일맵 안에 있을 때만 충돌 체크를 한다.
	if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
		RightIndex >= 0 && BottomIndex >= 0)
	{
		bool SideCollision = false;

		for (int i = TopIndex; i <= BottomIndex; i++)
		{
			for (int j = LeftIndex; j <= RightIndex; j++)
			{
				CTile* Tile = TileMap->GetTile(j, i);

				// 타일이 이동불가 옵션이 아니거나 사이드 콜리전이 비활성화되어 있으면 무시
				if (Tile->GetOption() != ETile_Option::ImpossibleToMove ||
					!Tile->GetSideCollision())
					continue;

				Vector2 TilePos = Tile->GetPos();
				Vector2 TileSize = Tile->GetSize();

				// 오브젝트가 평지 이동 중 타일에 걸리는 일이 생기지 않도록
				// 약간의 여윳값을 둔다.
				if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
					continue;

				// 오브젝트의 Right가 타일의 Left를 일정거리 이상 넘어갔다면
				// 충돌을 확인할 필요가 없다.
				/*if (RB.x - TilePos.x > 8.f)
					continue;*/

				// 현재 위치의 사각형과 타일을 충돌시켜서 파고들었는지 판단한다.
				if (LT.x <= TilePos.x + TileSize.x && LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x && RB.y >= TilePos.y)
				{
					SideCollision = true;

					// 얼마나 파고들었는지 구한다.(상수는 여윳값)
					float MoveX = TilePos.x - RB.x - 0.001f;

					// 파고든 거리만큼 오브젝트를 이동시켜준다.(타일에 끼지 않도록)
					m_Pos.x += MoveX;
					//m_Move.x += MoveX;
					break;
				}
			}

			// 충돌을 했다면 반복문을 빠져나온다.
			if (SideCollision)
				break;
		}
	}
}

void CGameObject::CheckMoveLeft()
{
	// 오브젝트의 이전 프레임 위치와 현재 위치의 사각형 영역을 구한다.
	Vector2 PrevLT = m_PrevPos - m_Pivot * m_Size;
	Vector2 PrevRB = PrevLT + m_Size;

	Vector2 LT = m_Pos - m_Pivot * m_Size;
	Vector2 RB = LT + m_Size;

	CTileMap* TileMap = m_Scene->GetTileMap();

	// 이전 프레임 위치와 현재 위치를 비교해
	// 한 프레임 이동한 차이만큼의 사각형 영역을 구한다.
	Vector2 ResultLT, ResultRB;
	ResultLT = LT;

	ResultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
	ResultRB.x = PrevLT.x;
	ResultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

	// 구한 사각형의 각 모서리에 닿고 있는 타일의 인덱스를 구한다.
	int LeftIndex = TileMap->GetTileOriginIndexX(ResultLT.x);
	int TopIndex = TileMap->GetTileOriginIndexY(ResultLT.y);
	int RightIndex = TileMap->GetTileOriginIndexX(ResultRB.x);
	int BottomIndex = TileMap->GetTileOriginIndexY(ResultRB.y);

	LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
	TopIndex = TopIndex < 0 ? 0 : TopIndex;
	RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
	BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

	// 모든 타일 인덱스가 타일맵 안에 있을 때만 충돌 체크를 한다.
	if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
		RightIndex >= 0 && BottomIndex >= 0)
	{
		bool SideCollision = false;

		for (int i = TopIndex; i <= BottomIndex; i++)
		{
			for (int j = RightIndex; j >= LeftIndex; j--)
			{
				CTile* Tile = TileMap->GetTile(j, i);

				// 타일이 이동불가 옵션이 아니거나 사이드 콜리전이 비활성화되어 있으면 무시
				if (Tile->GetOption() != ETile_Option::ImpossibleToMove ||
					!Tile->GetSideCollision())
					continue;

				Vector2 TilePos = Tile->GetPos();
				Vector2 TileSize = Tile->GetSize();

				// 오브젝트가 평지 이동 중 타일에 걸리는 일이 생기지 않도록
				// 약간의 여윳값을 둔다.
				if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
					continue;

				// 오브젝트의 Left가 타일의 Right를 일정거리 이상 넘어갔다면
				// 충돌을 확인할 필요가 없다.
				/*if (TilePos.x + TileSize.x - LT.x > 8.f)
					continue;*/

				// 현재 위치의 사각형과 타일을 충돌시켜서 파고들었는지 판단한다.
				if (LT.x <= TilePos.x + TileSize.x && LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x && RB.y >= TilePos.y)
				{
					SideCollision = true;

					// 얼마나 파고들었는지 구한다.(상수는 여윳값)
					float MoveX = TilePos.x + TileSize.x - LT.x + 0.001f;

					// 파고든 거리만큼 오브젝트를 이동시켜준다.(타일에 끼지 않도록)
					m_Pos.x += MoveX;
					//m_Move.x += MoveX;
					break;
				}
			}

			// 충돌을 했다면 반복문을 빠져나온다.
			if (SideCollision)
				break;
		}
	}
}
