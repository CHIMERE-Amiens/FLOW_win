#pragma warning(disable: 4312 4311)


//---------------------------------------------------------------------------------------
#define class_IdlBmlWrapper \
class ATL_NO_VTABLE CBmlWRAPPER : \
    public CComObjectRootEx<CComSingleThreadModel>,\
    public CComCoClass<CBmlWRAPPER, &CLSID_BmlWRAPPER>,\
    public ISupportErrorInfo,\
    public IConnectionPointContainerImpl<CBmlWRAPPER>,\
    public CProxy_IdlBmlWrapperEvents<CBmlWRAPPER>,\
    public IProvideClassInfo2Impl<&CLSID_BmlWRAPPER, &__uuidof(_DIBmlWRAPPEREVENTS), &LIBID_BmlWRAPPER>,\
    public IDispatchImpl<IBmlWRAPPER, &IID_BmlWRAPPER, &LIBID_BmlWRAPPER, /*wMajor =*/ 1, /*wMinor =*/ 0>


//---------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STANDARD_MEMBERS \
    IDL_MSCOMPAL_COOKIE_T           m_cookie;\
    CComBSTR                        m_bstrIdlObjIdentifier;\
    CComBSTR                        m_bstrProcName;\
    IDL_MSCOMPAL_OPSCONTROL_FLAGS_T m_OPSControlFlags;\
    BOOL                            m_bObjCreated;\
    CComBSTR                        m_bstrLastError;\
    vector<BSTR>                    m_vPendingIDLOutput;\
public:\
    void                            ForwardPendingIDLOutput(void);


//---------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STANDARD_CONSTRUCTORS \
    CBmlWRAPPER();\
    ~CBmlWRAPPER();\
    HRESULT FinalConstruct();\
    void    FinalRelease();\
    DECLARE_PROTECT_FINAL_CONSTRUCT()


//---------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STANDARD_CUSTOM_UPDATEREGISTRY \
    /*DECLARE_REGISTRY_RESOURCEID(IDR_BmlWRAPPER)*/\
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()\
    {\
        _ATL_REGMAP_ENTRY regMapEntries [] =\
        { \
            { L"APPIDGUID",         GUID_APPIDBmlWRAPPERSTR }, \
            { L"TYPELIBGUID",       GUID_LIBBmlWRAPPERSTR },\
            { L"COCLASSGUID",       GUID_COCLASSBmlWRAPPERSTR },\
            { L"PROGID",            BMLWRAPPER_PROGIDSTR }, \
            { L"VERINDPROGID",      BMLWRAPPER_VERINDPROGIDSTR } ,\
            { L"OBJNAME",           BMLWRAPPER_PROJNAMESTR }, \
            { NULL, NULL },         /* Place holder for OLEMISC, if defined */\
            { NULL, NULL } \
        }; \
        /* This is only defined for ActiveX controls */\
        __if_exists(_GetMiscStatus) \
        { \
            int ndxLastEntry = (sizeof(regMapEntries)/sizeof(_ATL_REGMAP_ENTRY))-1;\
            int ndxOleMisc = ndxLastEntry-1;\
            regMapEntries[ndxOleMisc].szKey = L"OLEMISC"; \
            TCHAR szOleMisc[10]; \
            wsprintf(szOleMisc, _T("%d"), _GetMiscStatus()); \
            USES_CONVERSION; \
            regMapEntries[ndxOleMisc].szData = T2OLE(szOleMisc); \
        }\
        return ATL::_pAtlModule->UpdateRegistryFromResource(IDR_BmlWRAPPER, bRegister, regMapEntries); \
    }


//---------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STANDARD_INTERFACEMAP \
BEGIN_COM_MAP(CBmlWRAPPER)\
    COM_INTERFACE_ENTRY(IBmlWRAPPER)\
    COM_INTERFACE_ENTRY(IDispatch)\
    COM_INTERFACE_ENTRY(ISupportErrorInfo)\
    COM_INTERFACE_ENTRY(IConnectionPointContainer)\
END_COM_MAP()

#define DECLARE_WRAPPER_STANDARD_CONNECTIONPTMAP \
BEGIN_CONNECTION_POINT_MAP(CBmlWRAPPER)\
    CONNECTION_POINT_ENTRY(DIID_IBmlWRAPPEREVENTS)\
END_CONNECTION_POINT_MAP()


//---------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STANDARD_ISUPPORTERRORINFO \
    /* ISupportsErrorInfo */ \
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)\
    {\
        static const IID* arr[] = \
        {\
            &IID_BmlWRAPPER,\
        };\
        for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)\
        {\
            if (InlineIsEqualGUID(*arr[i], riid))\
                return S_OK;\
        }\
        return S_FALSE;\
    }


//---------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STANDARD_DEFAULTIMPL \
public: \
    DECLARE_WRAPPER_STANDARD_CONSTRUCTORS\
    DECLARE_WRAPPER_STANDARD_CUSTOM_UPDATEREGISTRY\
    DECLARE_WRAPPER_STANDARD_INTERFACEMAP\
    DECLARE_WRAPPER_STANDARD_CONNECTIONPTMAP\
    DECLARE_WRAPPER_STANDARD_ISUPPORTERRORINFO


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STATIC_IDLNOTIFYCB \
    static void WRAPPER_STATICIDLNOTIFYCB(BSTR bstr1, BSTR bstr2, void* UserData); \
    void HandleIDLNotifyCB(BSTR bstr1, BSTR bstr2);


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_STATIC_IDLOUTPUTCB \
    static void WRAPPER_STATICIDLOUTPUTCB(BSTR bstrOutput, void* UserData); \
    void HandleIDLOutputCB(BSTR bstrOutput);

