#pragma once

#include "../../GameInfo.h"

/*���� �ε� �� ����� API�� �ƴ϶� FMOD ���̺귯���� �̿��� �����Ѵ�.*/
class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	// ���带 ��Ƶδ� Ʈ��
	// �޸� ������ �ϱ� ���� SharedPtr�� ����
	std::unordered_map<std::string, CSharedPtr<class CSound>> m_mapSound;
	
	FMOD::System* m_System;				// FMOD�� ���� �ý��� Ŭ������ �̿��� ���� ������ �Ѵ�.
	FMOD::ChannelGroup* m_MasterGroup;	// ��� ���� ä���� ������ �� �ִ� ������ �׷�
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapChannelGroup;	// BGM, ����Ʈ �� ������ ���� ä�� ���

public:
	bool Init();
	void Update();
	bool CreateSoundChannel(const std::string& Name);
	bool LoadSound(const std::string& GroupName, const std::string& Name,
		bool Loop, const char* FileName, const std::string& PathName);
	bool SetVolume(int Volume);									// �����ͺ���
	bool SetVolume(const std::string& GroupName, int Volume);	// ä�α׷캰 ����
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);
	void ReleaseSound(const std::string& Name);
};

