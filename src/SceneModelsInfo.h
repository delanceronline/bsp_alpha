// SceneModelsInfo.h: interface for the SceneModelsInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEMODELSINFO_H__663FBC85_ECF7_4616_80AF_7574729582F9__INCLUDED_)
#define AFX_SCENEMODELSINFO_H__663FBC85_ECF7_4616_80AF_7574729582F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModelInfo.h"

class SceneModelsInfo  
{
public:
	SceneModelsInfo();
	int m_num_models, m_num_scene_vertices, m_num_scene_materials;
	ModelInfo *model;
	virtual ~SceneModelsInfo();

};

#endif // !defined(AFX_SCENEMODELSINFO_H__663FBC85_ECF7_4616_80AF_7574729582F9__INCLUDED_)
