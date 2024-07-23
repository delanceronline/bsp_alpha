#if !defined(AFX_MYMATHS_H__BFFF1FA8_D200_4C11_9931_18A8EF6A13B1__INCLUDED_)
#define AFX_MYMATHS_H__BFFF1FA8_D200_4C11_9931_18A8EF6A13B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector3D.h"

#define DEGTORAD(x) ((x) * 0.017453292519943296)
#define RADTODEG(x) ((x) * 57.29577951308232088)

Vector3D ToUnit(Vector3D v);
void MultiMatrix_f(float *m, float factor, float *m_out);
void MultiMatrix1x3(float *m, Vector3D v, Vector3D *v_out);
void AdvanceRotationMatrix(float *Ri, Vector3D omega, float delta_t, float *Ro);
void MultiVector3D_f(Vector3D v, float factor, Vector3D *v_out);
void MultiMatrix3x1(float *m, Vector3D v, Vector3D *v_out);
void EqualMatrix(float *m1, float *m2);
void MatrixTranspose(float *m, float *m_out);
void MatrixStar(Vector3D v, float *m_out);
void MultiMatrix_f(float *m, float factor, float *m_out);
void MultiMatrix3x3(float *m1, float *m2, float *m3);
void AddMatrix3x3(float *m1, float *m2, float *m3);
Vector3D CrossProduct(Vector3D v1, Vector3D v2);
bool Greater(float var1, float var2);
bool Less(float var1, float var2);
bool GreaterOrEqual(float var1, float var2);
bool LessOrEqual(float var1, float var2);
bool IsEqual(float var1, float var2);

#endif // !defined(AFX_MYMATHS_H__BFFF1FA8_D200_4C11_9931_18A8EF6A13B1__INCLUDED_)
