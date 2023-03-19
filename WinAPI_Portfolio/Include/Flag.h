#pragma once

// 애니메이션 텍스처의 이미지 타입
enum class ETexture_Type
{
	Sprite,		// 연속된 이미지가 한 파일 내에 있는 경우
	Frame		// 연속된 이미지가 개별 파일로 있는 경우
};

// 씬에서 오브젝트 출력시 묶어줄 레이어 그룹
enum class ERender_Layer
{
	Back,		// 배경
	Tile,		// 타일(타일맵)
	Default,	// 일반
	Effect,		// 이펙트
	Max
};

// 충돌 채널
enum class ECollision_Channel
{
	Default,
	Player,
	Opponent,
	PlayerAttack,
	OpponentAttack,
	Mouse,
	Max
};

// 충돌 시 상호작용
enum class ECollision_Interaction
{
	Ignore,		// 무시
	Collision	// 충돌
};

// 충돌체 타입
enum class ECollider_Type
{
	Box,
	Circle
};

// HBRUSH에 지정할 색깔 종류
enum class EBrush_Type
{
	Red,
	Green,
	Black,
	Blue,
	Yellow,
	Max
};

// 이펙트 종류
enum class EEffect_Type
{
	Once,		// 한번만 재생
	Duration,	// 일정 기간 동안 재생
	Loop		// 반복
};

// 버튼 UI 상태
enum class EButton_State
{
	Normal,			// 일반
	MouseHovered,	// 마우스가 올라가 있을 때
	Click,			// 클릭했을 때
	Checked,		// 체크 활성화(체크박스 용)
	Disable,		// 비활성화
	Max
};

// 버튼 UI 사운드
enum class EButton_Sound_State
{
	MouseHovered,	// 마우스가 올라가 있을 때
	Click,			// 클릭했을 때
	Max
};

// 타일맵의 각 타일에 대한 속성
enum class ETile_Option
{
	Normal,				// 기본
	ImpossibleToMove	// 이동 불가(벽 등)
};
