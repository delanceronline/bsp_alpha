// Vertice.h: interface for the Vertice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTICE_H__CAAAFAFE_EF74_45C2_A06E_AE6A26B4860A__INCLUDED_)
#define AFX_VERTICE_H__CAAAFAFE_EF74_45C2_A06E_AE6A26B4860A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Vertice  
{
public:
	Vertice();
	virtual ~Vertice();

	Vertice& operator =(const Vertice& vert_in);

	float x, y, z, u, v;
	int joint_index;
};

#endif // !defined(AFX_VERTICE_H__CAAAFAFE_EF74_45C2_A06E_AE6A26B4860A__INCLUDED_)
