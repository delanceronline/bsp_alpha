// Joint.h: interface for the Joint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINT_H__FCEA62BB_22BD_42E4_8671_D2926C2B14F6__INCLUDED_)
#define AFX_JOINT_H__FCEA62BB_22BD_42E4_8671_D2926C2B14F6__INCLUDED_

#include "KeyFrame.h"
#include "Matrix.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Joint  
{
public:
	void AdvanceJoint(float time);
	void InitializeMatrix();
	float DefaultTranslation[3], DefaultRotation[3];
	KeyFrame *TranslationKey, *RotationKey;
	Joint *ptrParent;
	Matrix FinalMatrix;
	char Name[128], ParentName[128];
	bool isParent;
	int num_TKeys, num_RKeys;
	Joint();
	virtual ~Joint();

};

#endif // !defined(AFX_JOINT_H__FCEA62BB_22BD_42E4_8671_D2926C2B14F6__INCLUDED_)
