// Joint.cpp: implementation of the Joint class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "DXTemplate.h"
#include "Joint.h"
#include <memory.h>
#include <windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Joint::Joint()
{
	ptrParent = NULL;
}

Joint::~Joint()
{

}


void Joint::InitializeMatrix()
{
	Matrix m_rel;

	// Create a transformation matrix from the position and rotation 
	m_rel.setRotationRadian(DefaultRotation);
	m_rel.setTranslation(DefaultTranslation);

	// Each bone's final matrix is its relative matrix concatenated onto its 
	// parent's final matrix (which in turn is ....)
	//
	if (ptrParent == NULL)
		FinalMatrix.Set(m_rel.value);
	else
	{
		FinalMatrix.Set(ptrParent->FinalMatrix.value);
		FinalMatrix.MultiplyMatrix(m_rel.value);
	}
}

void Joint::AdvanceJoint(float time)
{
	int		i;
	float	deltaTime;
	float	fraction;
	float	Position[3];
	float	Rotation[3];


	/* 
	 * Position
	 */

	while(time > 24.0f)
		time -= 24.0f;

	if(time < 1.0f)
		time = 1.0f;

	//Find appropriate position key frame.
	i = 0;
	while( i < num_TKeys - 1 && TranslationKey[i].FrameTime < time )
		i++;

	//assert(i < num_TKeys);

	if( i > 0 )
	{
		//Interpolate between 2 key frames.

		//Time between the 2 key frames.
		deltaTime = TranslationKey[i].FrameTime - TranslationKey[i-1].FrameTime;

		//assert( deltaTime > 0 );

		//Relative position of interpolation point to the keyframes [0..1].
		fraction = (time - TranslationKey[i-1].FrameTime) / deltaTime;

		//assert( fraction > 0 );
		//assert( fraction < 1.0 );

		Position[0] = TranslationKey[i-1].KeyValue[0] + fraction * (TranslationKey[i].KeyValue[0] - TranslationKey[i-1].KeyValue[0]);
		Position[1] = TranslationKey[i-1].KeyValue[1] + fraction * (TranslationKey[i].KeyValue[1] - TranslationKey[i-1].KeyValue[1]);
		Position[2] = TranslationKey[i-1].KeyValue[2] + fraction * (TranslationKey[i].KeyValue[2] - TranslationKey[i-1].KeyValue[2]);
	}
	else
	{
		memcpy(Position, TranslationKey[i].KeyValue, sizeof(float) * 3);
	}

	/* 
	 * Rotation
	 */

	//Find appropriate rotation key frame.
	i = 0;
	while( i < num_RKeys - 1 && RotationKey[i].FrameTime < time)
		i++;

	//assert(i < num_RKeys);

	if( i > 0 )
	{
		//Interpolate between 2 key frames.

		//Time between the 2 key frames.
		deltaTime = RotationKey[i].FrameTime - RotationKey[i-1].FrameTime;
		//assert( deltaTime > 0 );

		//Relative position of interpolation point to the keyframes [0..1].
		fraction = (time - RotationKey[i-1].FrameTime) / deltaTime;
		//assert( fraction > 0 );
		//assert( fraction < 1.0 );

		Rotation[0] = RotationKey[i-1].KeyValue[0] + fraction * (RotationKey[i].KeyValue[0] - RotationKey[i-1].KeyValue[0]);
		Rotation[1] = RotationKey[i-1].KeyValue[1] + fraction * (RotationKey[i].KeyValue[1] - RotationKey[i-1].KeyValue[1]);
		Rotation[2] = RotationKey[i-1].KeyValue[2] + fraction * (RotationKey[i].KeyValue[2] - RotationKey[i-1].KeyValue[2]);
	}
	else
	{
		memcpy(Rotation, RotationKey[i].KeyValue, sizeof(float) * 3);
	}

	//Now we know the position and rotation for this animation frame.
	//Let's calculate the transformation matrix (m_final) for this bone.

	Matrix m_rel;
	Matrix m_frame;

	//Create a transformation matrix from the position and rotation of this.
	//Joint in the rest position.
	m_rel.setRotationRadian(DefaultRotation);
	m_rel.setTranslation(DefaultTranslation);

	//Create a transformation matrix from the position and rotation.
	//m_frame: additional transformation for this frame of the animation.
	m_frame.setRotationRadian(Rotation);
	m_frame.setTranslation(Position);

	//Add the animation state to the rest position.
	m_rel.MultiplyMatrix(m_frame.value);

	// 
	if (ptrParent == NULL)					// this is the root node
		FinalMatrix.Set(m_rel.value);	// m_final := m_rel
	else									// not the root node
	{
		//m_final := parent's m_final * m_rel (matrix concatenation).

		FinalMatrix.Set(ptrParent->FinalMatrix.value);
		FinalMatrix.MultiplyMatrix(m_rel.value);
	}
}
