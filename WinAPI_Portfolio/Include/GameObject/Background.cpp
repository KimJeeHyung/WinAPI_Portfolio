#include "Background.h"

CBackground::CBackground()
{
	SetTypeID<CBackground>();

	m_RenderLayer = ERender_Layer::Back;
}

CBackground::CBackground(const CBackground& Obj) :
	CGameObject(Obj)
{
}

CBackground::~CBackground()
{
}

bool CBackground::Init()
{
	SetPos(0.f, 0.f);
	SetSize(1280.f, 720.f);
	SetPivot(0.f, 0.f);

	SetTexture("Background", TEXT("clear_00s.bmp"));

	return true;
}

void CBackground::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBackground::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}
