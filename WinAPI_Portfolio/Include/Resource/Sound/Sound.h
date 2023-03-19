#pragma once
#include "../../Ref.h"
class CSound :
    public CRef
{
    friend class CSoundManager;

private:
    CSound();
    ~CSound();

private:
    FMOD::System*       m_System;
    FMOD::Sound*        m_Sound;
    FMOD::ChannelGroup* m_Group;
    FMOD::Channel*      m_Channel;  // 사운드를 재생하고 있는 채널
    bool                m_Play;     // 현재 재생 중인지
    bool                m_Loop;     // 반복 재생 여부
    bool                m_Pause;    // 현재 일시정지 중인지

public:
    bool GetPlay() const
    {
        return m_Play;
    }

    bool GetLoop() const
    {
        return m_Loop;
    }

public:
    bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop,
        const char* FileName, const std::string& PathName = SOUND_PATH);
    void Play();    // 재생
    void Stop();    // 정지
    void Pause();   // 일시정지
    void Resume();  // 재시작
};

