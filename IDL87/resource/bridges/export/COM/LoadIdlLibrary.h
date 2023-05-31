/*
 *	Copyright (c)  Harris Geospatial Solutions, Inc.  All rights reserved.
 *	This software includes information which is proprietary to and a
 *	trade secret of Harris Geospatial Solutions, Inc.  It is not to be disclosed
 *	to anyone outside of this organization. Reproduction by any means
 *	whatsoever is  prohibited without express written permission.
 */


#ifndef __LOADIDLLIBRARY_H__
#define __LOADIDLLIBRARY_H__

//------------------------------------------------------------------------
// WARNING: If you makes changes to these *.cpp/*.h files, make sure you copy both
//          into bridge\MscomWrapperTmpl directory, since it also used by the wrappers.
//          Also, make sure you inform Alan of the changes so that he can update the 
//          wrapper generation code in the "Export Bridge Assistant" (wizard).
//------------------------------------------------------------------------

#define ENV_BRIDGE_DEBUG		"IDL_BRIDGE_DEBUG"



// WARNING: These must match the same values found in idl_bml_defines.h
//          We don't include idl_bml_defines.h because this file
//          is used by many different "products" that may not want to link
//          to the BML definitions.
//
#if !defined(IDL_BML_OPS_ARCH_BITS_DEFINED)
#define IDL_BML_OPS_ARCH_BITS_DEFINED

typedef enum
{
    IDL_BML_OPS_ARCH_BITS_DEFAULT = 0,
    IDL_BML_OPS_ARCH_BITS_32,
    IDL_BML_OPS_ARCH_BITS_64
} IDL_BML_OPS_ARCH_BITS_E;

#endif



//------------------------------------------------------------------------
// This file is also includein the idl_ebutil.dll project, which is built
// by the JavaPal build since the exported routines are also exported into the 
// Java world.  The EBUTILS_EXPORT define is set when this file is included as 
// part of the Java exported library build.
//
#if defined(EBUTILS_EXPORT) && defined(MSWIN)

    #define EBUTILS_API		extern "C" __declspec(dllexport)
#else

    #define EBUTILS_API		extern "C"
#endif


//------------------------------------------------------------------------
#if defined(MSWIN)

#include "TCHAR.H"


BOOL LoadIdlLibrary             (_TCHAR* szLibName, _TCHAR* szQueryKey, HMODULE* phMod,     
                                 _TCHAR* szErrDir = NULL, long cbErrDirBuff = -1);



// If szTestFile is specified, then it is checked in each directory that is determined.
// If all directories fail to produce the file, then an error is returned.
//
// Return value:
//   TRUE  - the requested dir was correctly discovered, and the client's buffer is filled
//           out with the directory.
//   FALSE - the requested dir could not be determined, or
//           the client buffer was too small for the directory string, or
//           error accessing the registry.
//
//
EBUTILS_API BOOL	IDL_EBUTIL_GetIDLBinDir (_TCHAR* szDir, long cbDir, _TCHAR* szTestFile = NULL);

#endif // #if defined(MSWIN)


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// The following are available to MSWIN & Unix builds...
//

EBUTILS_API char*   IDL_EBUTIL_GetEnv                   (char* szEnv);
EBUTILS_API int     IDL_EBUTIL_GetEnvBridgeDebug        (void);

#if defined(MSWIN)
EBUTILS_API BOOL    IDL_EBUTIL_GetReqIDLArchDir         (IDL_BML_OPS_ARCH_BITS_E reqArch, _TCHAR* szDir, long cchDir);
EBUTILS_API BOOL    IDL_EBUTIL_GetOPXBinDir             (IDL_BML_OPS_ARCH_BITS_E reqArch, _TCHAR* szDir, long cchDir);
#else
EBUTILS_API int     IDL_EBUTIL_GetReqIDLArchDir         (IDL_BML_OPS_ARCH_BITS_E reqArch, char* szDir, long cchDir);
EBUTILS_API int     IDL_EBUTIL_GetOPXBinDir             (IDL_BML_OPS_ARCH_BITS_E reqArch, char* szDir, long cchDir);
#endif


#endif

