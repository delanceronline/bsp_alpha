// MeshVBInfo.h: interface for the MeshVBInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHVBINFO_H__A780CEC6_81AB_42C5_AC75_B1AE8B112A82__INCLUDED_)
#define AFX_MESHVBINFO_H__A780CEC6_81AB_42C5_AC75_B1AE8B112A82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MeshVBInfo  
{
public:
	int num_Triangles;
	int StartIndex;
	MeshVBInfo();
	virtual ~MeshVBInfo();

};

#endif // !defined(AFX_MESHVBINFO_H__A780CEC6_81AB_42C5_AC75_B1AE8B112A82__INCLUDED_)
