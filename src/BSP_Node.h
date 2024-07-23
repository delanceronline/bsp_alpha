// BSP_Node.h: interface for the BSP_Node class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSP_NODE_H__0E21F595_97CC_4427_AB36_B57700948AFB__INCLUDED_)
#define AFX_BSP_NODE_H__0E21F595_97CC_4427_AB36_B57700948AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertice.h"
#include "Vector3D.h"
#include "BSP_Node.h"
#include <windows.h>

struct Triangle
{
	Vertice v[3];
};

class BSP_Node  
{
public:
	BSP_Node();
	Vertice v[3];
	Triangle *pCoplanarTriangle;
	Triangle *tri;
	int *nMaterial_Leaf;
	bool IsBelongToLeaf;
	bool IsCoplanar;
	bool *IsTransparent_Leaf;
	Vector3D normal, v_normal[3];
	Vector3D *Normal_Leaf;
	BSP_Node *Parent, *Pos_Child, *Neg_Child, *LinkTo, *LinkFrom;
	bool side, splitted, IsPosChecked, IsNegChecked, IsTransparent, IsLeaf;
	int MaterialIndex, numTriangles, numConTri;
	UINT nNode;

	int *p_nMaterial_CPTri;
	bool *p_IsTransparent_CPTri;
	Vector3D *pNormal_CPTri;

	int Parent_nNode, nPosChild, nNegChild;
	int nVB;
	int *nVB_Leaf, *nVB_Coplanar;

	virtual ~BSP_Node();
};

#endif // !defined(AFX_BSP_NODE_H__0E21F595_97CC_4427_AB36_B57700948AFB__INCLUDED_)
