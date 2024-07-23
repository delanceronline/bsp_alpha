// SkyModel.h: interface for the SkyModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKYMODEL_H__BD73A48A_8EA3_4FA2_A0C3_3EB2D94DD751__INCLUDED_)
#define AFX_SKYMODEL_H__BD73A48A_8EA3_4FA2_A0C3_3EB2D94DD751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh.h"
#include "Material.h"
#include "Joint.h"
#include "MeshVBInfo.h"
#include "Matrix.h"
#include <stdio.h>
#include <string.h>

class SkyModel  
{
public:
	bool LinkJoints(void);
	bool LoadASCIIFile(char *filename);
	SkyModel();
	Mesh *mesh;
	Material *material;
	Joint *joint;
	MeshVBInfo *meshvbinfo;
	int m_num_meshes, m_num_materials, m_num_mesh_vertices, m_num_mesh_triangles, m_num_joints;
	FILE *m_file;
	bool m_isAnimation;
	float m_BoundingRadius, m_MaxX, m_MinX, m_MaxY, m_MinY, m_MaxZ, m_MinZ, m_ViewOffset;
	float Cx, Cy, Cz;
	char TextureFile[256];
	float VAngle;

	virtual ~SkyModel();

};

#endif // !defined(AFX_SKYMODEL_H__BD73A48A_8EA3_4FA2_A0C3_3EB2D94DD751__INCLUDED_)
