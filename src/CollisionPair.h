// CollisionPair.h: interface for the CollisionPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISIONPAIR_H__DB9CA5CD_66D7_4C20_976E_17D000040A30__INCLUDED_)
#define AFX_COLLISIONPAIR_H__DB9CA5CD_66D7_4C20_976E_17D000040A30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "Vector3D.h"

class CollisionPair  
{
public:
	CollisionPair();
	LPVOID target_so;
	CollisionPair *m_previous, *m_next;
	Vector3D ContactPoint;
	float depth;
	virtual ~CollisionPair();
};

#endif // !defined(AFX_COLLISIONPAIR_H__DB9CA5CD_66D7_4C20_976E_17D000040A30__INCLUDED_)
