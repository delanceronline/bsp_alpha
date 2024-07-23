// Vector2D.h: interface for the Vector2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR2D_H__CD4F280C_FE6F_45E9_A5FF_A9DA0AB0D646__INCLUDED_)
#define AFX_VECTOR2D_H__CD4F280C_FE6F_45E9_A5FF_A9DA0AB0D646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Math.h"

class Vector2D  
{
public:
	Vector2D();
	Vector2D(float in_x, float in_y);

	float x, y;

	Vector2D Normalize(void);
	float Mag(void);
	float operator*(const Vector2D& v2);
	Vector2D operator*(const float& t);
	Vector2D operator+(const Vector2D& v2);
	Vector2D operator-(const Vector2D& v2);
	
	virtual ~Vector2D();

};

#endif // !defined(AFX_VECTOR2D_H__CD4F280C_FE6F_45E9_A5FF_A9DA0AB0D646__INCLUDED_)
