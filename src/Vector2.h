#pragma once
#include <SDL/SDL.h>

struct vector2
{
public:
	float x = 0;
	float y = 0;

	vector2()
	{
		this->x = 0;
		this->y = 0;
	}

	vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void operator+=(const vector2& v)
	{
		this->x += v.x;
		this->y += v.y;
	}

	void operator-=(const vector2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}

	void operator*=(const vector2& v)
	{
		this->x *= v.x;
		this->y *= v.y;
	}

	void operator/=(const vector2& v)
	{
		this->x /= v.x;
		this->y /= v.y;
	}

	vector2 operator+(const vector2& v)
	{
		return vector2(this->x + v.x, this->y + v.y);
	}

	vector2 operator-(const vector2& v)
	{
		return vector2(this->x - v.x, this->y - v.y);
	}

	vector2 operator*(const vector2& v)
	{
		return vector2(this->x * v.x, this->y * v.y);
	}

	vector2 operator/(const vector2& v)
	{
		return vector2(this->x / v.x, this->y / v.y);
	}

	vector2 operator*(const float& f)
	{
		return vector2(this->x * f, this->y * f);
	}

	vector2 operator/(const float& f)
	{
		return vector2(this->x / f, this->y / f);
	}

	vector2 operator*=(const float& f)
	{
		return vector2(this->x * f, this->y * f);
	}

	vector2 operator+(const float& f)
	{
		return vector2(this->x + f, this->y + f);
	}

	float Magnitude()
	{
		return (float)sqrt(this->x * this->x + this->y * this->y);
	};

	vector2 Normalize()
	{
		float distance = Magnitude();
		return vector2(this->x / distance, this->y / distance);
	}
};
