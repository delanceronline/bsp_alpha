// SceneObject.h: interface for the SceneObject class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SCENEOBJECT_H__47660504_B627_4E9C_A887_5D59999A1CF9__INCLUDED_)
#define AFX_SCENEOBJECT_H__47660504_B627_4E9C_A887_5D59999A1CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "Vector3D.h"
#include "BoundBox.h"

class SceneObject  
{
public:
	void SimpleInitialize(float mass);
	void UpdateBoundBox(void);
	void InitializeKinematics(Vector3D Extp_in, Vector3D Extn_in);
	void AdvanceKinematics(float delta_t);
	SceneObject();
	virtual ~SceneObject();
	Vector3D lin_r, ang_r, lin_v, ang_v, lin_a, ang_a;
	Vector3D cm_pos;
	float I_inv_mat[9];
	Vector3D lin_mom, ang_mom;
	float I_body_inv_mat[9];
	float Rot_mat[9];
	Vector3D lin_imp, ang_imp;
	float one_over_box_mass;
	float BoundingRadius;
	SceneObject *m_previous, *m_next;
	BoundBox *bb;
	Vector3D lin_mom_local;
	Vector3D Extp, Extn;
	float attacking_power, life_value;
	bool IsAttacked;
};

#endif // !defined(AFX_SCENEOBJECT_H__47660504_B627_4E9C_A887_5D59999A1CF9__INCLUDED_)
