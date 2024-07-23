// Vertice.cpp: implementation of the Vertice class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "DXTemplate.h"
#include "Vertice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vertice::Vertice()
{

}

Vertice::~Vertice()
{

}

Vertice& Vertice::operator =(const Vertice& vert_in)
{
	if(&vert_in == this)
		return *this;

	this->x = vert_in.x;
	this->y = vert_in.y;
	this->z = vert_in.z;
	this->u = vert_in.u;
	this->v = vert_in.v;
	this->joint_index = vert_in.joint_index;
	
	return *this;
}