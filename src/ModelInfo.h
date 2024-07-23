// ModelInfo.h: interface for the ModelInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELINFO_H__DA9F4B2F_2F89_47D0_8788_D036AD970620__INCLUDED_)
#define AFX_MODELINFO_H__DA9F4B2F_2F89_47D0_8788_D036AD970620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ModelInfo  
{
public:
	ModelInfo();
	int m_start_index_VB, m_start_index_material;
	int m_num_model_vertices;
	int m_num_model_materials;
	virtual ~ModelInfo();

};

#endif // !defined(AFX_MODELINFO_H__DA9F4B2F_2F89_47D0_8788_D036AD970620__INCLUDED_)
