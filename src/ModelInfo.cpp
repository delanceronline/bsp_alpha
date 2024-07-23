// ModelInfo.cpp: implementation of the ModelInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "ModelInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModelInfo::ModelInfo()
{
	m_num_model_vertices = 0;
	m_num_model_materials = 0;
	m_start_index_VB = 0;
	m_start_index_material = 0;
}

ModelInfo::~ModelInfo()
{

}
