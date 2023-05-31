// mscompal_wrapper_impl_defines.h

#pragma warning(disable: 4312 4311)

#include "mscompal_wrapper_helper.h"

//---------------------------------------------------------------------------------------
// Defines used in Method/Property implementation code

char* const SZ_ERROR_ARGVMEMALLOCFAILED = "Failed to allocate memory for processing input arguments.";
char* const SZ_ERROR_CREATEOBJNOTCALLED = "CreateObject() has not been called.";
char* const SZ_ERROR_BADVARTYPE         = "Bad variable type";


#define MAX_ERR_SIZE	1024


#define RETURN_ERROR(hr,szErr) {\
        m_bstrLastError = szErr;\
        return hr;\
    }

#define CHECK_PENDING_IDLOUTPUT_UPON_RETURN	\
	CForwardPendingIDLOutput checkPendingIDLOutput(this);


#define VALIDATE_PAL \
	if ( !m_bInitialized && !InternalInitialize() ){\
		return FALSE;\
	}

#define VALIDATE_PAL_HRESULT \
	if ( !m_bInitialized && !InternalInitialize() ){\
		return E_FAIL;\
	}


#define VALIDATE_OBJECT \
	VALIDATE_PAL;\
	CHECK_PENDING_IDLOUTPUT_UPON_RETURN\
    if ( !m_bObjCreated )\
        RETURN_ERROR(E_UNEXPECTED,SZ_ERROR_CREATEOBJNOTCALLED);

#define VALIDATE_ARGREF(ref)\
    if ( !ref ) return E_POINTER;

#define BEGIN_PARMS(cParms)\
    int         argc = cParms;\
    CComVariant argv[cParms];\
    int         argp[cParms];\
	int			_eFlags = 0;

#define ADDPARM(ndx,expr,flags)\
    argv[ndx] = CComVariant(expr);\
    argp[ndx] = flags;

#define ADDPARMREF(ndx,expr,flags)\
    argv[ndx] = CComVariant(*(expr));\
    argp[ndx] = flags;

#define ADDPARM_VT(ndx,expr,vt,flags)\
    argv[ndx] = CComVariant(expr,vt);\
    argp[ndx] = flags;

#define ADDPARM_VTREF(ndx,expr,vt,flags)\
    argv[ndx] = CComVariant(*(expr),vt);\
    argp[ndx] = flags;

#define ADDPARM_VAR(ndx,var,flags)\
	/*_eFlags   = 0;*/\
	/*if ( IsVariantArray(&var) && !IsArrayIdlSameMajority(&var) )*/\
	/*	_eFlags = BML_PARMFLAG_CONVMAJORITY;*/\
    argv[ndx] = var;\
    argp[ndx] = flags; /* | _eFlags;*/

#define ADDPARM_VARREF(ndx,pVar,flags)\
    argv[ndx].vt = VT_VARIANT | VT_BYREF;\
    argv[ndx].pvarVal = pVar;\
	/*_eFlags = 0;*/\
	/*if ( IsVariantArray(pVar) && !IsArrayIdlSameMajority(pVar) )*/\
	/* 	_eFlags = BML_PARMFLAG_CONVMAJORITY;*/\
    argp[ndx] = flags; /*| _eFlags;*/

#define END_PARMS \
	for ( int i = 0; i < argc; i++ ) {\
		if ( argv[i].vt == VT_ERROR )\
			RETURN_ERROR(DISP_E_BADVARTYPE,SZ_ERROR_BADVARTYPE);\
	}

#define DECLARE_EMPTY_PARMS\
    int      argc = 0;\
    VARIANT* argv = NULL;\
    int*     argp = NULL;
	/*long  _eFlags = 0;*/

#define DECLARE_EMPTY_PARMS_CARGS(cArgs)\
    int      argc = ( cArgs <= 0 ? 0 : cArgs);\
    VARIANT* argv = NULL;\
    int*     argp = NULL;
	/*long  _eFlags = 0;*/


#define PROCESS_VARNUM_ARGS(_varargs,_argpal)\
    if ( argc > 0 )\
    {\
        argv = new VARIANT[argc];\
        argp = new int[argc];\
        if ( !argv || !argp )\
            RETURN_ERROR(E_OUTOFMEMORY,SZ_ERROR_ARGVMEMALLOCFAILED);\
        for ( int i = 0; i < argc; i++ )\
        {\
            ADDPARM_VAR(i,_varargs[i],_argpal[i]);\
        }\
    }

#define CLEANUP_PARMS\
    if ( argv ) delete[] argv;\
    if ( argp ) delete[] argp;


#define DECLARE_FUNCTION_RESULTS\
    CComVariant varResults;

#define CALL_METHOD(szMethod)\
	static CComBSTR bstrMethod = szMethod;\
    HRESULT hr = IDL_MSCOMPAL_CallMethod(m_cookie,bstrMethod,argc,argv,argp,NULL);\
    if ( FAILED(hr) )\
        return hr;

#define CALL_FUNCTION(szMethod)\
	static CComBSTR bstrMethod = szMethod;\
    HRESULT hr = IDL_MSCOMPAL_CallMethod(m_cookie,bstrMethod,argc,argv,argp,&varResults);\
    if ( FAILED(hr) )\
        return hr;

#define PROCESS_FUNCTION_RESULTS_VAR(pFuncRes)\
        return varResults.Detach(pFuncRes);

#define PROCESS_FUNCTION_RESULTS_NATIVE(pFuncRes)\
    if ( !Variant2Native(&varResults,(void*)pFuncRes) )\
        return E_FAIL;

#define PROCESS_OUT_VAR(ndx,pParm)\
        return varResults.Detach(pParm);

#define PROCESS_OUT_NATIVE(ndx,pParm)\
    if ( !Variant2Native(&argv[ndx],(void*)pParm) )\
        return E_FAIL;


#define GET_PROPERTY(szProp,Value)\
	static CComBSTR bstrProp = szProp;\
    HRESULT hr = IDL_MSCOMPAL_GetProperty(m_cookie,bstrProp,Value);\
    if ( FAILED(hr) )\
        return hr;

#define SET_PROPERTY(szProp)\
	static CComBSTR bstrProp = szProp;\
    HRESULT hr = IDL_MSCOMPAL_SetProperty(m_cookie,bstrProp,argc,argv,argp);\
    if ( FAILED(hr) )\
        return hr;



#define REGISTER_WRAPPER \
    HRESULT hr = IDL_MSCOMPAL_RegisterWrapper(m_bstrIdlObjIdentifier,m_OPSControlFlags,m_bstrProcName,&m_cookie);\
    if ( FAILED(hr) )\
        return hr;\
    if ( !m_cookie )\
        return E_UNEXPECTED;

#define CREATE_OBJECT \
    DECLARE_EMPTY_PARMS_CARGS(cargs);\
	SAFEARRAY* psaVals = NULL;\
	SAFEARRAY* psaPals = NULL;\
	if ( argc > 0 ) {\
		BOOL bvarArgval = ((int)(argval.vt & (VT_ARRAY | VT_VARIANT)) == (int)(VT_ARRAY | VT_VARIANT));\
		BOOL blngArgpal = ((int)(argpal.vt & (VT_ARRAY | VT_I4))      == (int)(VT_ARRAY | VT_I4));\
		psaVals = argval.parray;\
		psaPals = argpal.parray;\
		if ( !bvarArgval || !blngArgpal ||\
			 !psaVals || !psaPals || !psaVals->pvData || !psaPals->pvData ||\
			  psaVals->cDims != 1 || psaPals->cDims != 1 ||\
			  psaVals->rgsabound[0].cElements != argc || psaPals->rgsabound[0].cElements != argc ) {\
			RETURN_ERROR(E_INVALIDARG,\
				"Error CreateObject(): Invalid arguments: 'argval' Must be a 1-D array of VARIANT types; 'argpal' Must be a 1-D array of 32-bit values.");\
			  }\
		try {\
			argv = new VARIANT[argc];\
			argp = new int[argc];\
		} catch(...) {}\
        if ( !argv || !argp )\
            RETURN_ERROR(E_OUTOFMEMORY,SZ_ERROR_ARGVMEMALLOCFAILED);\
		VARIANT* pvarVals = (VARIANT*)psaVals->pvData;\
		DWORD*   pvarPals =   (DWORD*)psaPals->pvData;\
        for ( int i = 0; i < argc; i++ ) {\
			int argPal  = pvarPals[i];\
            ADDPARM_VAR(i,pvarVals[i],argPal);\
			if ( pvarVals[i].vt == VT_ERROR )\
				RETURN_ERROR(DISP_E_BADVARTYPE,SZ_ERROR_BADVARTYPE);\
        }\
	}\
    HRESULT hr = IDL_MSCOMPAL_RegisterWrapper(m_bstrIdlObjIdentifier,m_OPSControlFlags,m_bstrProcName,&m_cookie);\
	if ( FAILED(hr) ) {\
        CLEANUP_PARMS;\
        return hr;\
	}\
	if ( !m_cookie ) {\
        CLEANUP_PARMS;\
        return E_UNEXPECTED;\
	}\
    REGISTER_IDLNOTIFYCB;\
	REGISTER_IDLOUTPUTCB;\
	hr = IDL_MSCOMPAL_CreateObject(m_cookie,argc,argv,argp,(IDL_MSCOMPAL_IDL_INIT_DATA_T)idlInitFlags,hwndAttach,NULL);\
    if ( FAILED(hr) ) {\
        IDL_MSCOMPAL_ReleaseWrapper(m_cookie);\
		UNREGISTER_IDLNOTIFYCB;\
		UNREGISTER_IDLOUTPUTCB;\
        CLEANUP_PARMS;\
        return hr;\
    }\
    else\
        m_bObjCreated = TRUE;\
    CLEANUP_PARMS;

#define DESTROY_OBJECT \
    if ( m_bObjCreated )\
    {\
        HRESULT hr = IDL_MSCOMPAL_DestroyObject(m_cookie);\
        if ( FAILED(hr) ){\
            return hr;\
        }\
        hr = IDL_MSCOMPAL_ReleaseWrapper(m_cookie);\
        m_bObjCreated = FALSE;\
    }

#define GET_LAST_ERROR \
    if ( !LastError ) \
        return E_POINTER; \
    if ( m_cookie > 0 && IDL_MSCOMPAL_GetLastError ) \
		 IDL_MSCOMPAL_GetLastError(m_cookie,&m_bstrLastError);\
    m_bstrLastError.CopyTo(LastError);\

#define GET_COOKIE \
    if ( !Cookie)\
        return E_POINTER;\
    *Cookie = m_cookie;


//---------------------------------------------------------------------------------------------
#define CBmlWRAPPER_IMPLEMENT_IDLNOTIFYCB \
void CBmlWRAPPER::WRAPPER_STATICIDLNOTIFYCB(BSTR bstr1, BSTR bstr2, void* UserData)\
{\
    if ( !UserData )\
        return;\
    CBmlWRAPPER* pClass = (CBmlWRAPPER*)UserData;\
    pClass->HandleIDLNotifyCB(bstr1,bstr2);\
}\
void CBmlWRAPPER::HandleIDLNotifyCB(BSTR bstr1, BSTR bstr2)\
{\
    Fire_OnIDLNotify(bstr1, bstr2);\
}


//---------------------------------------------------------------------------------------------
#define REGISTER_IDLNOTIFYCB \
    IDL_MSCOMPAL_RegisterNotifyCallback(m_cookie,WRAPPER_STATICIDLNOTIFYCB,(void*)this);

#define UNREGISTER_IDLNOTIFYCB \
    if ( m_cookie > 0 ) \
        IDL_MSCOMPAL_UnregisterNotifyCallback(m_cookie);


//---------------------------------------------------------------------------------------------
#define CBmlWRAPPER_IMPLEMENT_IDLOUTPUTCB \
void CBmlWRAPPER::WRAPPER_STATICIDLOUTPUTCB(BSTR bstrOutput, void* UserData)\
{\
    if ( !UserData )\
        return;\
    CBmlWRAPPER* pClass = (CBmlWRAPPER*)UserData;\
    pClass->HandleIDLOutputCB(bstrOutput);\
}\
void CBmlWRAPPER::HandleIDLOutputCB(BSTR bstrOutput)\
{\
	if ( m_vec.GetSize() )\
	{\
		BSTR bstr = ::SysAllocString(bstrOutput);\
		if ( bstr )\
			m_vPendingIDLOutput.push_back(bstr);\
	}\
}


//---------------------------------------------------------------------------------------------
#define REGISTER_IDLOUTPUTCB \
    IDL_MSCOMPAL_RegisterOutputCallback(m_cookie,WRAPPER_STATICIDLOUTPUTCB,(void*)this);

#define UNREGISTER_IDLOUTPUTCB \
    if ( m_cookie > 0 ) \
        IDL_MSCOMPAL_UnregisterOutputCallback(m_cookie);





