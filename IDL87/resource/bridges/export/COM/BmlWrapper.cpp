// This file contains methods that are common to all wrapper objects instances
//
#include "stdafx.h"
#include "BmlWrapper.h"
#include "LoadIdlLibrary.h"

extern HINSTANCE g_hInst;

#include "Wrapper.cpp"

CBmlWRAPPER_IMPLEMENT_IDLNOTIFYCB
CBmlWRAPPER_IMPLEMENT_IDLOUTPUTCB


//-----------------------------------------------------------------------------
CBmlWRAPPER::CBmlWRAPPER()
{
	// These three are #defined'd in Wrapper.cpp
    m_bstrIdlObjIdentifier = CBmlWRAPPER_IDLOBJNAME;
    m_OPSControlFlags      = CBmlWRAPPER_BMLPROCCONTROLFLAGS;
    m_bstrProcName         = CBmlWRAPPER_BMLEXTPROCNAME;

    m_cookie = -1;
    m_bObjCreated = FALSE;

#if ( defined(BMLWRAPPER_ISACTIVEXCTRL) && BMLWRAPPER_ISACTIVEXCTRL == 1)
    m_hImage= NULL;
    m_bCursorIn = FALSE;
    m_bCurrRuntimeMode = FALSE;
	m_lastMouseX = 0;
	m_lastMouseY = 0;

    // This is critical to keep the ActiveX Host window from
    // creating this control as "windowless".
    //
    m_bWindowOnly = TRUE;

    memset(&m_bmpInfo,  0, sizeof(BITMAP) );
	memset(&m_rcClient, 0, sizeof(RECT)   );
#endif

	ResetMSCOMPALPtrs();
}


CBmlWRAPPER::~CBmlWRAPPER()
{
}



//-----------------------------------------------------------------------------
HRESULT CBmlWRAPPER::FinalConstruct()
{
    return S_OK;
}

void CBmlWRAPPER::FinalRelease()
{
	int cStrs = (int)m_vPendingIDLOutput.size();
	for ( int i = 0; i < cStrs; i++ )
	{
		BSTR bstr = m_vPendingIDLOutput[i];
		if ( bstr )
			::SysFreeString(bstr);
	}
	m_vPendingIDLOutput.clear();


#if ( defined(BMLWRAPPER_ISACTIVEXCTRL) && BMLWRAPPER_ISACTIVEXCTRL == 1)
    if ( m_hImage )
    {
        DeleteObject(m_hImage);
        m_hImage = NULL;
    }
#endif

	ResetMSCOMPALPtrs();
}



//-----------------------------------------------------------------------------
void CBmlWRAPPER::ResetMSCOMPALPtrs(void)
{

	IDL_MSCOMPAL_Initialize = NULL;
	IDL_MSCOMPAL_Release = NULL;
	IDL_MSCOMPAL_RegisterWrapper = NULL;
	IDL_MSCOMPAL_ReleaseWrapper = NULL;
	IDL_MSCOMPAL_CreateObject = NULL;
	IDL_MSCOMPAL_DestroyObject = NULL;
	IDL_MSCOMPAL_OnEvent = NULL;
	IDL_MSCOMPAL_CallMethod = NULL;
	IDL_MSCOMPAL_SetProperty = NULL;
	IDL_MSCOMPAL_GetProperty = NULL;
	IDL_MSCOMPAL_Abort = NULL;
	IDL_MSCOMPAL_GetLastError = NULL;
	IDL_MSCOMPAL_RegisterOutputCallback = NULL;
	IDL_MSCOMPAL_UnregisterOutputCallback = NULL;
	IDL_MSCOMPAL_RegisterNotifyCallback = NULL;
	IDL_MSCOMPAL_UnregisterNotifyCallback = NULL;
	IDL_MSCOMPAL_SetIDLVariable = NULL;
	IDL_MSCOMPAL_ExecuteString = NULL;
	IDL_MSCOMPAL_GetObjVariableName = NULL;
	IDL_MSCOMPAL_GetObjClassName = NULL;
	IDL_MSCOMPAL_GetProcNameFromCookie = NULL;

	m_hPALModule   = NULL;
	m_bInitialized = FALSE;
}


//-----------------------------------------------------------------------------
// This is called by the VALIDATE_PAL macros, which has been added to
// to the VALIDATE_OBJECT macro.
//
BOOL CBmlWRAPPER::InternalInitialize(void)
{
	if ( m_bInitialized )
		return TRUE;

	if ( !LoadIdlLibrary(_T("idl_pal_mscom.dll"),_T("MscomPalDir"),&m_hPALModule) )
	{
		char szErr[1024];
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPSTR) &lpMsgBuf,
			0,
			NULL);
		_snprintf(szErr,MAX_ERR_SIZE,"LoadLibrary(idl_pal_mscom.dll) failed: %s\n",(char*)lpMsgBuf);
		m_bstrLastError = szErr;

		LocalFree( lpMsgBuf );
		return FALSE;
	}

	GETPROC( MSCOMPAL_Initialize_T,				    IDL_MSCOMPAL_Initialize );
	GETPROC( MSCOMPAL_Release_T,					IDL_MSCOMPAL_Release );
	GETPROC( MSCOMPAL_RegisterWrapper_T,			IDL_MSCOMPAL_RegisterWrapper );
	GETPROC( MSCOMPAL_ReleaseWrapper_T,			    IDL_MSCOMPAL_ReleaseWrapper );
	GETPROC( MSCOMPAL_CreateObject_T,			    IDL_MSCOMPAL_CreateObject );
	GETPROC( MSCOMPAL_DestroyObject_T,			    IDL_MSCOMPAL_DestroyObject );
	GETPROC( MSCOMPAL_OnEvent_T,					IDL_MSCOMPAL_OnEvent );
	GETPROC( MSCOMPAL_CallMethod_T,				    IDL_MSCOMPAL_CallMethod );
	GETPROC( MSCOMPAL_SetProperty_T,				IDL_MSCOMPAL_SetProperty );
	GETPROC( MSCOMPAL_GetProperty_T,				IDL_MSCOMPAL_GetProperty );
	GETPROC( MSCOMPAL_Abort_T,					    IDL_MSCOMPAL_Abort );
	GETPROC( MSCOMPAL_GetLastError_T,			    IDL_MSCOMPAL_GetLastError );
	GETPROC( MSCOMPAL_RegisterOutputCallback_T,	    IDL_MSCOMPAL_RegisterOutputCallback );
	GETPROC( MSCOMPAL_UnregisterOutputCallback_T,	IDL_MSCOMPAL_UnregisterOutputCallback );
	GETPROC( MSCOMPAL_RegisterNotifyCallback_T,		IDL_MSCOMPAL_RegisterNotifyCallback );
	GETPROC( MSCOMPAL_UnregisterNotifyCallback_T,   IDL_MSCOMPAL_UnregisterNotifyCallback );
	GETPROC( MSCOMPAL_SetIDLVariable_T,			    IDL_MSCOMPAL_SetIDLVariable );
	GETPROC( MSCOMPAL_GetIDLVariable_T,			    IDL_MSCOMPAL_GetIDLVariable );
	GETPROC( MSCOMPAL_ExecuteString_T,			    IDL_MSCOMPAL_ExecuteString );
	GETPROC( MSCOMPAL_GetObjVariableName_T,		    IDL_MSCOMPAL_GetObjVariableName );
	GETPROC( MSCOMPAL_GetObjClassName_T,			IDL_MSCOMPAL_GetObjClassName );
	GETPROC( MSCOMPAL_GetProcNameFromCookie_T,	    IDL_MSCOMPAL_GetProcNameFromCookie );

	if ( FAILED(IDL_MSCOMPAL_Initialize()) )
	{
		if ( IDL_MSCOMPAL_GetLastError )
			 IDL_MSCOMPAL_GetLastError(-1,&m_bstrLastError);
		else
			m_bstrLastError = "Initialization failed for idl_pal_mscom.dll";
		ResetMSCOMPALPtrs();
		return FALSE;
	}

	m_bInitialized = TRUE;
	return TRUE;
}



//-----------------------------------------------------------------------------
void CBmlWRAPPER::ForwardPendingIDLOutput(void)
{
	int  cStrs  = (int)m_vPendingIDLOutput.size();
	if ( cStrs <= 0 )
		return;

	for ( int i = 0; i < cStrs; i++ )
	{
		BSTR bstr = m_vPendingIDLOutput[i];
		if ( bstr )
		{
			Fire_OnIDLOutput( bstr );
			::SysFreeString(  bstr );
		}
	}
	m_vPendingIDLOutput.clear();
}


//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::get_Cookie(IDL_MSCOMPAL_COOKIE_T* Cookie)
{
    VALIDATE_OBJECT;
    GET_COOKIE;
    return S_OK;
}


//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::GetIDLObjectVariableName(BSTR* Name)
{
    VALIDATE_OBJECT;
	return IDL_MSCOMPAL_GetObjVariableName(m_cookie, Name);
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::GetIDLObjectClassName(BSTR* Name)
{
    VALIDATE_OBJECT;
	return IDL_MSCOMPAL_GetObjClassName(m_cookie, Name);
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::GetProcessName(BSTR* Name)
{
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN;
	if ( !Name )
		return E_POINTER;
	return m_bstrProcName.CopyTo(Name);
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::SetProcessName(BSTR Name)
{
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN;

	// Once the object is created, don't allow the user to reset the name,
	// since it has no real effect, but it could give misleading information
	// if they "get" the process name that was "set" after the object was created.
	//
	if ( m_bObjCreated )
		return S_OK;

	if ( !Name || wcslen(Name) == 0 )
	{
		m_bstrProcName    = L"";
		m_OPSControlFlags = BML_CONTROL_INPROC;
	}
	else
	{
		m_bstrProcName    = Name;
		m_OPSControlFlags = BML_CONTROL_OUTPROC;
	}
	return S_OK;

}



//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::GetLastError(BSTR* LastError)
{
    GET_LAST_ERROR;
    return S_OK;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::CreateObject(long cargs, VARIANT argval, VARIANT argpal)
{
	HRESULT hr = CreateObjectEx(cargs,argval,argpal,0);
    return  hr;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::CreateObjectEx(long cargs, VARIANT argval, VARIANT argpal,
										 long idlInitFlags)
{
	VALIDATE_PAL_HRESULT;
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN;

	long hwndAttach = NULL;

#if ( defined(BMLWRAPPER_ISACTIVEXCTRL) && BMLWRAPPER_ISACTIVEXCTRL == 1)
	hwndAttach = (long)m_hWnd;
#endif

	// REGISTER_IDLNOTIFYCB & REGISTER_IDLOUTPUTCB are contained within this macro
	//
    CREATE_OBJECT;

    return hr;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::DestroyObject(void)
{
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN;
    UNREGISTER_IDLNOTIFYCB;
    UNREGISTER_IDLOUTPUTCB;
    DESTROY_OBJECT;
    return S_OK;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::Abort(void)
{
    VALIDATE_OBJECT;
	return IDL_MSCOMPAL_Abort(m_cookie);
}


//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::SetIDLVariable(BSTR bstrVar, VARIANT  Value)
{
    VALIDATE_OBJECT;
	return IDL_MSCOMPAL_SetIDLVariable(m_cookie, bstrVar, Value);

}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::GetIDLVariable(BSTR bstrVar, VARIANT* Value)
{
    VALIDATE_OBJECT;
	VALIDATE_ARGREF(Value);
	return IDL_MSCOMPAL_GetIDLVariable(m_cookie, bstrVar, Value);
}

//-----------------------------------------------------------------------------
STDMETHODIMP CBmlWRAPPER::ExecuteString(BSTR bstrCmd)
{
    VALIDATE_OBJECT;
	return IDL_MSCOMPAL_ExecuteString(m_cookie, bstrCmd);
}


#if ( defined(BMLWRAPPER_ISACTIVEXCTRL) && BMLWRAPPER_ISACTIVEXCTRL == 1)

//-------------------------------------------------------------------------------------------------
HRESULT CBmlWRAPPER::OnDraw(ATL_DRAWINFO& di)
{
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN;

    RECT& rc = *(RECT*)di.prcBounds;
    HDC& hdc = di.hdcDraw;

	COLORREF crFill   = RGB(255,255,255);
    BOOL bRuntimeMode = TRUE;

	// We aware that not all environments set the "User Mode" correctly or even at all.
	//
    if ( FAILED(GetAmbientUserMode(bRuntimeMode)) )  // UserMode == Runtime mode
        bRuntimeMode = TRUE;

    if ( !bRuntimeMode )
    {
        // If this is true, it means we've transitioned from Runtime mode back to Design mode
        if ( bRuntimeMode != m_bCurrRuntimeMode )
        {
            m_bCurrRuntimeMode = bRuntimeMode;
            if ( m_bObjCreated  )
            {
                DestroyObject();
				memset(&m_rcClient,0,sizeof(m_rcClient));
            }
        }

        if ( !m_hImage )
        {
            m_hImage = LoadImage(
                            g_hInst,
                            MAKEINTRESOURCE(IDL_BML_AXDESIGNSCREEN),
                            IMAGE_BITMAP,
                            0,0,
                            LR_DEFAULTCOLOR);
            if ( !m_hImage )
                return S_OK;
            if ( !GetObject( (HGDIOBJ)m_hImage,sizeof(BITMAP),&m_bmpInfo) )
                return S_OK;
        }

        FillRect(hdc, (RECT*)di.prcBounds, (HBRUSH)CreateSolidBrush(crFill));

        HDC hdcMem = CreateCompatibleDC(hdc);

        if ( hdcMem && m_hImage )
        {
            HGDIOBJ holdobj = SelectObject(hdcMem,m_hImage);

            long dcWidth  = rc.right  - rc.left;
            long dcHeight = rc.bottom - rc.top;

            float haspect = (float)m_bmpInfo.bmHeight / (float)m_bmpInfo.bmWidth;
            float vaspect = (float)m_bmpInfo.bmWidth  / (float)m_bmpInfo.bmHeight;

            int dstX, dstY, dstW, dstH;

            int nH = (int)((float)dcWidth * haspect);

            // Center it vertically if it will fit
            if ( nH <= dcHeight )
            {
                dstX = rc.left;
                dstY = rc.top + ( (dcHeight - nH) / 2 );
                dstW = dcWidth;
                dstH = nH;
            }
            else // Center it horizontally
            {
                dstH = dcHeight;
                dstW = (int)((float)dstH * vaspect);
                dstX = rc.left + ( (dcWidth - dstW) / 2 );
                dstY = rc.top;
            }

            StretchBlt( hdc,
                        dstX, dstY,
                        dstW, dstH,
                        hdcMem,
                        0, 0,
                        m_bmpInfo.bmWidth, m_bmpInfo.bmHeight,
                        SRCCOPY);

            SelectObject(hdcMem,holdobj);
            DeleteDC(hdcMem);
        }

    }
    else // Runtime mode
    {
        // If this is true, it means we've transitioned from Design Mode to Runtime Mode
        if ( bRuntimeMode != m_bCurrRuntimeMode )
        {
            m_bCurrRuntimeMode = bRuntimeMode;


#if ( !defined(AX_CLIENT_CALLS_CREATE_OBJECT) || AX_CLIENT_CALLS_CREATE_OBJECT != 1 )
            if ( !m_bObjCreated )
            {
                CComVariant vtNULL(0);
                if ( FAILED(CreateObject(0,vtNULL,vtNULL)) )
				{
					FillRect(hdc, (RECT*)di.prcBounds, (HBRUSH)CreateSolidBrush(crFill));
                    return S_OK;
				}
				::GetClientRect(m_hWnd,&m_rcClient);
            }
#endif
        }
        if ( !m_bObjCreated )
            FillRect(hdc, (RECT*)di.prcBounds, (HBRUSH)CreateSolidBrush(crFill));
        else
		{
			BmlEvent bmlevt;

			bmlevt.evtId = BMLEVENT_OnExpose;
			bmlevt.evt.expose.x = 0;
			bmlevt.evt.expose.y = 0;
			bmlevt.evt.expose.width  = rc.right-rc.left;
			bmlevt.evt.expose.height = rc.bottom-rc.top;

			HRESULT hr = IDL_MSCOMPAL_OnEvent(m_cookie, &bmlevt);
		}
    }

    return S_OK;
}


//-------------------------------------------------------------------------------------------------
LRESULT CBmlWRAPPER::OnEventIdl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN;

    if ( m_hWnd == m_currHwnd && uMsg == m_currMsg )
    {
        bHandled = TRUE;
        return 1;
    }

    long keyCode = 0;

    // If cookie is not set, it means we are receiving events for our window
    // before we've created and attached the underlying IDL object.
    //
    if ( m_cookie <= 0 )
    {
        m_currMsg  = 0;
        m_currHwnd = 0;
        bHandled = FALSE;
        return 0;
    }

    if ( uMsg == WM_PAINT )
    {
        CWindowImpl<CBmlWRAPPER>::DefWindowProc(uMsg,wParam,lParam);

        RECT rc;
        PAINTSTRUCT ps;

        HDC hdc = (wParam != NULL) ? (HDC)wParam : ::BeginPaint(m_hWndCD, &ps);
        if (hdc != NULL)
        {
            ::GetClientRect(m_hWndCD, &rc);

            ATL_DRAWINFO di;
            memset(&di, 0, sizeof(di));
            di.cbSize = sizeof(di);
            di.dwDrawAspect = DVASPECT_CONTENT;
            di.lindex = -1;
            di.hdcDraw = hdc;
            di.prcBounds = (LPCRECTL)&rc;

            OnDraw(di);
        }
        if (wParam == NULL)
            ::EndPaint(m_hWndCD, &ps);

        m_currMsg  = 0;
        m_currHwnd = 0;
        bHandled = TRUE;
        return 1;
    }

    m_currMsg  = uMsg;
    m_currHwnd = m_hWnd;

    // These get reflected back to the client
    long bForwardToIdl = 1;
    long mBtn    = 0;
    long kState  = 0;
    long xPos    = 0;
    long yPos    = 0;
	long isAscii = 0;
	POINT pt;

	BmlEvent bmlevt;
	memset(&bmlevt,0,sizeof(BmlEvent));

    switch( uMsg )
    {
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:

			if ( uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_LBUTTONDOWN )
				::SetCapture(m_hWnd);
			if ( uMsg == WM_MBUTTONUP   || uMsg == WM_RBUTTONUP   || uMsg == WM_LBUTTONUP )
				::ReleaseCapture();

            xPos = GET_X_LPARAM(lParam);
            yPos = GET_Y_LPARAM(lParam);

            if ( uMsg == WM_MOUSEMOVE )
			{
				// Save off the current mouse position for use later with OnKeyboard event
				m_lastMouseX = xPos;
				m_lastMouseY = yPos;
			}

			// IDL requires 'Y' to be relative to the bottom of the window
			yPos = m_rcClient.bottom - yPos - 1;
			if ( yPos < 0 )
				 yPos = 0;

            mBtn   |= (((wParam & MK_LBUTTON) == MK_LBUTTON) ? BMLEVENT_Mousebutton_Left   : 0);
            mBtn   |= (((wParam & MK_RBUTTON) == MK_RBUTTON) ? BMLEVENT_Mousebutton_Right  : 0);
            mBtn   |= (((wParam & MK_MBUTTON) == MK_MBUTTON) ? BMLEVENT_Mousebutton_Middle : 0);

            //  Test for Alt key press
            if ( GetKeyState(VK_MENU) < 0 )
                kState |= (long)BMLEVENT_Keymods_Alt;

            kState |= (((wParam & MK_SHIFT)   == MK_SHIFT)   ? (long)BMLEVENT_Keymods_Shift	   : 0);
            kState |= (((wParam & MK_CONTROL) == MK_CONTROL) ? (long)BMLEVENT_Keymods_Control  : 0);
            kState |= ((GetKeyState(VK_CAPITAL) & ~1)        ? (long)BMLEVENT_Keymods_Capslock : 0);

            if ( uMsg == WM_MOUSEMOVE )
			{
                bmlevt.evtId               = BMLEVENT_OnMouseMotion;
                bmlevt.evt.mMotion.x       = xPos;
                bmlevt.evt.mMotion.y       = yPos;		// Relative to bottom of image!
                bmlevt.evt.mMotion.keyMods = kState;
                Fire_OnMouseMove(mBtn,kState,xPos,yPos,&bForwardToIdl);
			}
            else if ( uMsg == WM_MOUSEWHEEL )
            {
                long wDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;

                bmlevt.evtId               = BMLEVENT_OnMouseWheel;
                bmlevt.evt.mWheel.x        = xPos;
                bmlevt.evt.mWheel.y        = yPos;
                bmlevt.evt.mWheel.delta    = wDelta;
                bmlevt.evt.mWheel.keyMods  = kState;
                Fire_OnMouseWheel(kState,wDelta,xPos,yPos,&bForwardToIdl);
            }
            else if ( uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN )
			{
                bmlevt.evtId               = BMLEVENT_OnMouseDown;
                bmlevt.evt.mDown.x         = xPos;
                bmlevt.evt.mDown.y         = yPos;
                bmlevt.evt.mDown.mButtons  = mBtn;
                bmlevt.evt.mDown.keyMods   = kState;
                bmlevt.evt.mDown.numClicks = 1;
				Fire_OnMouseDown(mBtn,kState,xPos,yPos,&bForwardToIdl);
			}
            else if ( uMsg == WM_LBUTTONUP )
			{
				bmlevt.evtId               = BMLEVENT_OnMouseUp;
				bmlevt.evt.mUp.x		   = xPos;
				bmlevt.evt.mUp.y           = yPos;
				bmlevt.evt.mUp.mButtons    = BMLEVENT_Mousebutton_Left;
                Fire_OnMouseUp(BMLEVENT_Mousebutton_Left,kState,xPos,yPos,&bForwardToIdl);
			}
            else if ( uMsg == WM_MBUTTONUP )
			{
				bmlevt.evtId               = BMLEVENT_OnMouseUp;
				bmlevt.evt.mUp.x		   = xPos;
				bmlevt.evt.mUp.y           = yPos;
				bmlevt.evt.mUp.mButtons    = BMLEVENT_Mousebutton_Middle;
                Fire_OnMouseUp(BMLEVENT_Mousebutton_Middle,kState,xPos,yPos,&bForwardToIdl);
			}
            else if ( uMsg == WM_RBUTTONUP )
			{
				bmlevt.evtId               = BMLEVENT_OnMouseUp;
				bmlevt.evt.mUp.x		   = xPos;
				bmlevt.evt.mUp.y           = yPos;
				bmlevt.evt.mUp.mButtons    = BMLEVENT_Mousebutton_Right;
                Fire_OnMouseUp(BMLEVENT_Mousebutton_Right,kState,xPos,yPos,&bForwardToIdl);
			}
            else if ( uMsg == WM_LBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK )
			{
                bmlevt.evtId               = BMLEVENT_OnMouseDown;
                bmlevt.evt.mDown.x         = xPos;
                bmlevt.evt.mDown.y         = yPos;
                bmlevt.evt.mDown.mButtons  = mBtn;
                bmlevt.evt.mDown.keyMods   = kState;
                bmlevt.evt.mDown.numClicks = 2;
                Fire_OnMouseDoubleClick(mBtn,kState,xPos,yPos,&bForwardToIdl);
			}

            break;

        case WM_KEYUP:
        case WM_KEYDOWN:

			isAscii = ProcessKeyEvent(wParam,lParam,keyCode,kState);

			if ( isAscii || keyCode )
			{
				if ( uMsg == WM_KEYUP )
					Fire_OnKeyUp(keyCode,kState,&bForwardToIdl);
				else
					Fire_OnKeyDown(keyCode,kState,&bForwardToIdl);

				if ( bForwardToIdl )
				{
					long yPos = m_rcClient.bottom - m_lastMouseY - 1;
					if ( yPos < 0 )
						 yPos = 0;
					bmlevt.evtId                    = BMLEVENT_OnKeyboard;
					bmlevt.evt.keyboard.isASCII     = isAscii;
					bmlevt.evt.keyboard.charCode    = isAscii ? keyCode : 0;
					bmlevt.evt.keyboard.keyValue    = isAscii ? 0 : keyCode;
					bmlevt.evt.keyboard.x           = m_lastMouseX;
					bmlevt.evt.keyboard.y           = yPos;
					bmlevt.evt.keyboard.press       = uMsg == WM_KEYDOWN;
					bmlevt.evt.keyboard.release     = uMsg == WM_KEYUP;
					bmlevt.evt.keyboard.keyMods     = kState;
				}
			}
			else
				bForwardToIdl = FALSE;
			break;

        // For some reason, ActiveX controls, as implemented by ATL, never seem
        // to get a WM_SIZE message.  Instead, they get this message *AFTER* the
        // control has resized itself, i.e. after SetWindowPos() has been called
        // on the control.
        //
        case WM_WINDOWPOSCHANGED:
        {
            WINDOWPOS* pPos = (WINDOWPOS*)lParam;

			m_rcClient.right  = pPos->cx;
			m_rcClient.bottom = pPos->cy;

			bmlevt.evtId               = BMLEVENT_OnResize;
			bmlevt.evt.resize.width    = pPos->cx;
			bmlevt.evt.resize.height   = pPos->cy;

            Fire_OnSize(pPos->cx,pPos->cy,&bForwardToIdl);
            break;
        }

        // These two MESSAGES are used to track mouse Enter/Exit
        case WM_SETCURSOR:
            if ( LOWORD(lParam) == HTCLIENT && !m_bCursorIn )
            {
                TRACKMOUSEEVENT tme;
                memset(&tme,0,sizeof(tme));
                tme.cbSize = sizeof(tme);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = m_hWnd;

                // This call says we want to receive a WM_MOUSELEAVE message
                // when the mouse leaves our client area.
                // When the WM_MOUSELEAVE message is sent, it cancels the tracking,
                // so we must make this call again upon re-entering our client area.
                //
                if ( TrackMouseEvent( &tme ) )
                {
                    m_bCursorIn = TRUE;
	                bmlevt.evtId = BMLEVENT_OnEnter;
                    Fire_OnMouseEnter();
                }
            }
            break;
        case WM_MOUSELEAVE:
			if ( GetCursorPos(&pt) && ::ScreenToClient(m_hWnd,&pt) )
			{
				m_lastMouseX = pt.x;
				m_lastMouseY = pt.y;
			}
            if ( m_bCursorIn )
            {
                Fire_OnMouseExit();
                m_bCursorIn = FALSE;
                bmlevt.evtId = BMLEVENT_OnExit;
                bForwardToIdl = 1;
            }
            break;

        default:
            break;
    }
    if ( bForwardToIdl && bmlevt.evtId )
    {
        HRESULT hr = IDL_MSCOMPAL_OnEvent(m_cookie, &bmlevt);
#if 1
        if ( uMsg == WM_WINDOWPOSCHANGED )
            ::PostMessage(m_hWnd,WM_PAINT,0,0);
#endif

    }

    m_currMsg  = 0;
    m_currHwnd = 0;
    bHandled = TRUE;
    return 1;
}


//-------------------------------------------------------------------------------------------------
int CBmlWRAPPER::ProcessKeyEvent(WPARAM wParam, LPARAM lParam,
								 long& keyCode, long& kState)
{

	keyCode = 0;
	kState  = 0;

	BYTE KeyState[256];
	WORD buffer;

	if ( !GetKeyboardState(KeyState) )
		return 0;
	UINT bMenuActive = GetKeyState(VK_MENU) & ~1;

	kState |= ((GetKeyState(VK_SHIFT)   & ~1) ? (long)BMLEVENT_Keymods_Shift    : 0);
	kState |= ((GetKeyState(VK_CONTROL) & ~1) ? (long)BMLEVENT_Keymods_Control  : 0);
	kState |= ((GetKeyState(VK_CAPITAL) & ~1) ? (long)BMLEVENT_Keymods_Capslock : 0);
	kState |= ( bMenuActive                   ? (long)BMLEVENT_Keymods_Alt      : 0);

	UINT scanCode    = (UINT)(lParam & (0xF << 15));
	UINT uVirtKey    = (UINT)wParam;

	BOOL bIsAscii = ToAscii(uVirtKey,scanCode,KeyState,&buffer,bMenuActive) == 1;

	if ( bIsAscii )
	{
		keyCode = buffer & 0xFF;
		return bIsAscii;
	}
	switch (wParam)
	{
		case VK_SHIFT:		keyCode = KEYBOARD_EVENT_SHIFT;			break;
		case VK_CONTROL:	keyCode = KEYBOARD_EVENT_CONTROL;		break;
		case VK_CAPITAL:	keyCode = KEYBOARD_EVENT_CAPSLOCK;		break;
		case VK_MENU:		keyCode = KEYBOARD_EVENT_ALT;			break;
		case VK_LEFT:		keyCode = KEYBOARD_EVENT_LEFT;			break;
		case VK_RIGHT:		keyCode = KEYBOARD_EVENT_RIGHT;			break;
		case VK_UP:			keyCode = KEYBOARD_EVENT_UP;			break;
		case VK_DOWN:		keyCode = KEYBOARD_EVENT_DOWN;			break;
		case VK_PRIOR:		keyCode = KEYBOARD_EVENT_PAGE_UP;		break;
		case VK_NEXT:		keyCode = KEYBOARD_EVENT_PAGE_DOWN;		break;
		case VK_HOME:		keyCode = KEYBOARD_EVENT_HOME;			break;
		case VK_END:		keyCode = KEYBOARD_EVENT_END;			break;
		case VK_DELETE:		keyCode = KEYBOARD_EVENT_DEL;
							bIsAscii = TRUE;
							break;
		default:
			break;
	}
	return bIsAscii;
}


#endif // (BMLWRAPPER_ISACTIVEXCTRL == 1)



