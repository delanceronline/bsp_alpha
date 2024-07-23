// SceneObject.cpp: implementation of the SceneObject class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "DXTemplate.h"
#include "SceneObject.h"
#include "MyMaths.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SceneObject::SceneObject()
{
	lin_r = Vector3D(0.0f, 0.0f, 0.0f);
	ang_r = Vector3D(0.0f, 0.0f, 0.0f);
	lin_v = Vector3D(0.0f, 0.0f, 0.0f);
	ang_v = Vector3D(0.0f, 0.0f, 0.0f);
	lin_a = Vector3D(0.0f, 0.0f, 0.0f);
	ang_a = Vector3D(0.0f, 0.0f, 0.0f);
	lin_imp = Vector3D(0.0f, 0.0f, 0.0f);
	ang_imp = Vector3D(0.0f, 0.0f, 0.0f);
	BoundingRadius = 0.0f;
	attacking_power = 0.0f;
	life_value = 100.0f;
	m_previous = NULL;
	m_next = NULL;
	bb = NULL;
	IsAttacked = false;
}

SceneObject::~SceneObject()
{
	if(bb != NULL)
	{
		delete bb;
		bb = NULL;
	}
}

void SceneObject::AdvanceKinematics(float delta_t)
{
	Vector3D temp, var;
	float m1[9], m2[9];

	//Transform tank's local coordinate to world coordinate.
	if(lin_mom_local.Mag() != 0.0f)
	{
		MultiMatrix3x1(Rot_mat, lin_mom_local, &temp);
		lin_mom_local = Vector3D(0.0f, 0.0f, 0.0f);
		lin_mom = lin_mom + temp;
	}

	//Advance linear and angular momentums.
	lin_mom = lin_mom + lin_imp * delta_t;
	ang_mom = ang_mom + ang_imp * delta_t;

	//Advance inverse of I.
	MultiMatrix3x3(Rot_mat, I_body_inv_mat, m1);
	MatrixTranspose(Rot_mat, m2);
	MultiMatrix3x3(m1, m2, I_inv_mat);

	//Advance angular velocity.
	MultiMatrix3x1(I_inv_mat, ang_mom, &ang_v);

	//Advance linear velocity.
	lin_v = lin_mom * one_over_box_mass;

	//Advance linear and angular displacements.
	lin_r = lin_r + lin_v * delta_t;
	ang_r = ang_r + ang_v * delta_t;

	//Advance rotational matrix.
	AdvanceRotationMatrix(Rot_mat, ang_v, delta_t, m1);
	EqualMatrix(m1, Rot_mat);

	if(bb != NULL)
	{
		//Update bounding box.

		//Rotate orth vectors of bounding box.
		MultiMatrix3x1(Rot_mat, bb->Ax, &bb->Nx);
		MultiMatrix3x1(Rot_mat, bb->Ay, &bb->Ny);
		MultiMatrix3x1(Rot_mat, bb->Az, &bb->Nz);

		//Update bounding box's vertices.
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extp.y, Extp.z), &bb->v[0]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extp.y, Extn.z), &bb->v[1]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extp.y, Extn.z), &bb->v[2]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extp.y, Extp.z), &bb->v[3]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extn.y, Extp.z), &bb->v[4]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extn.y, Extn.z), &bb->v[5]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extn.y, Extn.z), &bb->v[6]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extn.y, Extp.z), &bb->v[7]);

		bb->v[0] = bb->v[0] + lin_r;
		bb->v[1] = bb->v[1] + lin_r;
		bb->v[2] = bb->v[2] + lin_r;
		bb->v[3] = bb->v[3] + lin_r;
		bb->v[4] = bb->v[4] + lin_r;
		bb->v[5] = bb->v[5] + lin_r;
		bb->v[6] = bb->v[6] + lin_r;
		bb->v[7] = bb->v[7] + lin_r;
	}
}

void SceneObject::InitializeKinematics(Vector3D Extp_in, Vector3D Extn_in)
{
	float box_mass, var, w, l, h;

	Extp = Extp_in;
	Extn = Extn_in;

	box_mass = 3.0f;
	one_over_box_mass = 1.0f / box_mass;

	//Linear and angular velocities.
	lin_v = Vector3D(0.0f, 0.0f, 0.0f);
	ang_v = Vector3D(0.0f, 0.0f, 0.0f);

	//Linear and angular momentums.
	lin_mom = Vector3D(0.0f, 0.0f, 0.0f);
	ang_mom = Vector3D(0.0f, 0.0f, 0.0f);

	//Linear and angular impluses.
	lin_imp = Vector3D(0.0f, 0.0f, 0.0f);
	ang_imp = Vector3D(0.0f, 0.0f, 0.0f);

	//Rotational Matrix.
	Rot_mat[0] = 1.0f; Rot_mat[1] = 0.0f; Rot_mat[2] = 0.0f;
	Rot_mat[3] = 0.0f; Rot_mat[4] = 1.0f; Rot_mat[5] = 0.0f;
	Rot_mat[6] = 0.0f; Rot_mat[7] = 0.0f; Rot_mat[8] = 1.0f;

	if(bb != NULL)
	{
		bb->Initialize();

		//Rotate orth vectors of bounding box.
		MultiMatrix3x1(Rot_mat, bb->Ax, &bb->Nx);
		MultiMatrix3x1(Rot_mat, bb->Ay, &bb->Ny);
		MultiMatrix3x1(Rot_mat, bb->Az, &bb->Nz);

		//Update bounding box's vertices.
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extp.y, Extp.z), &bb->v[0]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extp.y, Extn.z), &bb->v[1]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extp.y, Extn.z), &bb->v[2]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extp.y, Extp.z), &bb->v[3]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extn.y, Extp.z), &bb->v[4]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extn.y, Extn.z), &bb->v[5]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extn.y, Extn.z), &bb->v[6]);
		MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extn.y, Extp.z), &bb->v[7]);

		bb->v[0] = bb->v[0] + lin_r;
		bb->v[1] = bb->v[1] + lin_r;
		bb->v[2] = bb->v[2] + lin_r;
		bb->v[3] = bb->v[3] + lin_r;
		bb->v[4] = bb->v[4] + lin_r;
		bb->v[5] = bb->v[5] + lin_r;
		bb->v[6] = bb->v[6] + lin_r;
		bb->v[7] = bb->v[7] + lin_r;

		//Postion of centre of mass.
		cm_pos = bb->r;
	}
	else
		cm_pos = Vector3D(0.0f, 0.0f, 0.0f);

	//Width, height and length of bounding box.
	w = Extp.x - Extn.x;
	h = Extp.y - Extn.y;
	l = Extp.z - Extn.z;

	//Inverse of Ibody.
	var = box_mass / 12.0f;
	I_body_inv_mat[0] = var * (h * h + l * l); I_body_inv_mat[1] = 0.0f; I_body_inv_mat[2] = 0.0f;
	I_body_inv_mat[3] = 0.0f; I_body_inv_mat[4] = var * (w * w + l * l); I_body_inv_mat[5] = 0.0f;
	I_body_inv_mat[6] = 0.0f; I_body_inv_mat[7] = 0.0f; I_body_inv_mat[8] = var * (w * w + h * h);

	lin_mom_local = Vector3D(0.0f, 0.0f, 0.0f);
}

//This function is intended to be used in MultiPlay mode
//because it only uses rotational matrix sent by another side 
//to update its bounding box's vertices.
void SceneObject::UpdateBoundBox()
{
	//Rotate orth vectors of bounding box.
	MultiMatrix3x1(Rot_mat, bb->Ax, &bb->Nx);
	MultiMatrix3x1(Rot_mat, bb->Ay, &bb->Ny);
	MultiMatrix3x1(Rot_mat, bb->Az, &bb->Nz);

	//Update bounding box's vertices.
	MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extp.y, Extp.z), &bb->v[0]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extp.y, Extn.z), &bb->v[1]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extp.y, Extn.z), &bb->v[2]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extp.y, Extp.z), &bb->v[3]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extn.y, Extp.z), &bb->v[4]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extp.x, Extn.y, Extn.z), &bb->v[5]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extn.y, Extn.z), &bb->v[6]);
	MultiMatrix3x1(Rot_mat, Vector3D(Extn.x, Extn.y, Extp.z), &bb->v[7]);

	bb->v[0] = bb->v[0] + lin_r;
	bb->v[1] = bb->v[1] + lin_r;
	bb->v[2] = bb->v[2] + lin_r;
	bb->v[3] = bb->v[3] + lin_r;
	bb->v[4] = bb->v[4] + lin_r;
	bb->v[5] = bb->v[5] + lin_r;
	bb->v[6] = bb->v[6] + lin_r;
	bb->v[7] = bb->v[7] + lin_r;
}

void SceneObject::SimpleInitialize(float mass)
{
	float box_mass, var, w, l, h;

	box_mass = mass;
	one_over_box_mass = 1.0f / box_mass;

	//Linear and angular displacements.
	lin_r = Vector3D(0.0f, 0.0f, 0.0f);
	ang_r = Vector3D(0.0f, 0.0f, 0.0f);

	//Linear and angular velocities.
	lin_v = Vector3D(0.0f, 0.0f, 0.0f);
	ang_v = Vector3D(0.0f, 0.0f, 0.0f);

	//Linear and angular momentums.
	lin_mom = Vector3D(0.0f, 0.0f, 0.0f);
	ang_mom = Vector3D(0.0f, 0.0f, 0.0f);

	//Linear and angular impluses.
	lin_imp = Vector3D(0.0f, 0.0f, 0.0f);
	ang_imp = Vector3D(0.0f, 0.0f, 0.0f);

	//Rotational Matrix.
	Rot_mat[0] = 1.0f; Rot_mat[1] = 0.0f; Rot_mat[2] = 0.0f;
	Rot_mat[3] = 0.0f; Rot_mat[4] = 1.0f; Rot_mat[5] = 0.0f;
	Rot_mat[6] = 0.0f; Rot_mat[7] = 0.0f; Rot_mat[8] = 1.0f;

	cm_pos = Vector3D(0.0f, 0.0f, 0.0f);

	w = 1.0f;
	h = 1.0f;
	l = 1.0f;

	//Inverse of Ibody.
	var = box_mass / 12.0f;
	I_body_inv_mat[0] = var * (h * h + l * l); I_body_inv_mat[1] = 0.0f; I_body_inv_mat[2] = 0.0f;
	I_body_inv_mat[3] = 0.0f; I_body_inv_mat[4] = var * (w * w + l * l); I_body_inv_mat[5] = 0.0f;
	I_body_inv_mat[6] = 0.0f; I_body_inv_mat[7] = 0.0f; I_body_inv_mat[8] = var * (w * w + h * h);

	lin_mom_local = Vector3D(0.0f, 0.0f, 0.0f);
}
