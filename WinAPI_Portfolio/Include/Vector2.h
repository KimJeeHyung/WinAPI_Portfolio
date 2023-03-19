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
		// 두 점을 잇는 직선을 빗변으로 갖는 직각삼각형의 밑변과 높이를 구한다.
		float Width = v.x - x;
		float Height = v.y - y;

		// 밑변과 높이를 이용해 빗변의 길이(두 점 사이의 거리)를 구한다.
		// sqrtf : 루트
		float c = sqrtf(Width * Width + Height * Height);

		// 밑변과 빗변을 이용해 cosθ값을 구한다,
		float Angle = Width / c;

		// acos을 이용해 θ(각도)를 구한 후 라디안에서 일반 각도로 전환
		Angle = acosf(Angle) / PI * 180.f;

		// 목적지의 y좌표가 내 y좌표보다 작을 경우(즉 목적지가 나보다 위에 있을 경우)
		// 360도에서 구한 각도만큼 빼준다.
		if (v.y < y)
			Angle = 360.f - Angle;

		return Angle;
	}

	// 두 점 사이의 거리를 계산 후 리턴하는 함수(피타고라스 정리 이용)
	float Distance(const Vector2& v) const
	{
		float Width = x - v.x;
		float Height = y - v.y;

		return sqrtf(Width * Width + Height * Height);
	}
};