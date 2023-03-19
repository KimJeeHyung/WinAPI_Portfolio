#include "Camera.h"
#include "../Input.h"

CCamera::CCamera() :
	m_MaxDistance(1280.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::Update(float DeltaTime)
{
	if (m_Target && m_Target2)
	{
		// 각 타겟이 없어지면 nullptr로 바꿔준다.
		if (!m_Target->GetActive())
			m_Target = nullptr;
		else if (!m_Target2->GetActive())
			m_Target2 = nullptr;
		else
		{
			// 두 타겟 사이의 중심 좌표를 구한다.
			m_TargetPos = (m_Target->GetPos() + m_Target2->GetPos()) / 2;

			m_Pos = m_TargetPos - m_TargetPivot * m_Resolution + m_TargetOffset;
		}
	}

	//if (m_Target)
	//{
	//	// 타겟이 죽는 등의 이유로 없어지면 nullptr로 바꿔준다.
	//	if (!m_Target->GetActive())
	//		m_Target = nullptr;
	//	else
	//		m_Pos = m_Target->GetPos() - m_TargetPivot * m_Resolution + m_TargetOffset;
	//}

	// 카메라가 월드를 벗어나지 못하도록 만들어준다.
	if (m_Pos.x < 0.f)
		m_Pos.x = 0.f;
	else if (m_Pos.x + m_Resolution.x > m_WorldResolution.x)
		m_Pos.x = m_WorldResolution.x - m_Resolution.x;

	if (m_Pos.y < 0.f)
		m_Pos.y = 0.f;
	else if (m_Pos.y + m_Resolution.y > m_WorldResolution.y)
		m_Pos.y = m_WorldResolution.y - m_Resolution.y;

	CInput::GetInst()->ComputeWorldMousePos(m_Pos);
}
