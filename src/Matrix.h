// Matrix.h: interface for the Matrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__27660089_10FD_4AEA_BFFA_62B62AE2B61D__INCLUDED_)
#define AFX_MATRIX_H__27660089_10FD_4AEA_BFFA_62B62AE2B61D__INCLUDED_

#include <math.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Matrix  
{
public:
	void RotateVector(float *v);
	void MultiplyMatrix(float *m);
	void Set(float *source);
	void inverseRotateVector(float *v);
	void setTranslation(float *translation);
	void setRotationRadian(float *rotation);
	void SetIdentity();
	void setInverseTranslation(float *translation);
	void setInverseRotationRadian(float *rotation);
	float value[16];
	Matrix();
	virtual ~Matrix();

};

#endif // !defined(AFX_MATRIX_H__27660089_10FD_4AEA_BFFA_62B62AE2B61D__INCLUDED_)
