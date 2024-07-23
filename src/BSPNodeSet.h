// BSPNodeSet.h: interface for the BSPNodeSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSPNODESET_H__D681BFCB_DE2F_4074_833C_EF0F568C5A71__INCLUDED_)
#define AFX_BSPNODESET_H__D681BFCB_DE2F_4074_833C_EF0F568C5A71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSP_Node.h"
#include "Material.h"

class BSPNodeSet  
{
public:
	int m_numTriangles;
	int m_numMaterials;
	int m_numNodes;
	int m_nRootNode;
	int m_nPosChild, m_nNegChild;
	BSP_Node *m_pBSPNode;
	Material *m_pMaterial;
	bool LoadBSPNodeFromFile(char *filename);
	BSPNodeSet();
	virtual ~BSPNodeSet();

};

#endif // !defined(AFX_BSPNODESET_H__D681BFCB_DE2F_4074_833C_EF0F568C5A71__INCLUDED_)
