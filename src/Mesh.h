// Mesh.h: interface for the Mesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH_H__A0BB53ED_EE42_420A_8C37_C23347E6C265__INCLUDED_)
#define AFX_MESH_H__A0BB53ED_EE42_420A_8C37_C23347E6C265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Normal.h"
#include "Vertice.h"
#include "Index.h"

class Mesh  
{
public:
	Mesh();
	virtual ~Mesh();

	int material_index, num_vertices, num_normals, num_triangles;
	char Name[100];
	Vertice *vertice;
	Normal *normal;
	Index *index;

};

#endif // !defined(AFX_MESH_H__A0BB53ED_EE42_420A_8C37_C23347E6C265__INCLUDED_)
