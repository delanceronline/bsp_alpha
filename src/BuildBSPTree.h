// BuildBSPTree.h: interface for the BuildBSPTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUILDBSPTREE_H__8809F723_0150_4E0C_A07A_5BA365DD4331__INCLUDED_)
#define AFX_BUILDBSPTREE_H__8809F723_0150_4E0C_A07A_5BA365DD4331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <D3DX8.h>
#include "BSP_Node.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "SkyModel.h"

class BuildBSPTree  
{
public:
	BSP_Node * FindMinSplits(bool side, BSP_Node *node);
	BSP_Node * FindBalanceNode(bool side, BSP_Node *node);
	void NumTrianglesOnSides(BSP_Node *node, int *pos, int *neg);
	int NumSplits(BSP_Node *node);
	Vector2D SplitUV(Vector2D *p_end, Vector2D *p_start, float t);
	int num_nodes(void);
	void BuildWholeBSPTree(void);
	void BuildBranches(BSP_Node *node);
	void UpdateLink(BSP_Node *node, BSP_Node *temp);
	BSP_Node *headnode, *treehead;
	void SetHeadNode(BSP_Node *hn);
	bool BuildRoot();
	BuildBSPTree();
	virtual ~BuildBSPTree();

};

#endif // !defined(AFX_BUILDBSPTREE_H__8809F723_0150_4E0C_A07A_5BA365DD4331__INCLUDED_)
