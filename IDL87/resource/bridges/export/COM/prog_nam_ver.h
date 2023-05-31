/*BEGIN_EXPORT*/
#ifndef prog_nam_ver_IDL_VERSION
#define IDL_VERSION_MAJOR 8
#define IDL_VERSION_MINOR 7
#define IDL_VERSION_SUB 1
#define IDL_REVISION 357791
#endif		/* prog_nam_ver_IDL_DEF */
/*END_EXPORT*/
/*
 * prog_nam_ver.h - This was split out from prog_nam.h, so that it can directly
 *                  included in projects such as the Export Bridge wrapper object.
 *
 *
 */

/*
  Copyright (c)  Harris Geospatial Solutions, Inc. All
  rights reserved. This software includes information which is
  proprietary to and a trade secret of Harris Geospatial Solutions, Inc.
  It is not to be disclosed to anyone outside of this organization.
  Reproduction by any means whatsoever is prohibited without express
  written permission.
  */

#ifndef prog_nam_ver_IDL_DEF
#define prog_nam_ver_IDL_DEF


#ifndef config_arch_IDL_DEF
#include "config_arch.h"
#endif



/*
 * Key to understanding some of these definitions:
 *	VERSION_ARCH - Name given to the underlying hardware.
 *	VERSION_OS - Name of the underlying operating system kernel
 *	VERSION_OS_FAMILY - Generic name of underlying OS (e.g. Unix)
 *	VERSION_OS_MARKETING_NAME - Name of the system, as used by
 *		the vendor.
 *
 * Rules:
 *	- Every IDL platform is uniquely identified by its
 *	  VERSION_OS/VERSION_ARCH pair. Once these names are
 *	  selected for a platform, they can never be changed.
 *	  This is because IDL programs depend on them to reliably
 *	  identify systems across IDL versions.
 *	- VERSION_OS_FAMILY: Often the generic name of the OS is what
 *	  matters, and not which specific instance of that OS is being
 *	  used. That's what VERSION_OS_FAMILY is used for.
 *	- VERSION_OS_MARKETING_NAME gives the name of the system as
 *	  used in vendor advertising. This names changes from time to
 *	  time as vendors rename their systems to get a fresh image.
 *	  For this reason, it must not be used for any programatic
 *	  purpose, and is only for use in generating descriptive text.
 *	  If VERSION_OS_MARKETING_NAME is not defined, that means that
 *	  VERSION_OS should be used in those contexts.
 */

/* Architecture */
#ifdef sparc
#define VERSION_ARCH "sparc"
#endif
#ifdef MIPSEL
#define VERSION_ARCH "mipsel"
#endif
#ifdef MIPSEB
#define VERSION_ARCH "mipseb"
#endif
#ifdef hp9000s800
#define VERSION_ARCH "hp_pa"
#endif
#ifdef _IBMR2
#define VERSION_ARCH "ibmr2"
#endif
#ifdef MSWIN_32
#define VERSION_ARCH "x86"
#endif
#ifdef MSWIN_64
#define VERSION_ARCH "x86_64"
#endif
#ifdef SUN_X86
#define VERSION_ARCH "x86"
#endif
#ifdef SUN_X86_64
#define VERSION_ARCH "x86_64"
#endif
#ifdef linux
#if defined(__ALPHA) || defined(__alpha) || defined(__alpha__)
#define VERSION_ARCH "alpha"
#elif defined(LINUX_X86_64)
#define VERSION_ARCH "x86_64"
#else
#define VERSION_ARCH "x86"
#endif
#endif
#ifdef darwin
#if defined(darwin_ppc)
#define VERSION_ARCH "ppc"
#elif defined(darwin_x86_64)
#define VERSION_ARCH "x86_64"
#else
#define VERSION_ARCH "i386"
#endif
#endif
#if defined(__ALPHA) || defined(__alpha)
#if !defined(MSWIN)
#define VERSION_ARCH "alpha"
#endif
#endif

/* Operating System */
#ifdef sun
#define VERSION_OS "sunos"
#define VERSION_OS_MARKETING_NAME "Solaris"
#endif
#ifdef irix
#define VERSION_OS "IRIX"
#endif				/* sgi */
#ifdef hpux
#define VERSION_OS "hp-ux"
#endif
#ifdef _AIX
#define VERSION_OS "AIX"
#endif
#ifdef MSWIN
#define VERSION_OS "Win32"
#define VERSION_OS_FAMILY "Windows"
#define VERSION_OS_MARKETING_NAME "Microsoft Windows"
#endif
#ifdef darwin
#define VERSION_OS "darwin"
#define VERSION_OS_MARKETING_NAME "Mac OS X"
#endif
#ifdef OSF
#define VERSION_OS "OSF"
#define VERSION_OS_MARKETING_NAME "Compaq Tru64"
#endif
#ifdef linux
#define VERSION_OS "linux"
#endif

/* If we haven't already set an OS family, then it's Unix */
#ifndef VERSION_OS_FAMILY
#define VERSION_OS_FAMILY "unix"
#endif


/*
 * Subdirectory name where platform specific binaries are kept.
 *
 * NOTE: These *must* agree with the startapp shell script used by
 * customers to run IDL.
 */
/* The OPX functionality often needs to know the locations of the binary
 * files for both 32- and 64-bit builds.
 */
#ifdef SUN_SPARC
#define IDL_BIN_DIR_32 "bin.solaris2.sparc"
#define IDL_BIN_DIR_64 "bin.solaris2.sparc64"
#ifdef SUN_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#else
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#endif

#ifdef SUN_INTEL
#define IDL_BIN_DIR_32 "bin.solaris2.x86"
#define IDL_BIN_DIR_64 "bin.solaris2.x86_64"
#ifdef SUN_X86_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#else
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#endif

#ifdef irix
#define IDL_BIN_DIR_32 "bin.sgi"
#define IDL_BIN_DIR_64 "bin.sgi.mips64"
#ifdef IRIX_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#else
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#endif

#ifdef hpux
#define IDL_BIN_DIR_32 "bin.hp"
#define IDL_BIN_DIR_64 "bin.hp.pa64"
#ifdef HPUX_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#else
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#endif

#ifdef _AIX
#define IDL_BIN_DIR_32 "bin.ibm"
#define IDL_BIN_DIR_64 "bin.ibm.rs6000_64"
#ifdef AIX_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#else
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#endif

#ifdef OSF
#define IDL_BIN_DIR_32 "bin.alpha"
#define IDL_BIN_DIR_64 "bin.alpha"
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif

#if defined(LINUX_X86_32) || defined(LINUX_X86_64)
#define IDL_BIN_DIR_32 "bin.linux.x86"
#define IDL_BIN_DIR_64 "bin.linux.x86_64"
#ifdef LINUX_X86_32
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#ifdef LINUX_X86_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#endif
#endif

#ifdef MSWIN
#define IDL_BIN_DIR_32 "bin.x86"
#define IDL_BIN_DIR_64 "bin.x86_64"
#ifdef _M_IX86
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#ifdef _M_AMD64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#endif
#endif

#ifdef darwin_ppc
#define IDL_BIN_DIR_32 "bin.darwin.ppc"
#define IDL_BIN_DIR_64 "bin.darwin.ppc"
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif

#if defined(darwin_i386) || defined(darwin_x86_64)
#define IDL_BIN_DIR_32 "bin.darwin.i386"
#define IDL_BIN_DIR_64 "bin.darwin.x86_64"
#ifdef darwin_i386
#define IDL_BIN_DIR IDL_BIN_DIR_32
#endif
#ifdef darwin_x86_64
#define IDL_BIN_DIR IDL_BIN_DIR_64
#endif
#endif




/* We need to protect just the version portion of this file. That way if
   someone includes both prog_nam_ver.h and idl_export.h it doesn't
   try to define these version #defines twice. */

/*BEGIN_EXPORT*/

#ifndef prog_nam_ver_IDL_VERSION2
#define prog_nam_ver_IDL_VERSION2

// Use double-macros to convert a #define number into a string.
// This should work fine with any alphanumeric characters.
#define IDL_STRINGIFY2(s) #s
#define IDL_STRINGIFY(s) IDL_STRINGIFY2(s)

/*
 * The string supplied in !VERSION.RELEASE.
 * We also need another copy without the sub-release field, for use
 * in app_user_dir, so that preferences are shared among sub-releases.
 */
#define IDL_MAJOR_STRING    IDL_STRINGIFY(IDL_VERSION_MAJOR)
#define IDL_MINOR_STRING    IDL_STRINGIFY(IDL_VERSION_MINOR)
#define IDL_SUBMINOR_STRING IDL_STRINGIFY(IDL_VERSION_SUB)

#define IDL_VERSION_STRING    IDL_MAJOR_STRING "." IDL_MINOR_STRING "." IDL_SUBMINOR_STRING
#define IDL_VERSION_STRING_NOSUBMINOR IDL_MAJOR_STRING "." IDL_MINOR_STRING

#define IDL_REVISION_STRING IDL_STRINGIFY(IDL_REVISION)

#endif		/* prog_nam_ver_IDL_VERSION2 */
/*END_EXPORT*/

/* Default location of IDL distribution tree */
#define VERSION_DIR_NAME "idl" IDL_MAJOR_STRING IDL_MINOR_STRING

/* Environment variables set by the Unix idlde startup script
 * used by the BML and various other libraries
 */
#define ENV_IDLDE_IDLBITS       "IDLDE_IDLBITS"
#define	ENV_IDLDE_ARCHDIR_32    "IDLDE_ARCHDIR_IDL32"
#define	ENV_IDLDE_ARCHDIR_64    "IDLDE_ARCHDIR_IDL64"



#endif		/* prog_nam_ver_IDL_DEF */


