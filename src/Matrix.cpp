// Matrix.cpp: implementation of the Matrix class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include "DXTemplate.h"
#include "Matrix.h"
#include <memory.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Matrix::Matrix()
{
	SetIdentity();
}

Matrix::~Matrix()
{

}

void Matrix::setInverseRotationRadian(float *rotation)
{
	double cr = cos( rotation[0] );
	double sr = sin( rotation[0] );
	double cp = cos( rotation[1] );
	double sp = sin( rotation[1] );
	double cy = cos( rotation[2] );
	double sy = sin( rotation[2] );

	value[0] = ( float )( cp*cy );
	value[1] = ( float )( cp*sy );
	value[2] = ( float )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	value[4] = ( float )( srsp*cy-cr*sy );
	value[5] = ( float )( srsp*sy+cr*cy );
	value[6] = ( float )( sr*cp );

	value[8] = ( float )( crsp*cy+sr*sy );
	value[9] = ( float )( crsp*sy-sr*cy );
	value[10] = ( float )( cr*cp );
}

void Matrix::setInverseTranslation(float *translation)
{
	value[3] = -translation[0];
	value[7] = -translation[1];
	value[11] = -translation[2];
}

void Matrix::SetIdentity()
{
	memset(value, 0, sizeof(float) * 16);
	value[0] = value[5] = value[10] = value[15] = 1;
}

void Matrix::setRotationRadian(float *rotation)
{
	double cr = cos( rotation[0] );
	double sr = sin( rotation[0] );
	double cp = cos( rotation[1] );
	double sp = sin( rotation[1] );
	double cy = cos( rotation[2] );
	double sy = sin( rotation[2] );

	value[0] = ( float )( cp*cy );
	value[4] = ( float )( cp*sy );
	value[8] = ( float )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	value[1] = ( float )( srsp*cy-cr*sy );
	value[5] = ( float )( srsp*sy+cr*cy );
	value[9] = ( float )( sr*cp );

	value[2] = ( float )( crsp*cy+sr*sy );
	value[6] = ( float )( crsp*sy-sr*cy );
	value[10] = ( float )( cr*cp );
}

void Matrix::setTranslation(float *translation)
{
	value[3] = translation[0];
	value[7] = translation[1];
	value[11] = translation[2];
}

void Matrix::inverseRotateVector(float *v)
{
	float vec[3];

	vec[0] = v[0] * value[0] + v[1] * value[4] + v[2] * value[8];
	vec[1] = v[0] * value[1] + v[1] * value[5] + v[2] * value[9];
	vec[2] = v[0] * value[2] + v[1] * value[6] + v[2] * value[10];

	memcpy(v, vec, sizeof(float) * 3);
}

void Matrix::Set(float *source)
{
	memcpy(value, source, sizeof(float) * 16);
}

void Matrix::MultiplyMatrix(float *m)
{
	float mat[16];

	mat[0] = value[0] * m[0] + value[1] * m[4] + value[2] * m[8];
	mat[1] = value[0] * m[1] + value[1] * m[5] + value[2] * m[9];
	mat[2] = value[0] * m[2] + value[1] * m[6] + value[2] * m[10];

	mat[4] = value[4] * m[0] + value[5] * m[4] + value[6] * m[8];
	mat[5] = value[4] * m[1] + value[5] * m[5] + value[6] * m[9];
	mat[6] = value[4] * m[2] + value[5] * m[6] + value[6] * m[10];

	mat[8] = value[8] * m[0] + value[9] * m[4] + value[10] * m[8];
	mat[9] = value[8] * m[1] + value[9] * m[5] + value[10] * m[9];
	mat[10] = value[8] * m[2] + value[9] * m[6] + value[10] * m[10];

	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;

	mat[3] = value[0] * m[3] + value[1] * m[7] + value[2] * m[11] + value[3];
	mat[7] = value[4] * m[3] + value[5] * m[7] + value[6] * m[11] + value[7];
	mat[11] = value[8] * m[3] + value[9] * m[7] + value[10] * m[11] + value[11];

	Set(mat);
}

void Matrix::RotateVector(float *v)
{
	float vec[3];

	vec[0] = v[0] * value[0] + v[1] * value[1] + v[2] * value[2];
	vec[1] = v[0] * value[4] + v[1] * value[5] + v[2] * value[6];
	vec[2] = v[0] * value[8] + v[1] * value[9] + v[2] * value[10];

	memcpy(v, vec, sizeof(float) * 3);
}

