// BSP_Node.cpp: implementation of the BSP_Node class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BSP_Node.h"
#include <windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BSP_Node::BSP_Node()
{
	Parent = NULL;
	Pos_Child = NULL;
	Neg_Child = NULL;
	LinkTo = NULL;
	LinkFrom = NULL;

	tri = NULL;
	nMaterial_Leaf = NULL;
	IsTransparent_Leaf = NULL;
	Normal_Leaf = NULL;

	pCoplanarTriangle = NULL;
	p_nMaterial_CPTri = NULL;
	p_IsTransparent_CPTri = NULL;
	pNormal_CPTri = NULL;

	nVB = NULL;
	nVB_Coplanar = NULL;
	nVB_Leaf = NULL;

	IsPosChecked = false;
	IsNegChecked = false;
	splitted = false;
	IsTransparent = false;
	IsLeaf = false;
	IsBelongToLeaf = false;
	IsCoplanar = false;
	side = 0;
	numTriangles = 0;
	numConTri = 0;

	Parent_nNode = 0;
	nPosChild = 0;
	nNegChild = 0;
}

BSP_Node::~BSP_Node()
{
	if(tri != NULL)
	{
		delete[] tri;
		tri = NULL;
	}

	if(nMaterial_Leaf != NULL)
	{
		delete[] nMaterial_Leaf;
		nMaterial_Leaf = NULL;
	}
	
	if(IsTransparent_Leaf != NULL)
	{
		delete[] IsTransparent_Leaf;
		IsTransparent_Leaf = NULL;
	}

	if(Normal_Leaf != NULL)
	{
		delete[] Normal_Leaf;
		Normal_Leaf = NULL;
	}

	if(pCoplanarTriangle != NULL)
	{
		delete[] pCoplanarTriangle;
		pCoplanarTriangle = NULL;
	}

	if(p_nMaterial_CPTri != NULL)
	{
		delete[] p_nMaterial_CPTri;
		p_nMaterial_CPTri = NULL;
	}

	if(p_IsTransparent_CPTri != NULL)
	{
		delete[] p_IsTransparent_CPTri;
		p_IsTransparent_CPTri = NULL;
	}

	if(pNormal_CPTri != NULL)
	{
		delete[] pNormal_CPTri;
		pNormal_CPTri = NULL;
	}
	
	if(nVB_Leaf != NULL)
		delete[] nVB_Leaf;

	if(nVB_Coplanar != NULL)
		delete[] nVB_Coplanar;
}
