#include "Vector2.h"

void Vector2::SetX(int x)
{
	this->x = x;
}

void Vector2::SetY(int y)
{
	this->y = y;
}

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(const Vector2& v)
{
	x = v.x;
	y = v.y;
}

Vector2::Vector2(int i)
{
	x = y = i;
}

Vector2::Vector2(int x, int y)
{
	this->x = x;
	this->y = y;
}
