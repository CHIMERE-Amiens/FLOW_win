/*
 * config_arch.h - Configuration file: architecture platform defines.
 *                 This was pulled from config.h so that projects like
 *                 Export Bridge wrapper objects can directly include this file.
 *
 */

#ifndef config_arch_IDL_DEF
#define config_arch_IDL_DEF

/*BEGIN_EXPORT*/

#ifdef sun
   #ifdef sparc
      #define SUN_SPARC
      #ifdef __sparcv9		/* Building as 64-bit application */
         #define SUN_64
      #endif
   #else
      #define SUN_INTEL
      #ifdef __x86_64 /* Predefined - see man cc */
         #define SUN_X86_64
      #else
         #define SUN_X86
      #endif
   #endif
#endif				/* sun */

#if defined(__alpha) && defined(__osf__)
#define ALPHA_OSF
#endif

#if defined(_WIN32) || defined(MSWIN)
#ifndef MSWIN
#define MSWIN
#endif
#ifdef _WIN64
#define MSWIN_64
#else
#define MSWIN_32
#endif
/*
 * Important note: WIN32 is also defined for 64-bit Windows. Use
 * IDL_MEMINT_64 or MSWIN_64 to detect 64-bit builds.
 */
#ifndef WIN32
#define WIN32
#endif
#endif

#if defined(__linux__) && !defined(linux)
#define linux
#endif

#ifdef linux
#define LINUX_X86
#ifdef __x86_64__
#define LINUX_X86_64
#else
#define LINUX_X86_32
#endif
#endif

#if defined(sgi) && (_MIPS_SZPTR == 64)
#define IRIX_64
#endif

#if defined(_AIX) && defined(__64BIT__)
#define AIX_64
#endif

/*
 * Proper ANSI C doesn't allow pre-defined cpp macros that don't start
 * with an underscore. Explicitly define the ones we use.
 */
#if defined(__hpux) && !defined(hpux)
#define hpux
#endif
#if defined(__hp9000s800) && !defined(hp9000s800)
#define hp9000s800
#endif
#if defined(hpux) && defined(__LP64__)
#define HPUX_64
#endif


/* darwin */
#ifdef __APPLE__
  #ifndef darwin
    #define darwin
  #endif
  #ifdef __ppc__
    #define darwin_ppc
  #endif
  #if defined(__i386__) || defined(__x86_64__)
    #ifdef __LP64__
      #define darwin_x86_64
    #else
      #define darwin_i386
    #endif
  #endif
#endif


/* A rose by any other name: There are multiple ways to say Unix */
#if !defined(unix) && (defined(__unix__) || defined(__unix) || defined(_AIX) || defined(darwin))
#define unix
#endif



/*
 * IDL_SIZEOF_C_LONG: The number of bytes contained in a C long
 * 	integer (i.e. sizeof(long)).
 *
 * IDL_SIZEOF_C_PTR: The number of bytes contained in a machine pointer
 * 	(i.e. sizeof(char *)).
 *
 * The C language does not specify a required size for the short, int or
 * long types. It requires that sizeof(char) is 1, and it requires
 * that the following relationship hold:
 *
 *	sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long).
 *
 * In the days of 16-bit computing (16-bit machine pointers), it was
 * common for short and int to be 2 bytes (16-bits), and sizeof(long)
 * to be 4 (32-bit). In the era of 32-bit computing (32-bit machine
 * pointers), it was almost always true that short remains at 2 bytes
 * while both int and long were 32-bit quantities. This is commonly
 * called ILP32, and is the model used on all 32-bit platforms ever
 * supported by IDL.
 *
 * 64-bit systems fall into two camps: Unix systems use the LP64 model,
 * in which sizeof(short) is 2, sizeof(int) is 4, and both sizeof(long)
 * and machine pointers are 8 (64-bit). Microsoft Windows uses a different
 * approach: sizeof(long) remains at 4 while machine pointers move to 8,
 * and programmers use an explicitly defined type for 64-bit integers.
 *
 * Note that although sizeof(long) generally tracks sizeof(machine pointer),
 * this relationship cannot be always be relied on, so both sizes must be
 * known independently.
 */
#if defined(ALPHA_OSF) || defined(SUN_64) || defined(LINUX_X86_64) || defined(HPUX_64) || defined(IRIX_64) || defined(AIX_64) || defined(darwin_x86_64) || defined(SUN_X86_64)
#define IDL_SIZEOF_C_LONG	8
#else
#define IDL_SIZEOF_C_LONG	4
#endif
#if (IDL_SIZEOF_C_LONG == 8) || defined(MSWIN_64)
#define IDL_SIZEOF_C_PTR	8
#else
#define IDL_SIZEOF_C_PTR	4
#endif


/*END_EXPORT*/



#endif				/* config_arch_IDL_DEF */


