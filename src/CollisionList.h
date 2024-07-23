// CollisionList.h: interface for the CollisionList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISIONLIST_H__1E206CCD_874B_4C12_8FB1_FA4B66D92480__INCLUDED_)
#define AFX_COLLISIONLIST_H__1E206CCD_874B_4C12_8FB1_FA4B66D92480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CollisionPair.h"

class CollisionList  
{
public:
	void CleanList();
	bool Add(CollisionPair *cp);
	CollisionList();
	virtual ~CollisionList();
	CollisionPair *head;
};

#endif // !defined(AFX_COLLISIONLIST_H__1E206CCD_874B_4C12_8FB1_FA4B66D92480__INCLUDED_)
