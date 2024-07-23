// Vector2D.cpp: implementation of the Vector2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector2D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float in_x, float in_y)
{
	x = in_x;
	y = in_y;
}

Vector2D::~Vector2D()
{

}

Vector2D Vector2D::operator+(const Vector2D& v2)
{
  return Vector2D(x + v2.x, y + v2.y);
}

Vector2D Vector2D::operator-(const Vector2D& v2)
{
  return Vector2D(x - v2.x, y - v2.y);
}

Vector2D Vector2D::Normalize()
{
	return Vector2D(x / Mag(), y / Mag());
}

float Vector2D::Mag()
{
	return powf(x * x + y * y, 0.5f);
}

float Vector2D::operator*(const Vector2D& v2)
{
  return x * v2.x + y * v2.y;
}

Vector2D Vector2D::operator*(const float& t)
{
  return Vector2D(x * t, y * t);
}