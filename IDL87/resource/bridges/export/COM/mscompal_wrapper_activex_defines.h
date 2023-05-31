#pragma warning(disable: 4312 4311)

#include "mscompal_wrapper_events_defines.h"


//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
#define class_IdlBmlActiveXWrapper \
class ATL_NO_VTABLE CBmlWRAPPER : \
    public CComObjectRootEx<CComSingleThreadModel>,\
    public IDispatchImpl<IBmlWRAPPER, &IID_BmlWRAPPER, &LIBID_BmlWRAPPER, /*wMajor =*/ 1, /*wMinor =*/ 0>,\
    public IPersistStreamInitImpl<CBmlWRAPPER>,\
    public IOleControlImpl<CBmlWRAPPER>,\
    public IOleObjectImpl<CBmlWRAPPER>,\
    public IOleInPlaceActiveObjectImpl<CBmlWRAPPER>,\
    public IViewObjectExImpl<CBmlWRAPPER>,\
    public IOleInPlaceObjectWindowlessImpl<CBmlWRAPPER>,\
    public ISupportErrorInfo,\
    public IConnectionPointContainerImpl<CBmlWRAPPER>,\
    public CProxy_IdlBmlWrapperEvents<CBmlWRAPPER>, \
    public IPersistStorageImpl<CBmlWRAPPER>,\
    public ISpecifyPropertyPagesImpl<CBmlWRAPPER>,\
    public IQuickActivateImpl<CBmlWRAPPER>,\
    public IDataObjectImpl<CBmlWRAPPER>,\
    public IProvideClassInfo2Impl<&CLSID_BmlWRAPPER, &__uuidof(_DIBmlWRAPPEREVENTS), &LIBID_BmlWRAPPER>,\
    public CComCoClass<CBmlWRAPPER, &CLSID_BmlWRAPPER>,\
    public CComControl<CBmlWRAPPER>



//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_OLEMISC_STATUS \
DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | \
    OLEMISC_CANTLINKINSIDE | \
    OLEMISC_INSIDEOUT | \
    OLEMISC_ACTIVATEWHENVISIBLE | \
    OLEMISC_SETCLIENTSITEFIRST\
)

//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_INTERFACEMAP \
/*DECLARE_REGISTRY_RESOURCEID(IDR_BmlWRAPPER)*/\
BEGIN_COM_MAP(CBmlWRAPPER)\
    COM_INTERFACE_ENTRY(IBmlWRAPPER)\
    COM_INTERFACE_ENTRY(IDispatch)\
    COM_INTERFACE_ENTRY(IViewObjectEx)\
    COM_INTERFACE_ENTRY(IViewObject2)\
    COM_INTERFACE_ENTRY(IViewObject)\
    COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)\
    COM_INTERFACE_ENTRY(IOleInPlaceObject)\
    COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)\
    COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)\
    COM_INTERFACE_ENTRY(IOleControl)\
    COM_INTERFACE_ENTRY(IOleObject)\
    COM_INTERFACE_ENTRY(IPersistStreamInit)\
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)\
    COM_INTERFACE_ENTRY(ISupportErrorInfo)\
    COM_INTERFACE_ENTRY(IConnectionPointContainer)\
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)\
    COM_INTERFACE_ENTRY(IQuickActivate)\
    COM_INTERFACE_ENTRY(IPersistStorage)\
    COM_INTERFACE_ENTRY(IDataObject)\
    COM_INTERFACE_ENTRY(IProvideClassInfo)\
    COM_INTERFACE_ENTRY(IProvideClassInfo2)\
END_COM_MAP()


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_BASEPROPMAP \
BEGIN_PROP_MAP(CBmlWRAPPER)\
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)\
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)\
    /* Example entries */\
    /* PROP_ENTRY("Property Description", dispid, clsid) */\
    /* PROP_PAGE(CLSID_StockColorPage) */\
END_PROP_MAP()



//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_CONNECTIONPTMAP \
BEGIN_CONNECTION_POINT_MAP(CBmlWRAPPER)\
    CONNECTION_POINT_ENTRY(DIID_IBmlWRAPPEREVENTS)\
END_CONNECTION_POINT_MAP()


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_MSGMAP \
BEGIN_MSG_MAP(CBmlWRAPPER)\
    DECLARE_IDLBMLWRAPPERCTRL_EVENTS_MESSAGE_HANDLERS\
    CHAIN_MSG_MAP(CComControl<CBmlWRAPPER>)\
    DEFAULT_REFLECTION_HANDLER()\
END_MSG_MAP()


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_IVIEWOBJECTEX \
/* IViewObjectEx */\
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_DEFAULTIMPL \
public: \
    DECLARE_WRAPPER_STANDARD_CONSTRUCTORS\
    DECLARE_WRAPPER_STANDARD_CUSTOM_UPDATEREGISTRY\
    DECLARE_WRAPPER_AX_OLEMISC_STATUS\
    DECLARE_WRAPPER_AX_INTERFACEMAP\
    DECLARE_WRAPPER_AX_BASEPROPMAP\
    DECLARE_WRAPPER_AX_CONNECTIONPTMAP\
    DECLARE_WRAPPER_AX_MSGMAP\
    DECLARE_WRAPPER_STANDARD_ISUPPORTERRORINFO\
    DECLARE_WRAPPER_AX_IVIEWOBJECTEX


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_DRAW_AND_EVENT_HANDLERS\
    HRESULT OnDraw(ATL_DRAWINFO& di);\
    LRESULT OnEventIdl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);\
	int     ProcessKeyEvent(WPARAM wParam, LPARAM lParam, long& keyCode, long& kState);


//---------------------------------------------------------------------------------------------
#define DECLARE_WRAPPER_AX_MEMBERS \
private: \
    HANDLE          m_hImage;\
    BITMAP          m_bmpInfo;\
    UINT            m_currMsg;\
    HWND            m_currHwnd;\
    BOOL            m_bCursorIn;\
    BOOL            m_bCurrRuntimeMode;\
	long			m_lastMouseX;\
	long			m_lastMouseY;\
	RECT			m_rcClient;


