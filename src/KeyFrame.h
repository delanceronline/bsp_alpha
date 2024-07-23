// KeyFrame.h: interface for the KeyFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYFRAME_H__46CB77DD_0834_44A6_B759_C1FE4F19EAA3__INCLUDED_)
#define AFX_KEYFRAME_H__46CB77DD_0834_44A6_B759_C1FE4F19EAA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KeyFrame  
{
public:
	float KeyValue[3], FrameTime;
	KeyFrame();
	virtual ~KeyFrame();

};

#endif // !defined(AFX_KEYFRAME_H__46CB77DD_0834_44A6_B759_C1FE4F19EAA3__INCLUDED_)
