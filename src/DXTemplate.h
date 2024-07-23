//-----------------------------------------------------------------------------
// File: DXTemplate.h
//
// Desc: Header file DXTemplate sample app
//-----------------------------------------------------------------------------
#ifndef AFX_DXTEMPLATE_H__70724BF9_180D_45E0_9D0E_A6D80FB81FC4__INCLUDED_
#define AFX_DXTEMPLATE_H__70724BF9_180D_45E0_9D0E_A6D80FB81FC4__INCLUDED_

#include "SkyModel.h"
#include "SceneModelsInfo.h"
#include "BuildBSPTree.h"
#include "BSP_Node.h"
#include "SceneObject.h"
#include "Vector3D.h"
#include "ViewFrustum.h"
#include "BSPNodeSet.h"

//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// TODO: change "DirectX AppWizard Apps" to your name or the company name
#define DXAPP_KEY        TEXT("Software\\DirectX AppWizard Apps\\DXTemplate")

// Custom D3D vertex format used by the vertex buffer
struct CUSTOMVERTEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	FLOAT Nx, Ny, Nz;
	FLOAT u, v;

};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


// DirectInput action mapper reports events only when buttons/axis change
// so we need to remember the present state of relevant axis/buttons for 
// each DirectInput device.  The CInputDeviceManager will store a 
// pointer for each device that points to this struct
struct InputDeviceState
{
    // TODO: change as needed
    FLOAT fAxisRotateLR;
    BOOL  bButtonRotateLeft;
    BOOL  bButtonRotateRight;

    FLOAT fAxisRotateUD;
    BOOL  bButtonRotateUp;
    BOOL  bButtonRotateDown;
};


// Struct to store the current input state
struct UserInput
{
    // TODO: change as needed
    FLOAT fAxisRotateUD;
    FLOAT fAxisRotateLR;
    BOOL bDoConfigureInput;
    BOOL bDoConfigureDisplay;
};

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
    BOOL                    m_bLoadingApp;          // TRUE, if the app is loading
    LPDIRECT3DVERTEXBUFFER8 m_pVB;                  // Vextex buffer 
    CD3DFont*               m_pFont;                // Font for drawing text

	SkyModel model;
	SceneModelsInfo SMI;
	CUSTOMVERTEX *g_Vertices;

	BSPNodeSet m_BSPNodeSet;
	BuildBSPTree BSP_Tree;
	BSP_Node *BSP_Nodehead, *prev_BSP_Node, *temp_BSP_Node, *prev_start_node;
	int num_nodes, node_count;
	bool m_IsStartNodeSet, m_side;

	SceneObject Walker;
	ViewFrustum vf;

    CInputDeviceManager*    m_pInputDeviceManager;  // DirectInput device manager
    DIACTIONFORMAT          m_diafGame;             // Action format for game play
    LPDIRECT3DSURFACE8      m_pDIConfigSurface;     // Surface for config'ing DInput devices
    UserInput               m_UserInput;            // Struct for storing user input 

    FLOAT                   m_fWorldRotX;           // World rotation state X-axis
    FLOAT                   m_fWorldRotY;           // World rotation state Y-axis
	
	float m_til;

protected:
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
    HRESULT FrameMove();
    HRESULT FinalCleanup();
    HRESULT ConfirmDevice( D3DCAPS8*, DWORD, D3DFORMAT );
    VOID    Pause( BOOL bPause );

    HRESULT RenderText();

    HRESULT InitInput( HWND hWnd );
    void    UpdateInput( UserInput* pUserInput );
    void    CleanupDirectInput();

    VOID    ReadSettings();
    VOID    WriteSettings();

public:
	void DrawCoplanars(BSP_Node *pNode);
	void SetMaterialByIndex(int index);
	void DrawLeaf(BSP_Node *pLeaf);
	bool IsLineCross(Vector3D r1, Vector3D r2, float d1, float d2, BSP_Node *node);
	bool IsFrustumCross(float *w, BSP_Node *node);
	bool IsInFrustum(BSP_Node *node);
	void SetMaterial(BSP_Node *node);
	void DrawNode(BSP_Node *node);
	HRESULT LoadModel(void);
    LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    CMyD3DApplication();

    HRESULT InputAddDeviceCB( CInputDeviceManager::DeviceInfo* pDeviceInfo, const DIDEVICEINSTANCE* pdidi );
    static HRESULT CALLBACK StaticInputAddDeviceCB( CInputDeviceManager::DeviceInfo* pDeviceInfo, const DIDEVICEINSTANCE* pdidi, LPVOID pParam );   
    BOOL    ConfigureInputDevicesCB( IUnknown* pUnknown );
    static BOOL CALLBACK StaticConfigureInputDevicesCB( IUnknown* pUnknown, VOID* pUserData );
};


#endif // !defined(AFX_DXTEMPLATE_H__70724BF9_180D_45E0_9D0E_A6D80FB81FC4__INCLUDED_)
