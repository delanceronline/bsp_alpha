// BoundBox.h: interface for the BoundBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDBOX_H__32DF943A_0678_4AB0_A663_0FA18E72FDA2__INCLUDED_)
#define AFX_BOUNDBOX_H__32DF943A_0678_4AB0_A663_0FA18E72FDA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector3D.h"

class BoundBox  
{
public:
	void Initialize(void);
	Vector3D Ax, Ay, Az, v[8], Nx, Ny, Nz, Extp, Extn, r;		//Ax, Ay and Az must be unit vertors.
	BoundBox();
	virtual ~BoundBox();
};

#endif // !defined(AFX_BOUNDBOX_H__32DF943A_0678_4AB0_A663_0FA18E72FDA2__INCLUDED_)
