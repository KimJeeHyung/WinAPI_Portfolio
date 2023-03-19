#include "AnimationManager.h"
#include "AnimationSequence.h"
#include "../Texture/Texture.h"

CAnimationManager::CAnimationManager()
{
}

CAnimationManager::~CAnimationManager()
{
}

bool CAnimationManager::Init()
{
	return true;
}

bool CAnimationManager::CreateAniamtionSequence(const std::string& Name, CTexture* Texture)
{
	CAnimationSequence* Sequence = FindAnimation(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);
	Sequence->m_Texture = Texture;

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::AddAnimationFrame(const std::string& Name, 
	const Vector2& Start, const Vector2& End)
{
	CAnimationSequence* Sequence = FindAnimation(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrame(Start, End);

	return true;
}

bool CAnimationManager::AddAnimationFrame(const std::string& Name, 
	float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimation(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrame(PosX, PosY, SizeX, SizeY);

	return true;
}

CAnimationSequence* CAnimationManager::FindAnimation(const std::string& Name)
{
	std::unordered_map<std::string, CSharedPtr<class CAnimationSequence>>::iterator iter;
	iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseAnimation(const std::string& Name)
{
	std::unordered_map<std::string, CSharedPtr<class CAnimationSequence>>::iterator iter;
	iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return;

	// 애니메이션 시퀀스의 레퍼런스 카운트가 1이라면 트리에서 애니메이션 시퀀스를 제거한다.
	// (0에서 제거하면 댕글링이라 에러 발생)
	if (iter->second->GetRefCount() == 1)
		m_mapSequence.erase(iter);
}
