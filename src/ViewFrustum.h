// ViewFrustum.h: interface for the ViewFrustum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWFRUSTUM_H__4F50DC61_662B_4664_B7A8_1C14D558AB26__INCLUDED_)
#define AFX_VIEWFRUSTUM_H__4F50DC61_662B_4664_B7A8_1C14D558AB26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector3D.h"

class ViewFrustum  
{
public:
	void Update(Vector3D vp, float *Rot_mat);
	ViewFrustum();
	virtual ~ViewFrustum();
	float fov, tan_half_fov;
	float zn, zf;
	float AspectRatio;
	float BackCullingValue;
	Vector3D UpperLeft;
	Vector3D UpperRight;
	Vector3D LowerLeft;
	Vector3D LowerRight;
	Vector3D ViewPoint;
	Vector3D UP_Normal;
	Vector3D BP_Normal;
	Vector3D LP_Normal;
	Vector3D RP_Normal;
	Vector3D FP_Normal;
	Vector3D LookDir, LookPoint;
protected:
	void UpdateNormals(void);
};

#endif // !defined(AFX_VIEWFRUSTUM_H__4F50DC61_662B_4664_B7A8_1C14D558AB26__INCLUDED_)
