#pragma once

#include "../../GameInfo.h"

/*사운드 로딩 및 재생은 API가 아니라 FMOD 라이브러리를 이용해 구현한다.*/
class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	// 사운드를 담아두는 트리
	// 메모리 관리를 하기 위해 SharedPtr로 만듦
	std::unordered_map<std::string, CSharedPtr<class CSound>> m_mapSound;
	
	FMOD::System* m_System;				// FMOD의 메인 시스템 클래스를 이용해 사운드 관리를 한다.
	FMOD::ChannelGroup* m_MasterGroup;	// 모든 사운드 채널을 제어할 수 있는 마스터 그룹
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapChannelGroup;	// BGM, 이펙트 등 종류별 사운드 채널 목록

public:
	bool Init();
	void Update();
	bool CreateSoundChannel(const std::string& Name);
	bool LoadSound(const std::string& GroupName, const std::string& Name,
		bool Loop, const char* FileName, const std::string& PathName);
	bool SetVolume(int Volume);									// 마스터볼륨
	bool SetVolume(const std::string& GroupName, int Volume);	// 채널그룹별 볼륨
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);
	void ReleaseSound(const std::string& Name);
};

