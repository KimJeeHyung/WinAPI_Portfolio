#pragma once
#include "Scene.h"
class CMainScene :
    public CScene
{
	friend class CSceneManager;

protected:
	CMainScene();
	virtual ~CMainScene();

private:
	CSharedPtr<class CMonster> m_Monster;
	CSharedPtr<class COpponent> m_Opponent;

public:
	bool Init();
	void Update(float DeltaTime);

private:
	void CreateAnimationSequence();
	void CreateRagnaAnimation();
	void CreateNoelAnimation();

	void CreateSound();

private:
	void Cam1Key();
	void Cam2Key();
	void SetPlayable(bool Playable);
};

