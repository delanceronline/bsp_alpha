// BoundBox.cpp: implementation of the BoundBox class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include "DXTemplate.h"
#include "BoundBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BoundBox::BoundBox()
{
	Ax = Vector3D(1.0f, 0.0f, 0.0f);
	Ay = Vector3D(0.0f, 1.0f, 0.0f);
	Az = Vector3D(0.0f, 0.0f, 1.0f);
}

BoundBox::~BoundBox()
{

}

void BoundBox::Initialize()
{
	r = (Extp + Extn) * 0.5f;
}
