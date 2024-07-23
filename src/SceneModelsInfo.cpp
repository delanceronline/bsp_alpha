// SceneModelsInfo.cpp: implementation of the SceneModelsInfo class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "SceneModelsInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SceneModelsInfo::SceneModelsInfo()
{
	m_num_models = 0;
	m_num_scene_vertices = 0;
	m_num_scene_materials = 0;
	model = NULL;
}

SceneModelsInfo::~SceneModelsInfo()
{
	if(model != NULL)
	{
		delete model;
	}
}
