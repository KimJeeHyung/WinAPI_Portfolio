#pragma once

// �ִϸ��̼� �ؽ�ó�� �̹��� Ÿ��
enum class ETexture_Type
{
	Sprite,		// ���ӵ� �̹����� �� ���� ���� �ִ� ���
	Frame		// ���ӵ� �̹����� ���� ���Ϸ� �ִ� ���
};

// ������ ������Ʈ ��½� ������ ���̾� �׷�
enum class ERender_Layer
{
	Back,		// ���
	Tile,		// Ÿ��(Ÿ�ϸ�)
	Default,	// �Ϲ�
	Effect,		// ����Ʈ
	Max
};

// �浹 ä��
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

// �浹 �� ��ȣ�ۿ�
enum class ECollision_Interaction
{
	Ignore,		// ����
	Collision	// �浹
};

// �浹ü Ÿ��
enum class ECollider_Type
{
	Box,
	Circle
};

// HBRUSH�� ������ ���� ����
enum class EBrush_Type
{
	Red,
	Green,
	Black,
	Blue,
	Yellow,
	Max
};

// ����Ʈ ����
enum class EEffect_Type
{
	Once,		// �ѹ��� ���
	Duration,	// ���� �Ⱓ ���� ���
	Loop		// �ݺ�
};

// ��ư UI ����
enum class EButton_State
{
	Normal,			// �Ϲ�
	MouseHovered,	// ���콺�� �ö� ���� ��
	Click,			// Ŭ������ ��
	Checked,		// üũ Ȱ��ȭ(üũ�ڽ� ��)
	Disable,		// ��Ȱ��ȭ
	Max
};

// ��ư UI ����
enum class EButton_Sound_State
{
	MouseHovered,	// ���콺�� �ö� ���� ��
	Click,			// Ŭ������ ��
	Max
};

// Ÿ�ϸ��� �� Ÿ�Ͽ� ���� �Ӽ�
enum class ETile_Option
{
	Normal,				// �⺻
	ImpossibleToMove	// �̵� �Ұ�(�� ��)
};
