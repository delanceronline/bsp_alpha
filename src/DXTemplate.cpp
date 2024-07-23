//-----------------------------------------------------------------------------
// File: DXTemplate.cpp
//
// Desc: DirectX window application created by the DirectX AppWizard
//-----------------------------------------------------------------------------
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <D3DX8.h>
#include <DXErr8.h>
#include <tchar.h>
#include <dinput.h>
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DIUtil.h"
#include "DXUtil.h"
#include "resource.h"
#include "DXTemplate.h"
#include "MyMaths.h"

//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// This GUID must be unique for every game, and the same for 
// every instance of this app.  // {A1C7A3FB-3DBE-4392-94E4-4BEF1262F658}
// The GUID allows DirectInput to remember input settings
GUID g_guidApp = { 0xa1c7a3fb, 0x3dbe, 0x4392, { 0x94, 0xe4, 0x4b, 0xef, 0x12, 0x62, 0xf6, 0x58 } };


// Input semantics used by this app
enum INPUT_SEMANTICS
{
    // Gameplay semantics
    // TODO: change as needed
    INPUT_ROTATE_AXIS_LR=1, INPUT_ROTATE_AXIS_UD,       
    INPUT_ROTATE_LEFT,      INPUT_ROTATE_RIGHT,    
    INPUT_ROTATE_UP,        INPUT_ROTATE_DOWN,
    INPUT_CONFIG_INPUT,     INPUT_CONFIG_DISPLAY,
};

// Actions used by this app
DIACTION g_rgGameAction[] =
{
    // TODO: change as needed.  Be sure to delete user map files 
    // (C:\Program Files\DirectX\DirectInput\User Maps\*.ini)
    // after changing this, otherwise settings won't reset and will be read 
    // from the out of date ini files 

    // Device input (joystick, etc.) that is pre-defined by DInput, according
    // to genre type. The genre for this app is space simulators.
    { INPUT_ROTATE_AXIS_LR,  DIAXIS_3DCONTROL_LATERAL,      0, TEXT("Rotate left/right"), },
    { INPUT_ROTATE_AXIS_UD,  DIAXIS_3DCONTROL_MOVE,         0, TEXT("Rotate up/down"), },

    // Keyboard input mappings
    { INPUT_ROTATE_LEFT,     DIKEYBOARD_LEFT,               0, TEXT("Rotate left"), },
    { INPUT_ROTATE_RIGHT,    DIKEYBOARD_RIGHT,              0, TEXT("Rotate right"), },
    { INPUT_ROTATE_UP,       DIKEYBOARD_UP,                 0, TEXT("Rotate up"), },
    { INPUT_ROTATE_DOWN,     DIKEYBOARD_DOWN,               0, TEXT("Rotate down"), },
    { INPUT_CONFIG_DISPLAY,  DIKEYBOARD_F2,                 DIA_APPFIXED, TEXT("Configure Display"), },    
    { INPUT_CONFIG_INPUT,    DIKEYBOARD_F3,                 DIA_APPFIXED, TEXT("Configure Input"), },    
};

#define NUMBER_OF_GAMEACTIONS    (sizeof(g_rgGameAction)/sizeof(DIACTION))




//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
CMyD3DApplication* g_pApp  = NULL;
HINSTANCE          g_hInst = NULL;




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    CMyD3DApplication d3dApp;

    g_pApp  = &d3dApp;
    g_hInst = hInst;

    if( FAILED( d3dApp.Create( hInst ) ) )
        return 0;

    return d3dApp.Run();
}




//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
    m_dwCreationWidth           = 500;
    m_dwCreationHeight          = 375;
    m_strWindowTitle            = TEXT( "DXTemplate" );
    m_bUseDepthBuffer           = TRUE;

    // Create a D3D font using d3dfont.cpp
    m_pFont                     = new CD3DFont( _T("Arial"), 8, D3DFONT_BOLD );
    m_bLoadingApp               = TRUE;
    m_pVB                       = NULL;
    m_pInputDeviceManager       = NULL;
    m_pDIConfigSurface          = NULL;

    ZeroMemory( &m_UserInput, sizeof(m_UserInput) );
    m_fWorldRotX                = 0.0f;
    m_fWorldRotY                = 0.0f;

	BSP_Nodehead = NULL;
	prev_BSP_Node = NULL;
	temp_BSP_Node = NULL;
	prev_start_node = NULL;
	m_IsStartNodeSet = false;

	Walker.SimpleInitialize(3.0f);
	Walker.lin_r = Vector3D(0.0f, 0.0f, -100.0f);
	
	m_til = 0.0f;
	num_nodes = 0;
	node_count = 0;

    // Read settings from registry
    ReadSettings();
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
    // TODO: perform one time initialization

    // Drawing loading status message until app finishes loading
    SendMessage( m_hWnd, WM_PAINT, 0, 0 );

    // Initialize DirectInput
    InitInput( m_hWnd );

    m_bLoadingApp = FALSE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadSettings()
// Desc: Read the app settings from the registry
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::ReadSettings()
{
    HKEY hkey;
    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // TODO: change as needed

        // Read the stored window width/height.  This is just an example,
        // of how to use DXUtil_Read*() functions.
        DXUtil_ReadIntRegKey( hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth );
        DXUtil_ReadIntRegKey( hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight );

        RegCloseKey( hkey );
    }
}




//-----------------------------------------------------------------------------
// Name: WriteSettings()
// Desc: Write the app settings to the registry
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::WriteSettings()
{
    HKEY hkey;
    DWORD dwType = REG_DWORD;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // TODO: change as needed

        // Write the window width/height.  This is just an example,
        // of how to use DXUtil_Write*() functions.
        DXUtil_WriteIntRegKey( hkey, TEXT("Width"), m_rcWindowClient.right );
        DXUtil_WriteIntRegKey( hkey, TEXT("Height"), m_rcWindowClient.bottom );

        RegCloseKey( hkey );
    }
}





//-----------------------------------------------------------------------------
// Name: StaticInputAddDeviceCB()
// Desc: Static callback helper to call into CMyD3DApplication class
//-----------------------------------------------------------------------------
HRESULT CALLBACK CMyD3DApplication::StaticInputAddDeviceCB( 
                                         CInputDeviceManager::DeviceInfo* pDeviceInfo, 
                                         const DIDEVICEINSTANCE* pdidi, 
                                         LPVOID pParam )
{
    CMyD3DApplication* pApp = (CMyD3DApplication*) pParam;
    return pApp->InputAddDeviceCB( pDeviceInfo, pdidi );
}




//-----------------------------------------------------------------------------
// Name: InputAddDeviceCB()
// Desc: Called from CInputDeviceManager whenever a device is added. 
//       Set the dead zone, and creates a new InputDeviceState for each device
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InputAddDeviceCB( CInputDeviceManager::DeviceInfo* pDeviceInfo, 
                                                   const DIDEVICEINSTANCE* pdidi )
{
    // Setup the deadzone 
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = 500;
    pDeviceInfo->pdidDevice->SetProperty( DIPROP_DEADZONE, &dipdw.diph );

    // Create a new InputDeviceState for each device so the 
    // app can record its state 
    InputDeviceState* pNewInputDeviceState = new InputDeviceState;
    ZeroMemory( pNewInputDeviceState, sizeof(InputDeviceState) );
    pDeviceInfo->pParam = (LPVOID) pNewInputDeviceState;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitInput()
// Desc: Initialize DirectInput objects
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitInput( HWND hWnd )
{
    HRESULT hr;

    // Setup action format for the actual gameplay
    ZeroMemory( &m_diafGame, sizeof(DIACTIONFORMAT) );
    m_diafGame.dwSize          = sizeof(DIACTIONFORMAT);
    m_diafGame.dwActionSize    = sizeof(DIACTION);
    m_diafGame.dwDataSize      = NUMBER_OF_GAMEACTIONS * sizeof(DWORD);
    m_diafGame.guidActionMap   = g_guidApp;

    // TODO: change the genre as needed
    m_diafGame.dwGenre         = DIVIRTUAL_CAD_3DCONTROL; 

    m_diafGame.dwNumActions    = NUMBER_OF_GAMEACTIONS;
    m_diafGame.rgoAction       = g_rgGameAction;
    m_diafGame.lAxisMin        = -100;
    m_diafGame.lAxisMax        = 100;
    m_diafGame.dwBufferSize    = 16;
    _tcscpy( m_diafGame.tszActionMap, _T("DXTemplate Game") );

    // Create a new input device manager
    m_pInputDeviceManager = new CInputDeviceManager();

    if( FAILED( hr = m_pInputDeviceManager->Create( hWnd, NULL, m_diafGame, 
                                                    StaticInputAddDeviceCB, this ) ) )
        return DXTRACE_ERR_NOMSGBOX( "m_pInputDeviceManager->Create", hr );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    BOOL bCapsAcceptable;

    // TODO: Perform checks to see if these display caps are acceptable.
    bCapsAcceptable = TRUE;

    if( bCapsAcceptable )         
        return S_OK;
    else
        return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
    // TODO: create device objects

	if(LoadModel() != S_OK)
		return E_FAIL;

	prev_start_node = &m_BSPNodeSet.m_pBSPNode[m_BSPNodeSet.m_nRootNode - 1];

	num_nodes = m_BSPNodeSet.m_numNodes;

    HRESULT hr;

    // Init the font
    m_pFont->InitDeviceObjects( m_pd3dDevice );


    if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer(3 * m_BSPNodeSet.m_numTriangles * sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_MANAGED, &m_pVB ) ) )
        return DXTRACE_ERR_NOMSGBOX( "CreateVertexBuffer", hr );

    CUSTOMVERTEX* pVertices;

    if( FAILED( hr = m_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
        return DXTRACE_ERR_NOMSGBOX( "Lock", hr );

	int count = 0, i, j;
	BSP_Node *temp;

	for(i = 0; i < num_nodes; i++)
	{
		temp = &m_BSPNodeSet.m_pBSPNode[i];
		temp->nVB = count;

		pVertices[count].x = temp->v[0].x;
		pVertices[count].y = temp->v[0].y;
		pVertices[count].z = temp->v[0].z;
		pVertices[count].u = temp->v[0].u;
		pVertices[count].v = temp->v[0].v;
		pVertices[count].Nx = temp->normal.x;
		pVertices[count].Ny = temp->normal.y;
		pVertices[count].Nz = temp->normal.z;

		pVertices[count + 1].x = temp->v[1].x;
		pVertices[count + 1].y = temp->v[1].y;
		pVertices[count + 1].z = temp->v[1].z;
		pVertices[count + 1].u = temp->v[1].u;
		pVertices[count + 1].v = temp->v[1].v;
		pVertices[count + 1].Nx = temp->normal.x;
		pVertices[count + 1].Ny = temp->normal.y;
		pVertices[count + 1].Nz = temp->normal.z;

		pVertices[count + 2].x = temp->v[2].x;
		pVertices[count + 2].y = temp->v[2].y;
		pVertices[count + 2].z = temp->v[2].z;
		pVertices[count + 2].u = temp->v[2].u;
		pVertices[count + 2].v = temp->v[2].v;
		pVertices[count + 2].Nx = temp->normal.x;
		pVertices[count + 2].Ny = temp->normal.y;
		pVertices[count + 2].Nz = temp->normal.z;

		count += 3;

		if(temp->numConTri > 0)
		{
			temp->nVB_Coplanar = new int[temp->numConTri];

			for(j = 0; j < temp->numConTri; j++)
			{
				temp->nVB_Coplanar[j] = count;

				pVertices[count].x = temp->pCoplanarTriangle[j].v[0].x;
				pVertices[count].y = temp->pCoplanarTriangle[j].v[0].y;
				pVertices[count].z = temp->pCoplanarTriangle[j].v[0].z;
				pVertices[count].u = temp->pCoplanarTriangle[j].v[0].u;
				pVertices[count].v = temp->pCoplanarTriangle[j].v[0].v;
				pVertices[count].Nx = temp->pNormal_CPTri[j].x;
				pVertices[count].Ny = temp->pNormal_CPTri[j].y;
				pVertices[count].Nz = temp->pNormal_CPTri[j].z;

				pVertices[count + 1].x = temp->pCoplanarTriangle[j].v[1].x;
				pVertices[count + 1].y = temp->pCoplanarTriangle[j].v[1].y;
				pVertices[count + 1].z = temp->pCoplanarTriangle[j].v[1].z;
				pVertices[count + 1].u = temp->pCoplanarTriangle[j].v[1].u;
				pVertices[count + 1].v = temp->pCoplanarTriangle[j].v[1].v;
				pVertices[count + 1].Nx = temp->pNormal_CPTri[j].x;
				pVertices[count + 1].Ny = temp->pNormal_CPTri[j].y;
				pVertices[count + 1].Nz = temp->pNormal_CPTri[j].z;

				pVertices[count + 2].x = temp->pCoplanarTriangle[j].v[2].x;
				pVertices[count + 2].y = temp->pCoplanarTriangle[j].v[2].y;
				pVertices[count + 2].z = temp->pCoplanarTriangle[j].v[2].z;
				pVertices[count + 2].u = temp->pCoplanarTriangle[j].v[2].u;
				pVertices[count + 2].v = temp->pCoplanarTriangle[j].v[2].v;
				pVertices[count + 2].Nx = temp->pNormal_CPTri[j].x;
				pVertices[count + 2].Ny = temp->pNormal_CPTri[j].y;
				pVertices[count + 2].Nz = temp->pNormal_CPTri[j].z;

				count += 3;
			}
		}

		if(temp->IsLeaf)
		{
			if(temp->numTriangles > 0)
			{
				temp->nVB_Leaf = new int[temp->numTriangles];

				for(j = 0; j < temp->numTriangles; j++)
				{
					temp->nVB_Leaf[j] = count;

					pVertices[count].x = temp->tri[j].v[0].x;
					pVertices[count].y = temp->tri[j].v[0].y;
					pVertices[count].z = temp->tri[j].v[0].z;
					pVertices[count].u = temp->tri[j].v[0].u;
					pVertices[count].v = temp->tri[j].v[0].v;
					pVertices[count].Nx = temp->Normal_Leaf[j].x;
					pVertices[count].Ny = temp->Normal_Leaf[j].y;
					pVertices[count].Nz = temp->Normal_Leaf[j].z;

					pVertices[count + 1].x = temp->tri[j].v[1].x;
					pVertices[count + 1].y = temp->tri[j].v[1].y;
					pVertices[count + 1].z = temp->tri[j].v[1].z;
					pVertices[count + 1].u = temp->tri[j].v[1].u;
					pVertices[count + 1].v = temp->tri[j].v[1].v;
					pVertices[count + 1].Nx = temp->Normal_Leaf[j].x;
					pVertices[count + 1].Ny = temp->Normal_Leaf[j].y;
					pVertices[count + 1].Nz = temp->Normal_Leaf[j].z;

					pVertices[count + 2].x = temp->tri[j].v[2].x;
					pVertices[count + 2].y = temp->tri[j].v[2].y;
					pVertices[count + 2].z = temp->tri[j].v[2].z;
					pVertices[count + 2].u = temp->tri[j].v[2].u;
					pVertices[count + 2].v = temp->tri[j].v[2].v;
					pVertices[count + 2].Nx = temp->Normal_Leaf[j].x;
					pVertices[count + 2].Ny = temp->Normal_Leaf[j].y;
					pVertices[count + 2].Nz = temp->Normal_Leaf[j].z;

					count += 3;
				}
			}
		}
	}

    m_pVB->Unlock();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restores scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
    // TODO: setup render states
    HRESULT hr;

    // Set up the textures
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

    // Set miscellaneous render states
    //m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    //m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	//m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

    // Set the world matrix
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );
    m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3(Walker.lin_r.x, Walker.lin_r.y, Walker.lin_r.z);
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtRH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // Set the projection matrix
    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
    D3DXMatrixPerspectiveFovRH( &matProj, D3DX_PI/4, fAspect, 1.0f, 400.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	vf.zn = 1.0f;
	vf.zf = 210.0f;
	vf.fov = D3DX_PI/4;
	vf.ViewPoint = Walker.lin_r;
	vf.AspectRatio = fAspect;

    // Set up lighting states
	
    D3DLIGHT8 light;
    D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 0.0f );
    m_pd3dDevice->SetLight( 0, &light );
    m_pd3dDevice->LightEnable( 0, TRUE );

    D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 1.0f, -1.0f, 0.0f );
    m_pd3dDevice->SetLight( 1, &light );
    m_pd3dDevice->LightEnable( 1, TRUE );

    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
 
    // Restore the font
    m_pFont->RestoreDeviceObjects();

    if( !m_bWindowed )
    {
        // Create a surface for configuring DInput devices
        if( FAILED( hr = m_pd3dDevice->CreateImageSurface( 640, 480, 
                                        m_d3dsdBackBuffer.Format, &m_pDIConfigSurface ) ) ) 
            return DXTRACE_ERR_NOMSGBOX( "CreateImageSurface", hr );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: StaticConfigureInputDevicesCB()
// Desc: Static callback helper to call into CMyD3DApplication class
//-----------------------------------------------------------------------------
BOOL CALLBACK CMyD3DApplication::StaticConfigureInputDevicesCB( 
                                            IUnknown* pUnknown, VOID* pUserData )
{
    CMyD3DApplication* pApp = (CMyD3DApplication*) pUserData;
    return pApp->ConfigureInputDevicesCB( pUnknown );
}




//-----------------------------------------------------------------------------
// Name: ConfigureInputDevicesCB()
// Desc: Callback function for configuring input devices. This function is
//       called in fullscreen modes, so that the input device configuration
//       window can update the screen.
//-----------------------------------------------------------------------------
BOOL CMyD3DApplication::ConfigureInputDevicesCB( IUnknown* pUnknown )
{
    // Get access to the surface
    LPDIRECT3DSURFACE8 pConfigSurface = NULL;
    if( FAILED( pUnknown->QueryInterface( IID_IDirect3DSurface8,
                                          (VOID**)&pConfigSurface ) ) )
        return TRUE;

    // Render the scene, with the config surface blitted on top
    Render();

    RECT  rcSrc;
    SetRect( &rcSrc, 0, 0, 640, 480 );

    POINT ptDst;
    ptDst.x = (m_d3dsdBackBuffer.Width-640)/2;
    ptDst.y = (m_d3dsdBackBuffer.Height-480)/2;

    LPDIRECT3DSURFACE8 pBackBuffer;
    m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    m_pd3dDevice->CopyRects( pConfigSurface, &rcSrc, 1, pBackBuffer, &ptDst );
    pBackBuffer->Release();

    // Present the backbuffer contents to the front buffer
    m_pd3dDevice->Present( 0, 0, 0, 0 );

    // Release the surface
    pConfigSurface->Release();

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{
    // TODO: update world

    // Update user input state
    UpdateInput( &m_UserInput );

    // Respond to input
    if( m_UserInput.bDoConfigureInput )
    {
        // One-shot per keypress
        m_UserInput.bDoConfigureInput = FALSE;

        Pause( TRUE );

        // Get access to the list of semantically-mapped input devices
        // to delete all InputDeviceState structs before calling ConfigureDevices()
        CInputDeviceManager::DeviceInfo* pDeviceInfos;
        DWORD dwNumDevices;
        m_pInputDeviceManager->GetDevices( &pDeviceInfos, &dwNumDevices );

        for( DWORD i=0; i<dwNumDevices; i++ )
        {
            InputDeviceState* pInputDeviceState = (InputDeviceState*) pDeviceInfos[i].pParam;
            SAFE_DELETE( pInputDeviceState );
            pDeviceInfos[i].pParam = NULL;
        }

        // Configure the devices (with edit capability)
        if( m_bWindowed )
            m_pInputDeviceManager->ConfigureDevices( m_hWnd, NULL, NULL, DICD_EDIT, NULL );
        else
            m_pInputDeviceManager->ConfigureDevices( m_hWnd,
                                                     m_pDIConfigSurface,
                                                     (VOID*)StaticConfigureInputDevicesCB,
                                                     DICD_EDIT, (LPVOID) this );

        Pause( FALSE );
    }

    if( m_UserInput.bDoConfigureDisplay )
    {
        // One-shot per keypress
        m_UserInput.bDoConfigureDisplay = FALSE;

        Pause(TRUE);

        // Configure the display device
        UserSelectNewDevice();

        Pause(FALSE);
    }

    // Update the world state according to user input
	
    D3DXMATRIX matWorld;
    D3DXMATRIX matRotY;
    D3DXMATRIX matRotX;

    D3DXMatrixRotationX( &matRotX, m_fWorldRotX );
    D3DXMatrixRotationY( &matRotY, m_fWorldRotY );

    D3DXMatrixMultiply( &matWorld, &matRotX, &matRotY );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//Advance walker's position and view.
	float r, rot;
    if( m_UserInput.fAxisRotateLR )
		rot = m_UserInput.fAxisRotateLR * 5.0f;
	else
		rot = 0.0f;

    if( m_UserInput.fAxisRotateUD )
		r = m_UserInput.fAxisRotateUD * 50.0f;
	else
		r = 0.0f;

	Walker.lin_mom_local.z = r;
	Walker.ang_mom.y = rot;
	Walker.AdvanceKinematics(m_fElapsedTime);

	//Reset valuables to initial states.
	r = 0.0f;
	rot = 0.0f;
	Walker.lin_mom_local = Vector3D(0.0f, 0.0f, 0.0f);
	Walker.lin_mom = Vector3D(0.0f, 0.0f, 0.0f);
	Walker.ang_mom = Vector3D(0.0f, 0.0f, 0.0f);
	node_count = 0;

	//Update states of viewing frustum.
	vf.Update(Walker.lin_r, Walker.Rot_mat);

    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3(Walker.lin_r.x, Walker.lin_r.y, Walker.lin_r.z);
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3(vf.LookPoint.x, vf.LookPoint.y, vf.LookPoint.z);
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtRH(&matView, &vFromPt, &vLookatPt, &vUpVec);
    m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

    // Set the projection matrix
    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
    D3DXMatrixPerspectiveFovRH( &matProj, D3DX_PI/4, fAspect, 1.0f, 400.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Update the user input.  Called once per frame 
//-----------------------------------------------------------------------------
void CMyD3DApplication::UpdateInput( UserInput* pUserInput )
{
    if( NULL == m_pInputDeviceManager )
        return;

    // Get access to the list of semantically-mapped input devices
    CInputDeviceManager::DeviceInfo* pDeviceInfos;
    DWORD dwNumDevices;
    m_pInputDeviceManager->GetDevices( &pDeviceInfos, &dwNumDevices );

    // Loop through all devices and check game input
    for( DWORD i=0; i<dwNumDevices; i++ )
    {
        DIDEVICEOBJECTDATA rgdod[10];
        DWORD   dwItems = 10;
        HRESULT hr;
        LPDIRECTINPUTDEVICE8 pdidDevice = pDeviceInfos[i].pdidDevice;
        InputDeviceState* pInputDeviceState = (InputDeviceState*) pDeviceInfos[i].pParam;

        hr = pdidDevice->Acquire();
        hr = pdidDevice->Poll();
        hr = pdidDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                        rgdod, &dwItems, 0 );
        if( FAILED(hr) )
            continue;

        // Get the sematics codes for the game menu
        for( DWORD j=0; j<dwItems; j++ )
        {
            BOOL  bButtonState = (rgdod[j].dwData==0x80) ? TRUE : FALSE;
            FLOAT fButtonState = (rgdod[j].dwData==0x80) ? 1.0f : 0.0f;
            FLOAT fAxisState   = (FLOAT)((int)rgdod[j].dwData)/100.0f;

            switch( rgdod[j].uAppData )
            {
                // TODO: Handle semantics for the game 

                // Handle relative axis data
                case INPUT_ROTATE_AXIS_LR: 
                    pInputDeviceState->fAxisRotateLR = -fAxisState;
                    break;
                case INPUT_ROTATE_AXIS_UD:
                    pInputDeviceState->fAxisRotateUD = -fAxisState;
                    break;

                // Handle buttons separately so the button state data
                // doesn't overwrite the axis state data, and handle
                // each button separately so they don't overwrite each other
                case INPUT_ROTATE_LEFT:  pInputDeviceState->bButtonRotateLeft  = bButtonState; break;
                case INPUT_ROTATE_RIGHT: pInputDeviceState->bButtonRotateRight = bButtonState; break;
                case INPUT_ROTATE_UP:    pInputDeviceState->bButtonRotateUp    = bButtonState; break;
                case INPUT_ROTATE_DOWN:  pInputDeviceState->bButtonRotateDown  = bButtonState; break;

                // Handle one-shot buttons
                case INPUT_CONFIG_INPUT:   if( bButtonState ) pUserInput->bDoConfigureInput = TRUE; break;
                case INPUT_CONFIG_DISPLAY: if( bButtonState ) pUserInput->bDoConfigureDisplay = TRUE; break;
            }
        }
    }

    // TODO: change process code as needed

    // Process user input and store result into pUserInput struct
    pUserInput->fAxisRotateLR = 0.0f;
    pUserInput->fAxisRotateUD = 0.0f;

    // Concatinate the data from all the DirectInput devices
    for( i=0; i<dwNumDevices; i++ )
    {
        InputDeviceState* pInputDeviceState = (InputDeviceState*) pDeviceInfos[i].pParam;

        // Use the axis data that is furthest from zero
        if( fabs(pInputDeviceState->fAxisRotateLR) > fabs(pUserInput->fAxisRotateLR) )
            pUserInput->fAxisRotateLR = pInputDeviceState->fAxisRotateLR;

        if( fabs(pInputDeviceState->fAxisRotateUD) > fabs(pUserInput->fAxisRotateUD) )
            pUserInput->fAxisRotateUD = pInputDeviceState->fAxisRotateUD;

        // Process the button data 
        if( pInputDeviceState->bButtonRotateLeft )
            pUserInput->fAxisRotateLR = 1.0f;
        else if( pInputDeviceState->bButtonRotateRight )
            pUserInput->fAxisRotateLR = -1.0f;

        if( pInputDeviceState->bButtonRotateUp )
            pUserInput->fAxisRotateUD = 1.0f;
        else if( pInputDeviceState->bButtonRotateDown )
            pUserInput->fAxisRotateUD = -1.0f;

    } 
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
    // Clear the viewport
    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         0x000000ff, 1.0f, 0L );

    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        // TODO: render world
        // Render the vertex buffer contents
		m_IsStartNodeSet = false;

        m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(CUSTOMVERTEX) );
        m_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );

/*
		float var, w[5];
		var = prev_start_node->v[0].x * prev_start_node->normal.x + prev_start_node->v[0].y * prev_start_node->normal.y + prev_start_node->v[0].z * prev_start_node->normal.z;
		w[0] = (vf.ViewPoint * prev_start_node->normal) - var;
		w[1] = (vf.LowerLeft * prev_start_node->normal) - var;
		w[2] = (vf.LowerRight * prev_start_node->normal) - var;
		w[3] = (vf.UpperLeft * prev_start_node->normal) - var;
		w[4] = (vf.UpperRight * prev_start_node->normal) - var;

		if(!(w[0] < 0.0f && w[1] < 0.0f && w[2] < 0.0f && w[3] < 0.0f && w[4] < 0.0f) && !(w[0] >= 0.0f && w[1] >= 0.0f && w[2] >= 0.0f && w[3] >= 0.0f && w[4] >= 0.0f))
		{
			prev_start_node = &m_BSPNodeSet.m_pBSPNode[m_BSPNodeSet.m_nRootNode - 1];
		}
*/
		//DrawNode(BSP_Tree.treehead);
		//DrawNode(prev_start_node);
		DrawNode(&m_BSPNodeSet.m_pBSPNode[m_BSPNodeSet.m_nRootNode - 1]);

        // Render stats and help text  
        RenderText();

        // End the scene.
        m_pd3dDevice->EndScene();
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RenderText()
// Desc: Renders stats and help text to the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RenderText()
{
    D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
    TCHAR szMsg[MAX_PATH] = TEXT("");

    // Output display stats
    //FLOAT fNextLine = 40.0f; 

    lstrcpy( szMsg, m_strFrameStats );
    m_pFont->DrawText( 2, 0.0f, fontColor, szMsg );

    sprintf( szMsg, TEXT("Total # of nodes: %u"), num_nodes);
	m_pFont->DrawText( 2, 20.0f, fontColor, szMsg );

    sprintf( szMsg, TEXT(" # of nodes drawn: %u"), node_count);
	m_pFont->DrawText( 2, 40.0f, fontColor, szMsg );

    sprintf( szMsg, TEXT("x: %0.2f y: %0.2f z: %0.2f"), Walker.lin_r.x, Walker.lin_r.y, Walker.lin_r.z);
	m_pFont->DrawText( 2, 60.0f, fontColor, szMsg );

    lstrcpy( szMsg, m_strDeviceStats );
    m_pFont->DrawText( 2, 80.0f, fontColor, szMsg );

    // Output statistics & help
    //fNextLine = (FLOAT) m_d3dsdBackBuffer.Height; 

    //sprintf( szMsg, TEXT("Left/Right Axis: %0.2f Up/Down Axis: %0.2f "), 
              //m_UserInput.fAxisRotateLR, m_UserInput.fAxisRotateUD );
    //fNextLine -= 20.0f; m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );

    //lstrcpy( szMsg, TEXT("Use arrow keys or joystick to rotate object") );
    //fNextLine -= 20.0f; m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );


    //lstrcpy( szMsg, TEXT("Press 'F2' to configure display") );
    //fNextLine -= 20.0f; m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::Pause( BOOL bPause )
{
    // Get access to the list of semantically-mapped input devices
    // to zero the state of all InputDeviceState structs.  This is needed
    // because when using DISCL_FOREGROUND, the action mapper will not 
    // record actions when the focus switches, for example if a dialog appears.
    // This causes a problem when a button held down when loosing focus, and let
    // go when the focus is lost.  The app will not record that the button 
    // has been let go, so the state will be incorrect when focus returns.  
    // To fix this either use DISCL_BACKGROUND or zero the state when 
    // loosing focus.
    CInputDeviceManager::DeviceInfo* pDeviceInfos;
    DWORD dwNumDevices;
    m_pInputDeviceManager->GetDevices( &pDeviceInfos, &dwNumDevices );

    for( DWORD i=0; i<dwNumDevices; i++ )
    {
        InputDeviceState* pInputDeviceState = (InputDeviceState*) pDeviceInfos[i].pParam;
        ZeroMemory( pInputDeviceState, sizeof(InputDeviceState) );
    }

    CD3DApplication::Pause( bPause );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Overrrides the main WndProc, so the sample can do custom message
//       handling (e.g. processing mouse, keyboard, or menu commands).
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam )
{
    switch( msg )
    {
        case WM_PAINT:
        {
            if( m_bLoadingApp )
            {
                // Draw on the window tell the user that the app is loading
                // TODO: change as needed
                HDC hDC = GetDC( hWnd );
                TCHAR strMsg[MAX_PATH];
                wsprintf( strMsg, TEXT("Loading... Please wait") );
                RECT rct;
                GetClientRect( hWnd, &rct );
                DrawText( hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
                ReleaseDC( hWnd, hDC );
            }
            break;
        }

        case WM_COMMAND:
        {
            switch( LOWORD(wParam) )
            {
                case IDM_CONFIGINPUT:
                    m_UserInput.bDoConfigureInput = TRUE;
                    break;

                case IDM_CHANGEDEVICE:
                    m_UserInput.bDoConfigureDisplay = TRUE;
                    return 0; // Don't hand off to parent
            }
            break;
        }

    }

    return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Invalidates device objects.  
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
    // TODO: Cleanup any objects created in RestoreDeviceObjects()
    m_pFont->InvalidateDeviceObjects();
    SAFE_RELEASE( m_pDIConfigSurface );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.  
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
    // TODO: Cleanup any objects created in InitDeviceObjects()
    m_pFont->DeleteDeviceObjects();
    SAFE_RELEASE( m_pVB );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
    // TODO: Perform any final cleanup needed
    // Cleanup D3D font
    SAFE_DELETE( m_pFont );

    // Cleanup DirectInput
    CleanupDirectInput();

    // Write the settings to the registry
    WriteSettings();

	/*
	if(BSP_Nodehead != NULL)
	{
		BSP_Node *temp1, *temp2;
		temp1 = BSP_Nodehead;

		while(temp1 != NULL)
		{
			temp2 = temp1->Next;
			delete temp1;
			temp1 = temp2;
		}
	}
*/

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CleanupDirectInput()
// Desc: Cleanup DirectInput 
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::CleanupDirectInput()
{
    if( NULL == m_pInputDeviceManager )
        return;

    // Get access to the list of semantically-mapped input devices
    // to delete all InputDeviceState structs
    CInputDeviceManager::DeviceInfo* pDeviceInfos;
    DWORD dwNumDevices;
    m_pInputDeviceManager->GetDevices( &pDeviceInfos, &dwNumDevices );

    for( DWORD i=0; i<dwNumDevices; i++ )
    {
        InputDeviceState* pInputDeviceState = (InputDeviceState*) pDeviceInfos[i].pParam;
        SAFE_DELETE( pInputDeviceState );
        pDeviceInfos[i].pParam = NULL;
    }

    // Cleanup DirectX input objects
    SAFE_DELETE( m_pInputDeviceManager );

}

HRESULT CMyD3DApplication::LoadModel()
{
	if(!m_BSPNodeSet.LoadBSPNodeFromFile("scene.bsp"))
	{
		MessageBox(m_hWnd, "Failed to BSP model file", "Error!", MB_OK);
		return E_FAIL;
	}
	
	//Assign texture according to material[i].

	if(m_BSPNodeSet.m_numMaterials > 0)
	{
		for(int i = 0; i < m_BSPNodeSet.m_numMaterials; i++)
		{
			if(m_BSPNodeSet.m_pMaterial[i].hasDiffuseTexture)
			{
				if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, m_BSPNodeSet.m_pMaterial[i].DiffuseTexture,
													   &m_BSPNodeSet.m_pMaterial[i].g_pTexture) ) )
				{
					 MessageBox(m_hWnd, "Open texture file error!", "Error", MB_OK);
					 return false;
				}

				 m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
				 m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );	
			}
				
			if(m_BSPNodeSet.m_pMaterial[i].hasAlphaTexture)
			{
				if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, m_BSPNodeSet.m_pMaterial[i].AlphaTexture,
													   &m_BSPNodeSet.m_pMaterial[i].g_pAlphaTexture) ) )
				{
					 MessageBox(m_hWnd, "Open texture file error!", "Error", MB_OK);
					 return false;
				}
					
				m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
				m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
			}

		}// End loop model's materials.
	}

	return S_OK;
}

void CMyD3DApplication::DrawNode(BSP_Node *node)
{
	float var;

	if(node->IsLeaf)
	{
		// Draw this triangle in node.
		SetMaterial(node);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, node->nVB, 1);

		DrawCoplanars(node);
		DrawLeaf(node);

		node_count++;

		return;
	}

	//Check for viewing frustum with current partitioning plane.
	float w[5];
	var = node->v[0].x * node->normal.x + node->v[0].y * node->normal.y + node->v[0].z * node->normal.z;
	w[0] = (vf.ViewPoint * node->normal) - var;
	w[1] = (vf.LowerLeft * node->normal) - var;
	w[2] = (vf.LowerRight * node->normal) - var;
	w[3] = (vf.UpperLeft * node->normal) - var;
	w[4] = (vf.UpperRight * node->normal) - var;

	if(w[0] < 0.0f && w[1] < 0.0f && w[2] < 0.0f && w[3] < 0.0f && w[4] < 0.0f)
	{
		//All are at negative side.

		if(node->Neg_Child != NULL)
			DrawNode(node->Neg_Child);
		else
		{
			// Draw this triangle in node.
			SetMaterial(node);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, node->nVB, 1);
			DrawCoplanars(node);

			node_count++;

			return;
		}
	}
	else
	{
		if(w[0] >= 0.0f && w[1] >= 0.0f && w[2] >= 0.0f && w[3] >= 0.0f && w[4] >= 0.0f)
		{
			//All are at positive side.
	
			if(node->Pos_Child != NULL)
				DrawNode(node->Pos_Child);
			else
			{
				// Draw this triangle in node.
				SetMaterial(node);
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, node->nVB, 1);
				DrawCoplanars(node);

				node_count++;

				return;
			}
		}
		else
		{
			//////////////////////////////////////////////
			// Further consider this node.
			//Frustum is intersecting with node's plane, so consider both sides.
			//Check for which side viewport does loacate on.
			var = (Walker.lin_r * node->normal) - (node->v[0].x * node->normal.x + node->v[0].y * node->normal.y + node->v[0].z * node->normal.z);

			if(var < 0.0f)
			{
				//On negative side.
				//Draw opposite side, positive.
				if(node->Pos_Child != NULL)
					DrawNode(node->Pos_Child);
			}
			else
			{
				//On positive side even var = 0.
				//Draw opposite side, negative.
				if(node->Neg_Child != NULL)
					DrawNode(node->Neg_Child);
			}
				
			// Draw this triangle in node.
			SetMaterial(node);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, node->nVB, 1);
			DrawCoplanars(node);

			node_count++;

			if(var < 0.0f)
			{
				//On negative side.
				//Draw this side, negative.
				if(node->Neg_Child != NULL)
					DrawNode(node->Neg_Child);
			}
			else
			{
				//On positive side even var = 0.
				//Draw this side, positive.
				if(node->Pos_Child != NULL)
					DrawNode(node->Pos_Child);
			}
			/////////////////////////////////////////////////////
		}
	}
}

void CMyD3DApplication::SetMaterial(BSP_Node *node)
{
	D3DMATERIAL8 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL8));

	if(node->MaterialIndex >= 0)
	{	
		mtrl.Ambient.r = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Ambient[0];
		mtrl.Ambient.g = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Ambient[1];
		mtrl.Ambient.b = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Ambient[2];
		mtrl.Ambient.a = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Ambient[3];
					
		mtrl.Diffuse.r = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Diffuse[0];
		mtrl.Diffuse.g = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Diffuse[1];
		mtrl.Diffuse.b = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Diffuse[2];
		mtrl.Diffuse.a = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Diffuse[3];

		mtrl.Specular.r = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Specular[0];
		mtrl.Specular.g = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Specular[1];
		mtrl.Specular.b = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Specular[2];
		mtrl.Specular.a = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Specular[3];

		mtrl.Emissive.r = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Emissive[0];
		mtrl.Emissive.g = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Emissive[1];
		mtrl.Emissive.b = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Emissive[2];
		mtrl.Emissive.a = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Emissive[3];		
		mtrl.Power = m_BSPNodeSet.m_pMaterial[node->MaterialIndex].Shininess;
			
		m_pd3dDevice->SetMaterial(&mtrl);

		if(m_BSPNodeSet.m_pMaterial[node->MaterialIndex].hasDiffuseTexture)
		{
			m_pd3dDevice->SetTexture(0, m_BSPNodeSet.m_pMaterial[node->MaterialIndex].g_pTexture); 
			//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		}
		else
			m_pd3dDevice->SetTexture(0, NULL);

		if(m_BSPNodeSet.m_pMaterial[node->MaterialIndex].hasAlphaTexture)
		{
			m_pd3dDevice->SetTexture(1, m_BSPNodeSet.m_pMaterial[node->MaterialIndex].g_pAlphaTexture); 
			//m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );				
		}
		else
			m_pd3dDevice->SetTexture(1, NULL);
	}
	else
	{
		mtrl.Diffuse.r = mtrl.Ambient.r = 0.4f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 0.8f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 0.8f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		mtrl.Specular.r = 0.3f;
		mtrl.Specular.g = 0.3f;
		mtrl.Specular.b = 0.3f;
		mtrl.Specular.a = 1.0f;
		mtrl.Power = 0.3f;
		m_pd3dDevice->SetMaterial(&mtrl);
	}
}

bool CMyD3DApplication::IsInFrustum(BSP_Node *node)
{
	float var[5];
	Vector3D v[3];

	v[0] = Vector3D(node->v[0].x, node->v[0].y, node->v[0].z);
	v[1] = Vector3D(node->v[1].x, node->v[1].y, node->v[1].z);
	v[2] = Vector3D(node->v[2].x, node->v[2].y, node->v[2].z);

	for(int i = 0; i < 3; i++)
	{
		var[0] = (v[i] * vf.LP_Normal) - (vf.LP_Normal * vf.ViewPoint);
		var[1] = (v[i] * vf.RP_Normal) - (vf.RP_Normal * vf.ViewPoint);
		var[2] = (v[i] * vf.UP_Normal) - (vf.UP_Normal * vf.ViewPoint);
		var[3] = (v[i] * vf.BP_Normal) - (vf.BP_Normal * vf.ViewPoint);
		var[4] = (v[i] * vf.FP_Normal) - (vf.FP_Normal * vf.UpperRight);

		if(var[0] <= 0.0f && var[1] <= 0.0f && var[2] <= 0.0f && var[3] <= 0.0f && var[4] <= 0.0f)
			return true;
	}
	return false;
}

bool CMyD3DApplication::IsFrustumCross(float *w, BSP_Node *node)
{
	float d1, d2;
	Vector3D var1;

	d1 = node->v[0].x * node->normal.x + node->v[0].y * node->normal.y + node->v[0].z * node->normal.z;

	if(node->Parent != NULL)
		d2 = node->Parent->v[0].x * node->Parent->normal.x + node->Parent->v[0].y * node->Parent->normal.y + node->Parent->v[0].z * node->Parent->normal.z;
	else
		return true;

	if(IsLineCross(vf.ViewPoint, vf.UpperLeft, d1, d2, node))
		return true;
	if(IsLineCross(vf.ViewPoint, vf.LowerLeft, d1, d2, node))
		return true;
	if(IsLineCross(vf.ViewPoint, vf.UpperRight, d1, d2, node))
		return true;
	if(IsLineCross(vf.ViewPoint, vf.LowerRight, d1, d2, node))
		return true;

	if(IsLineCross(vf.LowerLeft, vf.UpperLeft, d1, d2, node))
		return true;
	if(IsLineCross(vf.LowerRight, vf.UpperRight, d1, d2, node))
		return true;

	if(IsLineCross(vf.LowerRight, vf.LowerLeft, d1, d2, node))
		return true;
	if(IsLineCross(vf.UpperRight, vf.UpperLeft, d1, d2, node))
		return true;

	return false;
}

bool CMyD3DApplication::IsLineCross(Vector3D r1, Vector3D r2, float d1, float d2, BSP_Node *node)
{
	float var2, side_val, t;
	Vector3D r;

	r = r2 - r1;
	var2 = r * node->normal;

	if(var2 != 0.0f)
		t = (d1 - (r1 * node->normal)) / var2;
	else
		//t = -1.0f;
		return false;

	if(t >= 0.0f && t <= 1.0f)
	{
		Vector3D p;

		p = r1 + (r * t);

		side_val = (p * node->Parent->normal) - d2;
		if(node->side == 0 && side_val >= 0.0f)
			return true;
		else
		{
			if(node->side == 1 && side_val < 0.0f)
				return true;
		}

	}
	
	return false;
}

void CMyD3DApplication::DrawLeaf(BSP_Node *pLeaf)
{
	if(pLeaf->IsLeaf)
	//if(pLeaf->numTriangles > 0)
	{
		int i;

		for(i = 0; i < pLeaf->numTriangles; i++)
		{
			SetMaterialByIndex(pLeaf->nMaterial_Leaf[i]);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, pLeaf->nVB_Leaf[i], 1);
		}
	}
}

void CMyD3DApplication::SetMaterialByIndex(int index)
{
	D3DMATERIAL8 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL8));

		if(index >= 0)
		{	
			mtrl.Ambient.r = m_BSPNodeSet.m_pMaterial[index].Ambient[0];
			mtrl.Ambient.g = m_BSPNodeSet.m_pMaterial[index].Ambient[1];
			mtrl.Ambient.b = m_BSPNodeSet.m_pMaterial[index].Ambient[2];
			mtrl.Ambient.a = m_BSPNodeSet.m_pMaterial[index].Ambient[3];
					
			mtrl.Diffuse.r = m_BSPNodeSet.m_pMaterial[index].Diffuse[0];
			mtrl.Diffuse.g = m_BSPNodeSet.m_pMaterial[index].Diffuse[1];
			mtrl.Diffuse.b = m_BSPNodeSet.m_pMaterial[index].Diffuse[2];
			mtrl.Diffuse.a = m_BSPNodeSet.m_pMaterial[index].Diffuse[3];

			mtrl.Specular.r = m_BSPNodeSet.m_pMaterial[index].Specular[0];
			mtrl.Specular.g = m_BSPNodeSet.m_pMaterial[index].Specular[1];
			mtrl.Specular.b = m_BSPNodeSet.m_pMaterial[index].Specular[2];
			mtrl.Specular.a = m_BSPNodeSet.m_pMaterial[index].Specular[3];

			mtrl.Emissive.r = m_BSPNodeSet.m_pMaterial[index].Emissive[0];
			mtrl.Emissive.g = m_BSPNodeSet.m_pMaterial[index].Emissive[1];
			mtrl.Emissive.b = m_BSPNodeSet.m_pMaterial[index].Emissive[2];
			mtrl.Emissive.a = m_BSPNodeSet.m_pMaterial[index].Emissive[3];		
			mtrl.Power = m_BSPNodeSet.m_pMaterial[index].Shininess;
			
			m_pd3dDevice->SetMaterial(&mtrl);

			if(m_BSPNodeSet.m_pMaterial[index].hasDiffuseTexture)
			{
				m_pd3dDevice->SetTexture(0, m_BSPNodeSet.m_pMaterial[index].g_pTexture); 
				//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	 			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
				
			}
			else
				m_pd3dDevice->SetTexture(0, NULL);

			if(m_BSPNodeSet.m_pMaterial[index].hasAlphaTexture)
			{
				m_pd3dDevice->SetTexture(1, m_BSPNodeSet.m_pMaterial[index].g_pAlphaTexture); 
				//m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
				m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
				m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );				
				
			}
			else
				m_pd3dDevice->SetTexture(1, NULL);
		}
		else
		{

			mtrl.Diffuse.r = mtrl.Ambient.r = 0.4f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 0.8f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 0.8f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
			mtrl.Specular.r = 0.3f;
			mtrl.Specular.g = 0.3f;
			mtrl.Specular.b = 0.3f;
			mtrl.Specular.a = 1.0f;
			mtrl.Power = 0.3f;
			m_pd3dDevice->SetMaterial(&mtrl);
		}
}

void CMyD3DApplication::DrawCoplanars(BSP_Node *pNode)
{
	if(pNode->numConTri > 0)
	{
		int i;

		for(i = 0; i < pNode->numConTri; i++)
		{
			SetMaterialByIndex(pNode->p_nMaterial_CPTri[i]);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, pNode->nVB_Coplanar[i], 1);
		}
	}
}
