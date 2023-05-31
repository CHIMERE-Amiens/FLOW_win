#pragma once
#include "resource.h"

#include <vector>
using namespace std;

#include "BmlWrapperSrvr.h"
#include "wrapper_guids.h"
#include "mscompal_wrapper_cp_impl.h"
#include "mscompal_wrapper_defines.h"


//-------------------------------------------------------------------------------------------------
#define IDL_PAL_CDECL   __cdecl

typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_Initialize_T)                  (void);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_Release_T)                     (void);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_RegisterWrapper_T)             (BSTR bstrObjIdentifier, IDL_MSCOMPAL_OPSCONTROL_FLAGS_T OPSControlFlags,
                                                                             BSTR bstrProcName,      IDL_MSCOMPAL_COOKIE_T* pcookie);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_ReleaseWrapper_T)              (IDL_MSCOMPAL_COOKIE_T cookie);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_CreateObject_T)                (IDL_MSCOMPAL_COOKIE_T cookie,
																			 int argc, VARIANT argv[], int argpal[],
                                                                             IDL_MSCOMPAL_IDL_INIT_DATA_T idlInitData,
																			 long WindowId, char* szWindowKeywords);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_DestroyObject_T)               (IDL_MSCOMPAL_COOKIE_T cookie);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_OnEvent_T)                     (IDL_MSCOMPAL_COOKIE_T cookie, BmlEvent* pEvt);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_CallMethod_T)                  (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrMethod,
                                                                             int argc, VARIANT argv[], int argpal[], VARIANT* varResults);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_SetProperty_T)                 (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrProp, int argc, VARIANT argv[], int argpal[]);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_GetProperty_T)                 (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrProp, VARIANT* varValue);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_Abort_T)                       (IDL_MSCOMPAL_COOKIE_T cookie);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_GetLastError_T)                (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrLastErr);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_RegisterOutputCallback_T)      (IDL_MSCOMPAL_COOKIE_T cookie, PAL_OUTPUT_CB cbFunc, void* UserData);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_UnregisterOutputCallback_T)    (IDL_MSCOMPAL_COOKIE_T cookie);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_RegisterNotifyCallback_T)      (IDL_MSCOMPAL_COOKIE_T cookie, PAL_NOTIFY_CB cbFunc, void* UserData);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_UnregisterNotifyCallback_T)    (IDL_MSCOMPAL_COOKIE_T cookie);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_SetIDLVariable_T)              (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrVar, VARIANT  Value);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_GetIDLVariable_T)              (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrVar, VARIANT* Value);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_ExecuteString_T)               (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrCmd);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_GetObjVariableName_T)          (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrName);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_GetObjClassName_T)             (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrName);
typedef HRESULT     (IDL_PAL_CDECL *MSCOMPAL_GetProcNameFromCookie_T)       (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrName);



#define GETPROC(type,funcName) \
	funcName = (type)::GetProcAddress(m_hPALModule, #funcName );\
	if ( funcName == NULL ) {\
		ResetMSCOMPALPtrs(); \
		char szErr[512];\
		sprintf(szErr,"GetProcAddress(idl_pal_mscom.dll) failed for: %s",#funcName);\
		m_bstrLastError = szErr;\
		return FALSE;\
	}




#if ( defined(BMLWRAPPER_ISACTIVEXCTRL) && BMLWRAPPER_ISACTIVEXCTRL == 1)

#include <atlctl.h>
#include "mscompal_wrapper_activex_defines.h"


//---------------------------------------------------------------------------------
class_IdlBmlActiveXWrapper
{
public:
    DECLARE_WRAPPER_AX_DEFAULTIMPL



#else

//---------------------------------------------------------------------------------
class_IdlBmlWrapper
{
public:
    DECLARE_WRAPPER_STANDARD_DEFAULTIMPL

#endif

    DECLARE_WRAPPER_STATIC_IDLNOTIFYCB;
    DECLARE_WRAPPER_STATIC_IDLOUTPUTCB;

public:
    STDMETHOD(get_Cookie)               (IDL_MSCOMPAL_COOKIE_T* Cookie);

    STDMETHOD(SetProcessName)           (BSTR  Name);
    STDMETHOD(GetProcessName)           (BSTR* Name);

    STDMETHOD(GetIDLObjectVariableName) (BSTR* Name);
    STDMETHOD(GetIDLObjectClassName)    (BSTR* Name);

    STDMETHOD(SetIDLVariable)           (BSTR bstrVar, VARIANT  Value);
    STDMETHOD(GetIDLVariable)           (BSTR bstrVar, VARIANT* Value);
    STDMETHOD(ExecuteString)            (BSTR bstrCmd);

    STDMETHOD(CreateObject)             (long argc, VARIANT argval, VARIANT argpal);
    STDMETHOD(CreateObjectEx)           (long argc, VARIANT argval, VARIANT argpal, long idlInitFlags);
    STDMETHOD(DestroyObject)            (void);

    STDMETHOD(GetLastError)             (BSTR* LastError);
    STDMETHOD(Abort)                    (void);

private:
    void      ResetMSCOMPALPtrs         (void);
    BOOL      InternalInitialize        (void);
    BOOL      m_bInitialized;
	HMODULE   m_hPALModule;

    MSCOMPAL_Initialize_T               IDL_MSCOMPAL_Initialize;
    MSCOMPAL_Release_T                  IDL_MSCOMPAL_Release;
    MSCOMPAL_RegisterWrapper_T          IDL_MSCOMPAL_RegisterWrapper;

    MSCOMPAL_ReleaseWrapper_T           IDL_MSCOMPAL_ReleaseWrapper;
    MSCOMPAL_CreateObject_T             IDL_MSCOMPAL_CreateObject;
    MSCOMPAL_DestroyObject_T            IDL_MSCOMPAL_DestroyObject;
    MSCOMPAL_OnEvent_T                  IDL_MSCOMPAL_OnEvent;
    MSCOMPAL_CallMethod_T               IDL_MSCOMPAL_CallMethod;

    MSCOMPAL_SetProperty_T              IDL_MSCOMPAL_SetProperty;
    MSCOMPAL_GetProperty_T              IDL_MSCOMPAL_GetProperty;
    MSCOMPAL_Abort_T                    IDL_MSCOMPAL_Abort;
    MSCOMPAL_GetLastError_T             IDL_MSCOMPAL_GetLastError;
    MSCOMPAL_RegisterOutputCallback_T   IDL_MSCOMPAL_RegisterOutputCallback;
    MSCOMPAL_UnregisterOutputCallback_T IDL_MSCOMPAL_UnregisterOutputCallback;
    MSCOMPAL_RegisterNotifyCallback_T   IDL_MSCOMPAL_RegisterNotifyCallback;
    MSCOMPAL_UnregisterNotifyCallback_T IDL_MSCOMPAL_UnregisterNotifyCallback;
    MSCOMPAL_SetIDLVariable_T           IDL_MSCOMPAL_SetIDLVariable;
    MSCOMPAL_GetIDLVariable_T           IDL_MSCOMPAL_GetIDLVariable;
    MSCOMPAL_ExecuteString_T            IDL_MSCOMPAL_ExecuteString;
    MSCOMPAL_GetObjVariableName_T       IDL_MSCOMPAL_GetObjVariableName;
    MSCOMPAL_GetObjClassName_T          IDL_MSCOMPAL_GetObjClassName;
    MSCOMPAL_GetProcNameFromCookie_T    IDL_MSCOMPAL_GetProcNameFromCookie;



// Object-specific method/properties declarations come from this file..
#include "Wrapper.h"



#if ( defined(BMLWRAPPER_ISACTIVEXCTRL) && BMLWRAPPER_ISACTIVEXCTRL == 1)
    DECLARE_WRAPPER_AX_DRAW_AND_EVENT_HANDLERS;
    DECLARE_WRAPPER_AX_MEMBERS;
#endif

private:
    DECLARE_WRAPPER_STANDARD_MEMBERS;

};

DECLARE_CBmlWRAPPER_OBJECT_ENTRY_AUTO;





//---------------------------------------------------------------------------------
class CForwardPendingIDLOutput
{
public:
	 CForwardPendingIDLOutput(CBmlWRAPPER* pWrapper){m_pWrapper = pWrapper;}
	 ~CForwardPendingIDLOutput(){if (m_pWrapper) m_pWrapper->ForwardPendingIDLOutput();}
private:
	CBmlWRAPPER* m_pWrapper;
};


