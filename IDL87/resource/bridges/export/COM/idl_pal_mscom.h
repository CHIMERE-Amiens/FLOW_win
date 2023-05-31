/*
 *  Copyright (c)  Harris Geospatial Solutions, Inc.  All rights reserved.
 *  This software includes information which is proprietary to and a
 *  trade secret of Harris Geospatial Solutions, Inc.  It is not to be disclosed
 *  to anyone outside of this organization. Reproduction by any means
 *  whatsoever is  prohibited without express written permission.
 */


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IDL_PAL_MSCOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IDL_PAL_MSCOM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef IDL_PAL_MSCOM_EXPORTS
#define IDL_PAL_MSCOM_API __declspec(dllexport)
#else
#define IDL_PAL_MSCOM_API __declspec(dllimport)
#endif


#ifndef __IDL_BML_TYPEDEFS__
#define __IDL_BML_TYPEDEFS__

// BML data types
typedef int IDL_BML_OPSCONTROL_FLAGS_T;
typedef int IDL_BML_COOKIE_T;
typedef int IDL_BML_ERRCODE_T;
typedef int IDL_BML_ACMSTATUS_T;

#endif


//-------------------------------------------------------------------------------------------------
//#include "idl_bml_defines.h"
#ifndef __IDL_BML_DEFINES_H__
#define __IDL_BML_DEFINES_H__


// Process Control Flags
#define     BML_CONTROL_INPROC          0
#define     BML_CONTROL_OUTPROC         1               // Requires 'szProcName' to be filled out

// Argument parameter flags - can be OR'd together
#define     BML_PARMFLAG_CONST          0x00000001      // Parameter is In-Only.
#define     BML_PARMFLAG_IN_OUT         0x00000002      // Parameter is In/Out.
#define     BML_PARMFLAG_RETVAL         0x00000004      // Value is the results of a function call or property get
#define     BML_PARMFLAG_CONVMAJORITY   0x00004000      // Input array needs conversion to IDL's array majority.
#define     BML_PARMFLAG_MASK           0x0000FFFF      // Parm Flags are in lower-word only.
                                                        // Upper word is for PAL/Wrapper use.
#define     BML_PARMFLAG_COMPLEX_R      0x00000100      // Real-part of complex number
#define     BML_PARMFLAG_COMPLEX_I      0x00000200      // Imaginary-part of complex number


// Sometimes the PALs want to convert IDL error codes into something closer to native error
// codes.  However, the OPS error codes/notifications are not listed in the msg_code.h file
// nor are they negative.  So, we define values here that map to the current OPS codes.
//
// IDL related error/info  codes are in the range -2999 to -2000
// IDL OPS Error           codes are in the range -3999 to -3000
// IDL OPS Notification    codes are in the range -4999 to -4000
#define BML_IDL_ERROR_INFO_BASE         2000
#define BML_OPS_ERROR_BASE              3000
#define BML_OPS_NOTIFICATION_BASE       4000


#define BML_IDL_BUSY                    (-(BML_IDL_ERROR_INFO_BASE + 1))
#define BML_IDL_ABORTED                 (-(BML_IDL_ERROR_INFO_BASE + 2))
#define BML_IDL_ABORT_NOT_OWNER         (-(BML_IDL_ERROR_INFO_BASE + 3))
#define BML_IDL_NOTHING_TO_ABORT        (-(BML_IDL_ERROR_INFO_BASE + 4))

#define BML_OPS_NOTICE_PROCESS_ABORTED  (-(BML_OPS_NOTIFICATION_BASE + 1))


// Asynchronous Command Status
#define BML_ACM_READY                   0
#define BML_ACM_EXECUTING               1
#define BML_ACM_COMPLETED               2
#define BML_ACM_TERMINATEDWITHERROR     3
#define BML_ACM_ABORTED                 4


// Event Ids
#define BMLEVENT_OnMouseDown            1
#define BMLEVENT_OnMouseUp              2
#define BMLEVENT_OnMouseMotion          3
#define BMLEVENT_OnKeyboard             4
#define BMLEVENT_OnExpose               5
#define BMLEVENT_OnResize               6
#define BMLEVENT_OnEnter                7       // Support for the following three are still unclear
#define BMLEVENT_OnExit                 8
#define BMLEVENT_OnScroll               9
#define BMLEVENT_OnMouseWheel           10

#define BMLEVENT_Mousebutton_Left       1
#define BMLEVENT_Mousebutton_Middle     2
#define BMLEVENT_Mousebutton_Right      4

#define BMLEVENT_Keymods_Shift          1
#define BMLEVENT_Keymods_Control        2
#define BMLEVENT_Keymods_Capslock       4
#define BMLEVENT_Keymods_Alt            8

#define KEYBOARD_EVENT_SHIFT            1       // Windows VK_SHIFT,      Unix XK_Shift_L | XK_Shift_R
#define KEYBOARD_EVENT_CONTROL          2       // Windows VK_CONTROL,    Unix XK_Control_L | XK_Control_R
#define KEYBOARD_EVENT_CAPSLOCK         3       // Windows VK_CAPITAL,    Unix XK_Caps_Lock
#define KEYBOARD_EVENT_ALT              4       // Windows VK_MENU,       Unix XK_Alt_L | XK_Alt_R
#define KEYBOARD_EVENT_LEFT             5       // Windows VK_LEFT,       Unix XK_Left
#define KEYBOARD_EVENT_RIGHT            6       // Windows VK_RIGHT,      Unix XK_Right
#define KEYBOARD_EVENT_UP               7       // Windows VK_UP,         Unix XK_Up
#define KEYBOARD_EVENT_DOWN             8       // Windows VK_DOWN,       Unix XK_Down
#define KEYBOARD_EVENT_PAGE_UP          9       // Windows VK_PRIOR,      Unix XK_Page_Up
#define KEYBOARD_EVENT_PAGE_DOWN        10      // Windows VK_NEXT,       Unix XK_Page_Down
#define KEYBOARD_EVENT_HOME             11      // Windows VK_HOME,       Unix XK_Home
#define KEYBOARD_EVENT_END              12      // Windows VK_END,        Unix XK_End
#define KEYBOARD_EVENT_DEL              127     // Set 'isASCII' to '1', when using this code

// NOTES:
//   'Y'        must be relative to the Bottom of the image
//   mbuttons   must be 0 or Values from BMLEVENT_Mousebutton_xxx  OR'd (added) together
//   keymods    must be 0 or Values from BMLEVENT_KeyMods_xxx      OR'd (added) together

typedef struct
{
    long    x;
    long    y;
    long    keyMods;
} BmlEvent_MouseMotion;

typedef struct
{
    long    x;
    long    y;
    long    mButtons;
    long    keyMods;
	long    numClicks;
} BmlEvent_MouseDown;

typedef struct
{
    long    x;
    long    y;
    long    mButtons;
} BmlEvent_MouseUp;

typedef struct
{
    long    x;
    long    y;
    long	delta;                      // direction and distance the wheel has rolled
    long    keyMods;
} BmlEvent_MouseWheel;

typedef struct
{
    long    isASCII;
    long    charCode;                   // If  isASCII, then is the ASCII character code
    long    keyValue;                   // If !isASCII, then value from KEYBOARD_EVENT_xxx
    long    x;
    long    y;
    long    press;                      // TRUE for KeyDown
    long    release;                    // TRUE for KeyUp
    long    keyMods;
} BmlEvent_Keyboard;

typedef struct
{
    long    width;
    long    height;
} BmlEvent_Resize;

typedef struct
{
    long    x;
    long    y;
    long    width;
    long    height;
} BmlEvent_Expose;


typedef struct
{
    long    evtId;                          // Must be one of the BMLEVENT_xxx  defined values
    union {
        BmlEvent_MouseMotion    mMotion;
        BmlEvent_MouseDown      mDown;
        BmlEvent_MouseUp        mUp;
		BmlEvent_MouseWheel		mWheel;
        BmlEvent_Keyboard       keyboard;
        BmlEvent_Resize         resize;
        BmlEvent_Expose         expose;
    } evt;
} BmlEvent;

typedef void (* BML_OUTPUT_CB)(IDL_BML_COOKIE_T cookie, char* szOutput);
typedef void (* BML_NOTIFY_CB)(IDL_BML_COOKIE_T cookie, char* str1, char* str2);


// BML Internal Message Service (IMS) defines.
//
// The following can be OR'd together when registering an IMS callback, in order 
// to subscribe to a subset of messages.  Or, use BML_IMS_SUBSCRIBE_ALL to 
// subscribe to all messages.
// Only one of these values is sent during the actual callback.
//
#define BML_IMS_NULL                0
#define BML_IMS_OPS_EXIT            0x1     // argc == 1, argv[0] == IDL_STRING(name of OPS that is exiting)
#define BML_IMS_CURSOR_CHANGE       0x2     // argc == 1, argv[0] == IDL_STRING(name of cursor shape)
#define BML_IMS_SUBSCRIBE_ALL       -1


// The following values MUST match their IDL_INIT_xxx equivalents from ur_main.h/idl_export.h
#define IDL_BML_LIC_FULL			0;			// No value
#define IDL_BML_LIC_RUNTIME			4;			// #define IDL_INIT_RUNTIME		4
#define IDL_BML_LIC_LICENSED_SAV	12;			// #define IDL_INIT_EMBEDDED	(IDL_INIT_RUNTIME|8)
#define IDL_BML_LIC_VM				8192;		// #define IDL_INIT_VM			(1 << 13)

typedef int IDL_BML_IDL_INIT_DATA_T; 


#endif __IDL_BML_DEFINES_H__


//-------------------------------------------------------------------------------------------------
// MSCom PAL 

typedef void (* PAL_OUTPUT_CB)(BSTR bstrOutput, void* UserData);
typedef void (* PAL_NOTIFY_CB)(BSTR bstr1,  BSTR bstr2, void* UserData);


#define IDL_MSCOMPAL_COOKIE_T           IDL_BML_COOKIE_T
#define IDL_MSCOMPAL_OPSCONTROL_FLAGS_T IDL_BML_OPSCONTROL_FLAGS_T

#define IDL_MSCOMPAL_IDL_INIT_DATA_T    IDL_BML_IDL_INIT_DATA_T


#if !defined(__MSCOMPAL_NO_EXPORT_API__)

extern "C" 
{

IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_Initialize                 (void);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_Release                    (void);

IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_RegisterWrapper            (BSTR bstrObjIdentifier, IDL_MSCOMPAL_OPSCONTROL_FLAGS_T OPSControlFlags,
                                                                         BSTR bstrProcName,      IDL_MSCOMPAL_COOKIE_T* pcookie);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_ReleaseWrapper             (IDL_MSCOMPAL_COOKIE_T cookie);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_CreateObject               (IDL_MSCOMPAL_COOKIE_T cookie, 
                                                                         int argc, VARIANT argv[], int argpal[], 
                                                                         IDL_MSCOMPAL_IDL_INIT_DATA_T idlInitData, 
                                                                         long WindowId, char* szWindowKeywords);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_DestroyObject              (IDL_MSCOMPAL_COOKIE_T cookie);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_OnEvent                    (IDL_MSCOMPAL_COOKIE_T cookie, BmlEvent* pEvt);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_CallMethod                 (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrMethod, 
                                                                         int argc, VARIANT argv[], int argpal[], VARIANT* varResults = NULL);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_SetProperty                (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrProp, int argc, VARIANT argv[], int argpal[]);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_GetProperty                (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrProp, VARIANT* varValue);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_Abort                      (IDL_MSCOMPAL_COOKIE_T cookie);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_GetLastError               (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrLastErr);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_RegisterOutputCallback     (IDL_MSCOMPAL_COOKIE_T cookie, PAL_OUTPUT_CB cbFunc, void* UserData);       
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_UnregisterOutputCallback   (IDL_MSCOMPAL_COOKIE_T cookie);  
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_RegisterNotifyCallback     (IDL_MSCOMPAL_COOKIE_T cookie, PAL_NOTIFY_CB cbFunc, void* UserData);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_UnregisterNotifyCallback   (IDL_MSCOMPAL_COOKIE_T cookie);  

IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_SetIDLVariable             (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrVar, VARIANT  Value);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_GetIDLVariable             (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrVar, VARIANT* Value);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_ExecuteString              (IDL_MSCOMPAL_COOKIE_T cookie, BSTR bstrCmd);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_GetObjVariableName         (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrName);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_GetObjClassName            (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrName);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_GetProcNameFromCookie      (IDL_MSCOMPAL_COOKIE_T cookie, BSTR* bstrName);

// These are special entry points used by the LabVIEW ActiveX control to allow it
// to convert VARIANT-based data IDL data...without having the PAL create an actual IDL object/wrapper.
// Once the 'proxy' wrapper is created, then any of the above entry points can be used.
//
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_CreateProxyWrapper         (IDL_MSCOMPAL_COOKIE_T cookie);
IDL_PAL_MSCOM_API   HRESULT     IDL_MSCOMPAL_DestroyProxyWrapper        (IDL_MSCOMPAL_COOKIE_T cookie);

// This is to support the Export Bridge Wizard.
// Always returns a string pointer.  Upon error; returns an empty string
IDL_PAL_MSCOM_API   char*       IDL_MSCOMPAL_CreateGUID                 (void);

}

#endif
