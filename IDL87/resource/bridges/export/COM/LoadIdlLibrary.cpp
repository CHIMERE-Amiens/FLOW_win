/*
 *	Copyright (c)  Harris Geospatial Solutions, Inc.  All rights reserved.
 *	This software includes information which is proprietary to and a
 *	trade secret of Harris Geospatial Solutions, Inc.  It is not to be disclosed
 *	to anyone outside of this organization. Reproduction by any means
 *	whatsoever is  prohibited without express written permission.
 */
#include "stdafx.h"
#include <stdio.h>

#include "LoadIdlLibrary.h"

// WARNING: This code is also compiled by the Export Bridge MSCOM PAL library
// in *UNICODE* mode, which is why you have to use _TCHAR everywhere.
// If you add new code and only use char*, you need to make sure you don't break
// the MSCOM PAL library build.
//

// Include IDL version and platform defines.
#include "prog_nam_ver.h"

#ifdef unix
#include <errno.h>
#endif

#define IDL_BIN_DIR_VALIDATION_FILE "idl.dll"


//------------------------------------------------------------------------
// WARNING: If you makes changes to these *.cpp/*.h files, make sure you copy both
//          into bridge\MscomWrapperTmpl directory, since it also used by the wrappers.
//          Also, make sure you inform Alan of the changes so that he can update the 
//          wrapper generation code in the "Export Bridge Assistant" (wizard).
//------------------------------------------------------------------------


#if defined (WIN32) && !defined (MSWIN)
#pragma message("---------------------------------------------------------------------------------------")
#pragma message("You have included LoadIdlLibrary.cpp into your project, but you have not")
#pragma message("defined the symbol MSWIN.  This will cause LoadIdlLibrary() to be undefined.")
#pragma message("---------------------------------------------------------------------------------------")
#endif

//------------------------------------------------------------------------

#if !defined(DEBUG_MSG_LEVEL)
	#define  DEBUG_MSG_LEVEL		1
#endif

#if !defined(MAX_DBG_BUFF_SIZE)
	#define  MAX_DBG_BUFF_SIZE		2048
#endif

#if !defined(DebugMsg)
	#if defined(_DEBUG)

		#if defined(MSWIN)

			#define DebugMsg(_dbl,_msg)\
				OutputDebugString(_msg)

			#define DebugMsgF1(_dbl,_msg,_p1)\
				if ( _dbl <= DEBUG_MSG_LEVEL ) {\
					_TCHAR szDbg[MAX_DBG_BUFF_SIZE];\
					_sntprintf(szDbg,MAX_DBG_BUFF_SIZE,_msg,_p1);\
					OutputDebugString(szDbg); }
		#else

			#define DebugMsg(_dbl,_msg)\
				if ( _dbl <= DEBUG_MSG_LEVEL )		{ printf(_msg); }
			#define DebugMsgF1(_dbl,_msg,_p1)\
				if ( _dbl <= DEBUG_MSG_LEVEL )		{ printf(_msg,_p1); }

		#endif

	#else

		#define DebugMsg(_dbl,_msg)		
		#define DebugMsgF1(_dbl,_msg,_p1)

	#endif
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Windows specific routines:
//
#if defined(MSWIN)


//------------------------------------------------------------------------
//
static BOOL IDL_EBUTIL_GetIDLBinDirFromModule(_TCHAR* szDir,
  long cbDir, _TCHAR* szTestFile, _TCHAR* szModule)
{
  _TCHAR szLibPath[MAX_PATH];
  long cbLibPath;
  memset(szLibPath,0,sizeof(szLibPath));

  HMODULE hModule = GetModuleHandle(szModule);
  if (hModule == NULL)
    return FALSE;

  if ( szTestFile == NULL )
  {
    szTestFile  = _T("idl.dll");
  }

  DWORD dw = GetModuleFileName(hModule,szLibPath,MAX_PATH);
  if ( dw == 0 )
    return FALSE;

  // Hack off the trailing module name;
  _TCHAR* p = _tcsrchr(szLibPath,'\\');
  if ( p )
    *p = '\0';

  cbLibPath = (long)_tcslen(szLibPath);

  if ( cbLibPath == 0 )
    return FALSE;

  _tcsncpy(szDir, szLibPath, cbDir);

  if ( szLibPath[cbLibPath-1] != '\\' && szLibPath[cbLibPath-1] != '/' )
    _tcscat(szLibPath,_T("\\"));

  WIN32_FIND_DATA		ffd;
  _TCHAR				szFFD[MAX_PATH*2];

  _stprintf(szFFD,_T("%s%s"),szLibPath,szTestFile);

  HANDLE hFile  = FindFirstFile(szFFD,&ffd);
  if (hFile == INVALID_HANDLE_VALUE)
    return FALSE;

  FindClose(hFile);

  // If there is not enough room in the client's buffer, return an error.
  if ( (long)cbLibPath >= cbDir )
    return FALSE;
  memset(szDir, 0, cbDir);
  _tcsncpy(szDir, szLibPath, cbDir);
  return TRUE;
}


EBUTILS_API 
BOOL IDL_EBUTIL_GetIDLBinDir(_TCHAR* szDir, long cbDir, _TCHAR* szTestFile)
{
  if ( !szDir  || cbDir < 0 )
    return FALSE;

  // First look in the directory of our executable.
  if (IDL_EBUTIL_GetIDLBinDirFromModule(szDir, cbDir, szTestFile, NULL))
    return TRUE;

  // Then try some other common modules.
  if (IDL_EBUTIL_GetIDLBinDirFromModule(szDir, cbDir, szTestFile, _T("idl.dll")))
    return TRUE;

  if (IDL_EBUTIL_GetIDLBinDirFromModule(szDir, cbDir, szTestFile, _T("idl_bml.dll")))
    return TRUE;

  _TCHAR* pEnv = _tgetenv(_T("IDL_BIN_DIR"));
  if ( pEnv && *pEnv != '\0' )
  {
    // If there is not enough room in the client's buffer, return an error.
    long cbLibPath = (long)_tcslen(pEnv);
    if ( (long)cbLibPath >= (cbDir-1) )
      return FALSE;
    memset(szDir, 0, cbDir);
    _tcsncpy(szDir, pEnv, cbDir);

    // If IDL_BIN_DIR is defined but it is not the same as our
    // architecture (32-bit versus 64-bit) then switch it.
#ifdef _WIN64
#define IDL_BIN_DIR_WRONG IDL_BIN_DIR_32
#define IDL_BIN_DIR_RIGHT IDL_BIN_DIR_64
#else
#define IDL_BIN_DIR_WRONG IDL_BIN_DIR_64
#define IDL_BIN_DIR_RIGHT IDL_BIN_DIR_32
#endif
    _TCHAR* szBin = _tcsstr(szDir, _T("\\bin\\") _T(IDL_BIN_DIR_WRONG));
    if (szBin != NULL)
    {
      *szBin = '\0';
      _tcscat(szDir, _T("\\bin\\") _T(IDL_BIN_DIR_RIGHT));
    }

    if ( szDir[cbLibPath-1] != '\\' && szDir[cbLibPath-1] != '/' )
      _tcscat(szDir,_T("\\"));
    return TRUE;
  }

  return FALSE;
}


//-----------------------------------------------------------------------------
BOOL LoadIdlLibrary(_TCHAR* szLibName, _TCHAR* szQueryKey, HMODULE* phMod, 
					_TCHAR* szErrDir,  long cbErrDirBuff)
{
	if ( !szLibName  || _tcslen(szLibName)  == 0 || 
		 !szQueryKey || _tcslen(szQueryKey) == 0 || !phMod )
		return FALSE;

	_TCHAR szErr[MAX_DBG_BUFF_SIZE];
	int    lEnvDebug = IDL_EBUTIL_GetEnvBridgeDebug();

	long   cbLibPath = MAX_PATH;
	_TCHAR szLibPath[MAX_PATH];

	memset(szLibPath,0,sizeof(szLibPath));

	if ( !IDL_EBUTIL_GetIDLBinDir(szLibPath,MAX_PATH,szLibName) )
	{
		if ( szErrDir )
		{
			memset  (szErrDir, 0, cbErrDirBuff);
			_tcsncpy(szErrDir, szLibPath, cbErrDirBuff);
		}
		_stprintf(szErr, _T("Failed to locate library file '%s' in directory '%s'\n"),
								szLibName, (szLibPath[0] != NULL ? szLibPath : _T("")));
		DebugMsgF1(0,_T("%s"),szErr);

		if ( lEnvDebug > 0 )
		{
			OutputDebugString(szErr);
			_tprintf(szErr);
		}

		return FALSE;
	}

	if ( szErrDir )
	{
		memset  (szErrDir, 0, cbErrDirBuff);
		_tcsncpy(szErrDir, szLibPath, cbErrDirBuff);
	}

	_TCHAR szCurrDir[MAX_PATH+1];
	memset(szCurrDir,0,sizeof(szCurrDir));
	if ( !GetCurrentDirectory(MAX_PATH,szCurrDir) ||
		 !SetCurrentDirectory(szLibPath) )
	{
		DebugMsg(0,_T("LoadIdlLibrary: Get/SetCurrentDirectory() failed.\n"));
		return FALSE;
	}

	_tcscat(szLibPath,szLibName);

  *phMod = ::LoadLibrary( szLibPath );

	// CR 42643 - BML no longer messes up Current Working Directory on Windows.
	SetCurrentDirectory(szCurrDir);

	if ( !*phMod && lEnvDebug > 0 )
	{
		_stprintf(szErr, _T("LoadLibrary(%s) failed.\n"),szLibPath);
		OutputDebugString(szErr);
		_tprintf(szErr);
	}

	return ( *phMod != NULL );
}


#endif // #if defined(MSWIN)


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// The following are available to MSWIN & Unix builds...
//

//-----------------------------------------------------------------------------
// Returns:
//   NULL - if ENV does not exist or an error occurred.
//
EBUTILS_API char* IDL_EBUTIL_GetEnv(char* szEnv)
{
	if ( !szEnv || strlen(szEnv) == 0 )
		return (char*)0;

#if defined(MSWIN)

	#define           MAX_ENV_SIZE    2048
	static char szenv[MAX_ENV_SIZE];

	DWORD dwSize  = GetEnvironmentVariableA(szEnv,szenv,MAX_ENV_SIZE);
	if (  dwSize <= MAX_ENV_SIZE )
		return szenv;
	else
		return NULL;	// Env value is larger than our buffer: should we dump out error info?
#else

	char*  penv = getenv(szEnv);
	if (  !penv )
	{
		DebugMsgF2(0,"IDL_EBUTIL_GetEnv(%s) failed: errno: %d\n",szEnv,errno);
	}
	return penv;
#endif
}



//-----------------------------------------------------------------------------
// Returns:
//   0 - if ENV not found, or upon any errors
//
EBUTILS_API int IDL_EBUTIL_GetEnvBridgeDebug(void)
{
	char* pszEnv = IDL_EBUTIL_GetEnv(ENV_BRIDGE_DEBUG);
	if ( !pszEnv )
		return 0;

	int env = 0;
	sscanf( pszEnv, "%d", &env );

	return env;
}



//-----------------------------------------------------------------------------
// Returns NULL if the requested architecture directory does not exist
//
#if defined(MSWIN)

EBUTILS_API 
BOOL IDL_EBUTIL_GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_E reqArch, _TCHAR* szDir, long cchDir)
{
  return FALSE;
}

//-----------------------------------------------------------------------------
// This routine encapsulates all the business rules for finding the OPX (mixed bit
// mode OPS) server directory.
//
EBUTILS_API 
BOOL IDL_EBUTIL_GetOPXBinDir(IDL_BML_OPS_ARCH_BITS_E reqArch, _TCHAR* szDir, long cchDir)
{
  BOOL bRet = IDL_EBUTIL_GetIDLBinDir(szDir, cchDir, _T("idl.dll"));

  if (bRet)
  {
    if (reqArch == IDL_BML_OPS_ARCH_BITS_32)
    {
      _TCHAR* szBin64 = _tcsstr(szDir, _T("\\bin\\") _T(IDL_BIN_DIR_64));
      if (szBin64 != NULL)
      {
        *szBin64 = '\0';
        _tcscat(szDir, _T("\\bin\\") _T(IDL_BIN_DIR_32));
      }
    }
    else if (reqArch == IDL_BML_OPS_ARCH_BITS_64)
    {
      _TCHAR* szBin64 = _tcsstr(szDir, _T("\\bin\\") _T(IDL_BIN_DIR_32));
      if (szBin64 != NULL)
      {
        *szBin64 = '\0';
        _tcscat(szDir, _T("\\bin\\") _T(IDL_BIN_DIR_64));
      }
    }
  }

  return bRet;
}


#else 
/* Unix implementation */

EBUTILS_API 
int IDL_EBUTIL_GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_E reqArch, char* szDir, long cchDir)
{
	if ( !szDir || cchDir < 1 )
		return FALSE;

	char*  pszenv;
	size_t slen;

	switch ( reqArch )
	{
		default:
		case IDL_BML_OPS_ARCH_BITS_DEFAULT:

			pszenv = IDL_EBUTIL_GetEnv(ENV_IDLDE_IDLBITS);

			if ( !pszenv || (slen = strlen(pszenv)) == 0 )
			{
				DebugMsgF1(0,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_DEFAULT): GetEnv(%s) failed\n",ENV_IDLDE_IDLBITS);
				return FALSE;
			}
			else 
			{
				DebugMsgF2(1,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_DEFAULT): GetEnv(%s) returned: %s\n", 
								ENV_IDLDE_IDLBITS, pszenv);
			}

			// We only support the values of "32" or "64"
			if ( strstr(pszenv,"32") != 0 )
				return IDL_EBUTIL_GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_32, szDir, cchDir);
			else if ( strstr(pszenv,"64") != 0 )
				return IDL_EBUTIL_GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_64, szDir, cchDir);
			else 
			{
				DebugMsgF2(0,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_DEFAULT): Unsupported %s value: %s\n",
									ENV_IDLDE_IDLBITS, pszenv);
				return FALSE;
			}

		case IDL_BML_OPS_ARCH_BITS_32:

			pszenv = IDL_EBUTIL_GetEnv(ENV_IDLDE_ARCHDIR_32);
			if ( !pszenv || (slen = strlen(pszenv)) == 0 || slen >= cchDir )
			{
				DebugMsgF1(0,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_32): GetEnv(%s) failed\n",ENV_IDLDE_ARCHDIR_32);
				return FALSE;
			}
			else 
			{
				DebugMsgF2(1,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_32): GetEnv(%s) returned: %s\n", 
								ENV_IDLDE_ARCHDIR_32, pszenv);
			}
			strcpy(szDir, pszenv);
			return TRUE;

		case IDL_BML_OPS_ARCH_BITS_64:

			pszenv = IDL_EBUTIL_GetEnv(ENV_IDLDE_ARCHDIR_64);
			if ( !pszenv || (slen = strlen(pszenv)) == 0 || slen >= cchDir )
			{
				DebugMsgF1(0,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_64): GetEnv(%s) failed\n",ENV_IDLDE_ARCHDIR_64);
				return FALSE;
			}
			else 
			{
				DebugMsgF2(1,"GetReqIDLArchDir(IDL_BML_OPS_ARCH_BITS_64): GetEnv(%s) returned: %s\n", 
								ENV_IDLDE_ARCHDIR_64, pszenv);
			}
			strcpy(szDir, pszenv);
			return TRUE;
	}

	// We should not be here
	return FALSE;
}

//-----------------------------------------------------------------------------
// This routine encapsulates all the business rules for finding the OPX (mixed bit
// mode OPS) server directory.
//
EBUTILS_API 
int IDL_EBUTIL_GetOPXBinDir(IDL_BML_OPS_ARCH_BITS_E reqArch, char* szDir, long cchDir)
{

	// Since the IDLDE is started by running a script, which very nicely always
	// sets the appropriate environment variables, we can just call the helper
	// function as it has all the logic to do the right thing.
	//	
	int  iret = IDL_EBUTIL_GetReqIDLArchDir( reqArch, szDir, cchDir );
	if ( !iret )
		DebugMsgF3(0, "IDL_EBUTIL_GetOPXBinDir failed: %d %p %d\n",
						(long)reqArch, szDir, cchDir);
	return iret;
}


#endif


