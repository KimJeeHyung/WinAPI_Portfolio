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

    // ���� ������ SceneResource����, ������ ResourceManager���� �������� �޾ƿ´�.
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

    // ó�� �߰��Ǵ� �ִϸ��̼��� ��� ���� ��� �ִϸ��̼����� �⺻ �������ش�.
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

    // ������ �ִϸ��̼��� ó������ ����ϱ� ���� �ʱ�ȭ ���ش�.
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
    // �ٲ� �ִϸ��̼��� ���� �ִϸ��̼ǰ� �Ȱ����� �ٲ� �ʿ䰡 �����Ƿ� �ٷ� ����������.
    if (m_CurrentAnimation->m_Sequence->GetName() == Name)
        return;

    CAnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    // ������ ����Ǵ� �ִϸ��̼��� �ʱ�ȭ�Ѵ�.
    m_CurrentAnimation->m_Frame = 0;
    m_CurrentAnimation->m_Time = 0.f;

    size_t Size = m_CurrentAnimation->m_vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->m_vecNotify[i]->Call = false;
    }

    m_CurrentAnimation = Info;

    // ���� �ٲ� �ִϸ��̼��� ó������ ����ϱ� ���� �ʱ�ȭ ���ش�.
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
    // DeltaTime�� PlayScale�� ���� �ִϸ��̼� ��� �ӵ��� �����Ѵ�.
    m_CurrentAnimation->m_Time += DeltaTime * m_CurrentAnimation->m_PlayScale;

    // �ִϸ��̼� ����� �������� ����
    bool AnimationEnd = false;

    // ����� �ð��� 1������ �� �귯�� �� �ð��� �����ٸ� �������� ������Ų��.
    if (m_CurrentAnimation->m_Time >= m_CurrentAnimation->m_FrameTime)
    {
        // 1�������� �귯�� �� �ð��� ���� �ٽ� ����ϰ� ������ش�.
        m_CurrentAnimation->m_Time -= m_CurrentAnimation->m_FrameTime;

        if (m_CurrentAnimation->m_Reverse)
        {
            // �ִϸ��̼��� ����� ���̶�� �������� ����
            --m_CurrentAnimation->m_Frame;

            if (m_CurrentAnimation->m_Frame < 0)
                AnimationEnd = true;
        }
        else
        {
            // ������� �ƴ϶�� �������� ����
            ++m_CurrentAnimation->m_Frame;

            if (m_CurrentAnimation->m_Frame == m_CurrentAnimation->m_Sequence->GetFrameCount())
                AnimationEnd = true;
        }
    }

    size_t Size = m_CurrentAnimation->m_vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        // ���� ����� ��Ƽ���̰� ȣ���� �� �Ǿ��� ȣ��� �����ӿ� �����ߴٸ� ȣ���Ѵ�.
        if (!m_CurrentAnimation->m_vecNotify[i]->Call &&
            m_CurrentAnimation->m_vecNotify[i]->Frame == m_CurrentAnimation->m_Frame)
        {
            m_CurrentAnimation->m_vecNotify[i]->Call = true;
            m_CurrentAnimation->m_vecNotify[i]->Function();
        }
    }

    if (AnimationEnd)
    {
        // �ִϸ��̼��� ������ ȣ���� �Լ��� �ִٸ� ����
        if (m_CurrentAnimationEndFunc)
            m_CurrentAnimationEndFunc();

        // �ִϸ��̼��� ���� ���̶��
        if (m_CurrentAnimation->m_Loop)
        {
            if (m_CurrentAnimation->m_Reverse)
            {
                // ������� ��� ������ ���������� ���ư���.
                m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;
            }
            else
            {
                // ������� �ƴ϶�� ó������ ����
                m_CurrentAnimation->m_Frame = 0;
            }
        }
        // ������ �ƴ϶�� ���� ������ �����ӿ��� �����ְ� �Ѵ�.
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

        // End Function ȣ��
        if (m_CurrentAnimation->m_EndFunction)
            m_CurrentAnimation->m_EndFunction();

        // �ִϸ��̼��� ������ ��Ƽ���̸� �ٽ� ����� �� �ְ� ���ش�.
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
