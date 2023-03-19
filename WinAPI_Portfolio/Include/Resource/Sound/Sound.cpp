#include "Sound.h"
#include "../../PathManager.h"

CSound::CSound() :
	m_System(nullptr),
	m_Sound(nullptr),
	m_Group(nullptr),
	m_Channel(nullptr),
	m_Play(false),
	m_Loop(false),
	m_Pause(false)
{
}

CSound::~CSound()
{
	if (m_Sound)
	{
		m_Sound->release();
	}
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop,
	const char* FileName, const std::string& PathName)
{
	m_System = System;
	m_Group = Group;
	m_Loop = Loop;

	// 사운드 파일이 있는 폴더까지의 경로를 가져온다.
	char FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	FMOD_MODE Mode = FMOD_DEFAULT;

	// 사운드를 반복해서 재생시킬 경우 모드를 Loop로 바꿔준다.
	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	if (m_System->createSound(FullPath, Mode, nullptr, &m_Sound) != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_Group, false, &m_Channel);
	m_Play = true;
	m_Pause = false;
}

void CSound::Stop()
{
	// 정지와 일시정지 기능은 반드시 채널이 있어야 한다.
	if (m_Channel)
	{
		bool Playing = false;

		m_Channel->isPlaying(&Playing);

		if (Playing)
		{
			m_Channel->stop();
			m_Channel = nullptr;

			m_Play = false;
			m_Pause = false;
		}
	}
}

void CSound::Pause()
{
	if (m_Channel)
	{
		bool Playing = false;

		m_Channel->isPlaying(&Playing);

		if (Playing)
			m_Channel->setPaused(true);

		m_Play = false;
		m_Pause = true;
	}
}

void CSound::Resume()
{
	if (m_Channel)
	{
		if (m_Pause)
		{
			m_Channel->setPaused(false);

			m_Play = true;
			m_Pause = false;
		}
	}
}
