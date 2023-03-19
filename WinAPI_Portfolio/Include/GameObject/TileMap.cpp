#include "TileMap.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "Tile.h"
#include "../Scene/SceneManager.h"

CTileMap::CTileMap() :
	m_CountX(0),
	m_CountY(0),
	m_StartX(-1),
	m_StartY(-1),
	m_EndX(-1),
	m_EndY(-1)
{
	SetTypeID<CTileMap>();

	m_RenderLayer = ERender_Layer::Tile;
}

CTileMap::CTileMap(const CTileMap& Obj) :
	CGameObject(Obj)
{
}

CTileMap::~CTileMap()
{
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}
}

void CTileMap::ChangeTileOption(const Vector2& Pos, ETile_Option Option)
{
	int Index = GetTileIndex(Pos);

	if (Index == -1)
		return;

	m_vecTile[Index]->SetTileOption(Option);
}

void CTileMap::SetTileFrame(const Vector2& Pos, const Vector2& Start,
	const Vector2& End)
{
	int Index = GetTileIndex(Pos);

	if (Index == -1)
		return;

	m_vecTile[Index]->SetFrame(Start, End);
}

void CTileMap::SetTileRender(const Vector2& Pos, bool Render)
{
	int Index = GetTileIndex(Pos);

	if (Index == -1)
		return;

	m_vecTile[Index]->SetRender(Render);
}

void CTileMap::SetTileSideCollision(const Vector2& Pos, bool Side)
{
	int Index = GetTileIndex(Pos);

	if (Index == -1)
		return;

	m_vecTile[Index]->SetSideCollision(Side);
}

CTile* CTileMap::GetTile(const Vector2& Pos)
{
	int Index = GetTileIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMap::GetTile(int Index)
{
	return m_vecTile[Index];
}

CTile* CTileMap::GetTile(int IndexX, int IndexY)
{
	return m_vecTile[IndexY * m_CountX + IndexX];
}

int CTileMap::GetTileIndexX(float x)
{
	// 타일의 가로 인덱스를 구한다.
	int IndexX = (int)(x / m_TileSize.x);

	// 인덱스 값이 0보다 작거나 최대 개수보다 크다면 잘못된 인덱스라는 뜻으로 -1 리턴
	if (IndexX < 0 || IndexX >= m_CountX)
		return -1;

	return IndexX;
}

int CTileMap::GetTileIndexY(float y)
{
	// 타일의 세로 인덱스를 구한다.
	int IndexY = (int)(y / m_TileSize.y);

	// 인덱스 값이 0보다 작거나 최대 개수보다 크다면 잘못된 인덱스라는 뜻으로 -1 리턴
	if (IndexY < 0 || IndexY >= m_CountY)
		return -1;

	return IndexY;
}

int CTileMap::GetTileIndex(const Vector2& Pos)
{
	int IndexX = GetTileIndexX(Pos.x);
	int IndexY = GetTileIndexY(Pos.y);

	// 가로 세로 둘 중 하나라도 -1이면 잘못된 인덱스를 구했다는 뜻이다.
	if (IndexX == -1 || IndexY == -1)
		return -1;

	// 실제 타일 목록에서의 인덱스 값을 리턴한다.
	return IndexY * m_CountX + IndexX;
}

int CTileMap::GetTileOriginIndexX(float x)
{
	return (int)(x / m_TileSize.x);
}

int CTileMap::GetTileOriginIndexY(float y)
{
	return (int)(y / m_TileSize.y);
}

bool CTileMap::CreateTile(int CountX, int CountY, const Vector2& TileSize)
{
	m_CountX = CountX;
	m_CountY = CountY;
	m_TileSize = TileSize;

	// 타일을 새로 만들기 전 타일을 모두 지워준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	// 타일맵의 전체 크기는 (타일 한 개의 크기)X(가로, 세로 개수)이다.
	m_Size = m_TileSize * Vector2((float)m_CountX, (float)m_CountY);

	// 게임 내 월드의 해상도를 타일맵 크기로 잡아준다.
	m_Scene->GetCamera()->SetWorldResolution(m_Size);

	// 세로 줄
	for (int i = 0; i < m_CountY; i++)
	{
		// 가로 줄
		for (int j = 0; j < m_CountX; j++)
		{
			CTile* Tile = new CTile;

			Tile->m_Scene = m_Scene;
			Tile->m_Owner = this;

			// 타일의 크기에 맞춰 위치를 정해준다.
			Vector2 Pos = m_TileSize * Vector2((float)j, (float)i);

			// 새로 생성한 타일의 정보를 입력한다.
			Tile->SetTileInfo(Pos, m_TileSize, j, i, i * m_CountX + j, m_TileTexture);

			m_vecTile.push_back(Tile);
		}
	}

	return true;
}

void CTileMap::SetTileTexture(const std::string& Name)
{
	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();
	
	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

void CTileMap::SetTileTexture(CTexture* Texture)
{
	m_TileTexture = Texture;

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

void CTileMap::SetTileTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

void CTileMap::SetTileTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

#ifdef UNICODE

void CTileMap::SetTileTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

void CTileMap::SetTileTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

#else

void CTileMap::SetTileTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

void CTileMap::SetTileTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_TileTexture = m_Scene->GetSceneResource()->FindTexture(Name);

	// 타일 목록에 있는 모든 타일에 텍스처를 넣어준다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->m_Texture = m_TileTexture;
	}
}

#endif // UNICODE

bool CTileMap::SetTileColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (!m_TileTexture)
		return false;

	m_TileTexture->SetColorKey(r, g, b, Index);

	return true;
}

bool CTileMap::SetTileColorKeyAll(unsigned char r, unsigned char g, unsigned char b)
{
	if (!m_TileTexture)
		return false;

	m_TileTexture->SetColorKeyAll(r, g, b);

	return true;
}

bool CTileMap::Init()
{
	/*SetPos(0.f, 0.f);
	SetSize(1500.f, 1200.f);
	SetPivot(0.f, 0.f);

	SetTileTexture("Background", TEXT("Sky.bmp"));*/

	// 자신이 속한 씬의 타일맵을 자신으로 지정해준다.
	m_Scene->SetTileMap(this);

	return true;
}

void CTileMap::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (!m_vecTile.empty())
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

		// 좌상단 타일의 인덱스는 카메라 위치를 타일 크기로 나누어 구한다.
		m_StartX = (int)(CameraPos.x / m_TileSize.x);
		m_StartY = (int)(CameraPos.y / m_TileSize.y);

		// 우하단 타일의 인덱스는 화면 전체 크기(카메라 + 해상도)를 타일 크기로 나누어 구한다.
		m_EndX = (int)((CameraPos.x + Resolution.x) / m_TileSize.x);
		m_EndY = (int)((CameraPos.y + Resolution.y) / m_TileSize.y);

		m_StartX = m_StartX < 0 ? 0 : m_StartX;
		m_StartY = m_StartY < 0 ? 0 : m_StartY;

		m_EndX = m_EndX >= m_CountX ? m_CountX - 1 : m_EndX;
		m_EndY = m_EndY >= m_CountY ? m_CountY - 1 : m_EndY;
	}
}

void CTileMap::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);

	// 세로 줄
	for (int i = m_StartY; i <= m_EndY; i++)
	{
		// 가로 줄
		for (int j = m_StartX; j <= m_EndX; j++)
		{
			// 화면 안에 있는 타일만 출력해준다.
			m_vecTile[i * m_CountX + j]->Render(hDC);
		}
	}
}

void CTileMap::Save(FILE* File)
{
	CGameObject::Save(File);

	bool Texture = false;

	if (m_TileTexture)
		Texture = true;

	fwrite(&Texture, sizeof(bool), 1, File);

	if (m_TileTexture)
		m_TileTexture->Save(File);

	fwrite(&m_CountX, sizeof(int), 1, File);
	fwrite(&m_CountY, sizeof(int), 1, File);
	fwrite(&m_TileSize, sizeof(Vector2), 1, File);

	int TileCount = (int)m_vecTile.size();
	fwrite(&TileCount, sizeof(int), 1, File);

	for (int i = 0; i < TileCount; i++)
	{
		m_vecTile[i]->Save(File);
	}
}

void CTileMap::Load(FILE* File)
{
	CGameObject::Load(File);

	bool Texture = false;

	fread(&Texture, sizeof(bool), 1, File);

	if (Texture)
		m_TileTexture = m_Scene->GetSceneResource()->LoadTexture(File);

	fread(&m_CountX, sizeof(int), 1, File);
	fread(&m_CountY, sizeof(int), 1, File);
	fread(&m_TileSize, sizeof(Vector2), 1, File);

	int TileCount = 0;
	fread(&TileCount, sizeof(int), 1, File);

	for (int i = 0; i < TileCount; i++)
	{
		CTile* Tile = new CTile;

		Tile->m_Owner = this;
		Tile->m_Scene = m_Scene;
		Tile->Load(File);

		m_vecTile.push_back(Tile);
	}

	m_Scene->GetCamera()->SetWorldResolution(m_Size);
}
