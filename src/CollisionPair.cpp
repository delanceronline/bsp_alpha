// CollisionPair.cpp: implementation of the CollisionPair class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "DXTemplate.h"
#include "CollisionPair.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CollisionPair::CollisionPair()
{
	target_so = NULL;
	m_previous = NULL;
	m_next = NULL;
}

CollisionPair::~CollisionPair()
{

}
