#include "stdafx.h"
#include "resource.h"
#include "BmlWrapperSrvr.h"
#include "wrapper_guids.h"



class CBmlWRAPPER_MODULE : public CAtlDllModuleT< CBmlWRAPPER_MODULE >
{
public :
    DECLARE_LIBID(LIBID_BmlWRAPPER)
    //DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BmlWRAPPERAPP, GUID_APPIDBmlWRAPPER)
    static HRESULT WINAPI UpdateRegistryAppId(BOOL bRegister) throw()
    {
        _ATL_REGMAP_ENTRY aMapEntries [] =
        {
            { OLESTR("APPID"),   GUID_APPIDBmlWRAPPERSTR },
            { OLESTR("APPNAME"), BMLWRAPPER_PROJNAMESTR } ,
            { OLESTR("APPEXT"),  BMLWRAPPER_PROJEXTSTR } ,
            { NULL, NULL }
        };
        return ATL::_pAtlModule->UpdateRegistryFromResource(IDR_BmlWRAPPERAPP, bRegister, aMapEntries);
    }
};

CBmlWRAPPER_MODULE _AtlModule;
HINSTANCE g_hInst = NULL;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    g_hInst = hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
    return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = _AtlModule.DllUnregisterServer();
    return hr;
}
