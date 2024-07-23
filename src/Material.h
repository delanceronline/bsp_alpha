// Material.h: interface for the Material class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIAL_H__F32CD53E_B24D_497E_990B_731ED7A6BC4A__INCLUDED_)
#define AFX_MATERIAL_H__F32CD53E_B24D_497E_990B_731ED7A6BC4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <D3DX8.h>

class Material  
{
public:
	Material();
	char  Name[64];
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	float Emissive[4];
	float Shininess;
	float Transparency;
	char  DiffuseTexture[256];
	char  AlphaTexture[256];
	bool hasDiffuseTexture, hasAlphaTexture;
	LPDIRECT3DTEXTURE8 g_pTexture;
	LPDIRECT3DTEXTURE8 g_pAlphaTexture;
	virtual ~Material();
};

#endif // !defined(AFX_MATERIAL_H__F32CD53E_B24D_497E_990B_731ED7A6BC4A__INCLUDED_)
