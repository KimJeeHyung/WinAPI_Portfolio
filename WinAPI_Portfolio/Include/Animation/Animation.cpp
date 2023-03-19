#include "Animation.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CAnimation::CAnimation() :
    m_Owner(nullptr),
    m_Scene(nullptr),
    m_CurrentAnimation(nullptr)
{
}

CAnimation::~CAnimation()
{
    std::unordered_map<std::string, class CAnimationInfo*>::iterator iter;
    std::unordered_map<std::string, class CAnimationInfo*>::iterator iterEnd;

    iter = m_mapAnimation.begin();
    iterEnd = m_mapAnimation.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

void CAnimation::AddAnimation(const std::string& SequenceName, bool Loop, 
    float PlayTime, float PlayScale, bool Reverse)
{
    if (FindInfo(SequenceName))
        return;

    CAnimationSequence* Sequence = nullptr;

    // 씬이 있으면 SceneResource에서, 없으면 ResourceManager에서 시퀀스를 받아온다.
    if (m_Scene)
    {
        Sequence = m_Scene->GetSceneResource()->FindAnimation(SequenceName);
    }
    else
    {
        Sequence = CResourceManager::GetInst()->FindAnimation(SequenceName);
    }

    if (!Sequence)
        return;

    CAnimationInfo* Info = new CAnimationInfo;

    Info->m_Sequence = Sequence;
    Info->m_Loop = Loop;
    Info->m_PlayTime = PlayTime;
    Info->m_PlayScale = PlayScale;
    Info->m_Reverse = Reverse;
    Info->m_FrameTime = Info->m_PlayTime / Sequence->GetFrameCount();

    // 처음 추가되는 애니메이션일 경우 현재 재생 애니메이션으로 기본 지정해준다.
    if (m_mapAnimation.empty())
        m_CurrentAnimation = Info;

    m_mapAnimation.insert(std::make_pair(SequenceName, Info));
}

void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->m_PlayTime = PlayTime;
}

void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{
    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->m_PlayScale = PlayScale;
}

void CAnimation::SetPlayLoop(const std::string& Name, bool Loop)
{
    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->m_Loop = Loop;
}

void CAnimation::SetPlayReverse(const std::string& Name, bool Reverse)
{
    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->m_Reverse = Reverse;
}

void CAnimation::SetCurrentAniamtion(std::string& Name)
{
    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    m_CurrentAnimation = Info;

    // 지정한 애니메이션을 처음부터 재생하기 위해 초기화 해준다.
    m_CurrentAnimation->m_Frame = 0;
    m_CurrentAnimation->m_Time = 0.f;

    size_t Size = m_CurrentAnimation->m_vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->m_vecNotify[i]->Call = false;
    }
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
    // 바꿀 애니메이션이 현재 애니메이션과 똑같으면 바꿀 필요가 없으므로 바로 빠져나간다.
    if (m_CurrentAnimation->m_Sequence->GetName() == Name)
        return;

    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    // 기존에 재생되던 애니메이션을 초기화한다.
    m_CurrentAnimation->m_Frame = 0;
    m_CurrentAnimation->m_Time = 0.f;

    size_t Size = m_CurrentAnimation->m_vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->m_vecNotify[i]->Call = false;
    }

    m_CurrentAnimation = Info;

    // 새로 바꾼 애니메이션을 처음부터 재생하기 위해 초기화 해준다.
    m_CurrentAnimation->m_Frame = 0;
    m_CurrentAnimation->m_Time = 0.f;

    Size = m_CurrentAnimation->m_vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->m_vecNotify[i]->Call = false;
    }
}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
    return m_CurrentAnimation->m_Sequence->GetName() == Name;
}

void CAnimation::Update(float DeltaTime)
{
    // DeltaTime에 PlayScale을 곱해 애니메이션 재생 속도를 조절한다.
    m_CurrentAnimation->m_Time += DeltaTime * m_CurrentAnimation->m_PlayScale;

    // 애니메이션 재생이 끝났는지 여부
    bool AnimationEnd = false;

    // 진행된 시간이 1프레임 당 흘러야 할 시간을 지났다면 프레임을 증가시킨다.
    if (m_CurrentAnimation->m_Time >= m_CurrentAnimation->m_FrameTime)
    {
        // 1프레임이 흘러야 할 시간을 빼서 다시 계산하게 만들어준다.
        m_CurrentAnimation->m_Time -= m_CurrentAnimation->m_FrameTime;

        if (m_CurrentAnimation->m_Reverse)
        {
            // 애니메이션이 역재생 중이라면 프레임을 감소
            --m_CurrentAnimation->m_Frame;

            if (m_CurrentAnimation->m_Frame < 0)
                AnimationEnd = true;
        }
        else
        {
            // 역재생이 아니라면 프레임을 증가
            ++m_CurrentAnimation->m_Frame;

            if (m_CurrentAnimation->m_Frame == m_CurrentAnimation->m_Sequence->GetFrameCount())
                AnimationEnd = true;
        }
    }

    size_t Size = m_CurrentAnimation->m_vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        // 현재 모션의 노티파이가 호출이 안 되었고 호출될 프레임에 도달했다면 호출한다.
        if (!m_CurrentAnimation->m_vecNotify[i]->Call &&
            m_CurrentAnimation->m_vecNotify[i]->Frame == m_CurrentAnimation->m_Frame)
        {
            m_CurrentAnimation->m_vecNotify[i]->Call = true;
            m_CurrentAnimation->m_vecNotify[i]->Function();
        }
    }

    if (AnimationEnd)
    {
        // 애니메이션이 끝나고 호출할 함수가 있다면 실행
        if (m_CurrentAnimationEndFunc)
            m_CurrentAnimationEndFunc();

        // 애니메이션이 루프 중이라면
        if (m_CurrentAnimation->m_Loop)
        {
            if (m_CurrentAnimation->m_Reverse)
            {
                // 역재생일 경우 마지막 프레임으로 돌아간다.
                m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;
            }
            else
            {
                // 역재생이 아니라면 처음부터 시작
                m_CurrentAnimation->m_Frame = 0;
            }
        }
        // 루프가 아니라면 각각 마지막 프레임에서 멈춰있게 한다.
        else
        {
            if (m_CurrentAnimation->m_Reverse)
            {
                m_CurrentAnimation->m_Frame = 0;
            }
            else
            {
                m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;
            }
        }

        // End Function 호출
        if (m_CurrentAnimation->m_EndFunction)
            m_CurrentAnimation->m_EndFunction();

        // 애니메이션이 끝나면 노티파이를 다시 사용할 수 있게 해준다.
        for (size_t i = 0; i < Size; i++)
        {
            m_CurrentAnimation->m_vecNotify[i]->Call = false;
        }
    }
}

void CAnimation::ResetAnimation(const std::string& Name)
{
}

CAnimationInfo* CAnimation::FindInfo(const std::string& Name)
{
    std::unordered_map<std::string, class CAnimationInfo*>::iterator iter;
    iter = m_mapAnimation.find(Name);

    if (iter == m_mapAnimation.end())
        return nullptr;

    return iter->second;
}
