#pragma once

#include <math.h>

const float PI = 3.14159f;

struct Vector2
{
	float x, y;

	Vector2() :
		x(0.f),
		y(0.f)
	{
	}

	Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	Vector2(const Vector2& v) :
		x(v.x),
		y(v.y)
	{
	}

	void operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	void operator = (float f)
	{
		x = f;
		y = f;
	}

	bool operator == (const Vector2& v) const
	{
		return x == v.x && y == v.y;
	}

	bool operator != (const Vector2& v) const
	{
		return x != v.x || y != v.y;
	}

	// +
	Vector2 operator + (const Vector2& v) const
	{
		Vector2 result;

		result.x = x + v.x;
		result.y = y + v.y;

		return result;
	}

	Vector2 operator + (float f) const
	{
		Vector2 result;

		result.x = x + f;
		result.y = y + f;

		return result;
	}

	Vector2 operator + (int i) const
	{
		Vector2 result;

		result.x = x + (float)i;
		result.y = y + (float)i;

		return result;
	}

	// +=
	void operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
	}

	void operator += (float f)
	{
		x += f;
		y += f;
	}

	void operator += (int i)
	{
		x += (float)i;
		y += (float)i;
	}

	// -
	Vector2 operator - (const Vector2& v) const
	{
		Vector2 result;

		result.x = x - v.x;
		result.y = y - v.y;

		return result;
	}

	Vector2 operator - (float f) const
	{
		Vector2 result;

		result.x = x - f;
		result.y = y - f;

		return result;
	}

	Vector2 operator - (int i) const
	{
		Vector2 result;

		result.x = x - (float)i;
		result.y = y - (float)i;

		return result;
	}

	// -=
	void operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	void operator -= (float f)
	{
		x -= f;
		y -= f;
	}

	void operator -= (int i)
	{
		x -= (float)i;
		y -= (float)i;
	}

	// *
	Vector2 operator * (const Vector2& v) const
	{
		Vector2 result;

		result.x = x * v.x;
		result.y = y * v.y;

		return result;
	}

	Vector2 operator * (float f) const
	{
		Vector2 result;

		result.x = x * f;
		result.y = y * f;

		return result;
	}

	Vector2 operator * (int i) const
	{
		Vector2 result;

		result.x = x * (float)i;
		result.y = y * (float)i;

		return result;
	}

	// *=
	void operator *= (const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
	}

	void operator *= (float f)
	{
		x *= f;
		y *= f;
	}

	void operator *= (int i)
	{
		x *= (float)i;
		y *= (float)i;
	}

	// /
	Vector2 operator / (const Vector2& v) const
	{
		Vector2 result;

		result.x = x / v.x;
		result.y = y / v.y;

		return result;
	}

	Vector2 operator / (float f) const
	{
		Vector2 result;

		result.x = x / f;
		result.y = y / f;

		return result;
	}

	Vector2 operator / (int i) const
	{
		Vector2 result;

		result.x = x / (float)i;
		result.y = y / (float)i;

		return result;
	}

	// /=
	void operator /= (const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
	}

	void operator /= (float f)
	{
		x /= f;
		y /= f;
	}

	void operator /= (int i)
	{
		x /= (float)i;
		y /= (float)i;
	}

	float Angle(const Vector2& v) const
	{
		// �� ���� �մ� ������ �������� ���� �����ﰢ���� �غ��� ���̸� ���Ѵ�.
		float Width = v.x - x;
		float Height = v.y - y;

		// �غ��� ���̸� �̿��� ������ ����(�� �� ������ �Ÿ�)�� ���Ѵ�.
		// sqrtf : ��Ʈ
		float c = sqrtf(Width * Width + Height * Height);

		// �غ��� ������ �̿��� cos�谪�� ���Ѵ�,
		float Angle = Width / c;

		// acos�� �̿��� ��(����)�� ���� �� ���ȿ��� �Ϲ� ������ ��ȯ
		Angle = acosf(Angle) / PI * 180.f;

		// �������� y��ǥ�� �� y��ǥ���� ���� ���(�� �������� ������ ���� ���� ���)
		// 360������ ���� ������ŭ ���ش�.
		if (v.y < y)
			Angle = 360.f - Angle;

		return Angle;
	}

	// �� �� ������ �Ÿ��� ��� �� �����ϴ� �Լ�(��Ÿ��� ���� �̿�)
	float Distance(const Vector2& v) const
	{
		float Width = x - v.x;
		float Height = y - v.y;

		return sqrtf(Width * Width + Height * Height);
	}
};