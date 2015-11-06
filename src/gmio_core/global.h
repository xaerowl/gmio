/****************************************************************************
** gmio
** Copyright Fougue (2 Mar. 2015)
** contact@fougue.pro
**
** This software is a reusable library whose purpose is to provide complete
** I/O support for various CAD file formats (eg. STL)
**
** This software is governed by the CeCILL-B license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-B
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html".
****************************************************************************/

/*! \file global.h
 *  Fundamental global declarations, included by almost all other header files
 *
 *  \defgroup gmio_core gmioCore
 *  All other gmio modules rely on this module
 *
 *  \addtogroup gmio_core
 *  @{
 */

#ifndef GMIO_GLOBAL_H
#define GMIO_GLOBAL_H

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) \
      || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) \
      || defined(__NT__)
#  define GMIO_OS_WIN
#endif /* Windows */

#if defined(__linux) || defined(__linux__) || defined(linux)
#  define GMIO_OS_LINUX
#endif /* Linux */

#if defined(__APPLE__)
#  define GMIO_OS_MAC
#endif /* Apple */

#ifdef GMIO_OS_WIN
#  define GMIO_DECL_EXPORT __declspec(dllexport)
#  define GMIO_DECL_IMPORT __declspec(dllimport)
#else
/*! Expands to the C compiler extension to export functions to a DLL */
#  define GMIO_DECL_EXPORT
/*! Expands to the C compiler extension to import functions from a DLL */
#  define GMIO_DECL_IMPORT
#endif /* WIN */

#ifdef GMIO_LIB_DLL
#  ifdef GMIO_LIB_MAKE_DLL
#    define GMIO_LIB_EXPORT GMIO_DECL_EXPORT
#  else
#    define GMIO_LIB_EXPORT GMIO_DECL_IMPORT
#  endif /* GMIO_LIB_MAKE_DLL */
#else
/*! Expands either to GMIO_DECL_EXPORT or GMIO_DECL_IMPORT when respectively
 *  compiling/using the DLL */
#  define GMIO_LIB_EXPORT
#endif /* GMIO_LIB_DLL */

/* "config.h" is generated by cmake, it should reside in the out-of-source
 * build dir.
 * In CMakeFiles.txt, the directory where resides "config.h" is added to the
 * include path list
 */
#include "config.h"

#ifdef GMIO_HAVE_STDINT_H
#  include <stdint.h>
#else
typedef char          int8_t;
typedef unsigned char uint8_t;

/* TODO: short is not necessarily 16b on all archs */
typedef short          int16_t;
typedef unsigned short uint16_t;

/* TODO: int is not necessarily 32b on all archs */
typedef int          int32_t;
typedef unsigned int uint32_t;
#endif /* GMIO_USE_STDINT_H */

#ifndef GMIO_HAVE_INT64_T
#  if defined(GMIO_HAVE_MSVC_INT64)
typedef __int64_t          int64_t;
typedef unsigned __int64_t uint64_t;
#  elif defined(GMIO_HAVE_LONG_LONG)
typedef long long          int64_t;
typedef unsigned long long uint64_t;
#  endif
#endif /* !GMIO_HAVE_INT64_T */

#if defined(GMIO_HAVE_INT64_T) \
      || defined(GMIO_HAVE_MSVC_INT64) \
      || defined(GMIO_HAVE_LONG_LONG)
#  define GMIO_HAVE_INT64_TYPE
#endif

#ifdef GMIO_HAVE_STDBOOL_H
#  include <stdbool.h>

typedef bool gmio_bool_t;

#  define GMIO_FALSE false
#  define GMIO_TRUE  true
#elif !defined(DOXYGEN)
typedef int gmio_bool_t;

enum gmio_bool_value
{
    GMIO_FALSE = 0,
    GMIO_TRUE = 1
};
#else
/* For documentation only */
/*! Boolean type alias
 *
 *  If strict ISO-C90 or if \c <stdbool.h> does not exists then:
 *      \li \c gmio_bool_t is an alias of \c int
 *      \li <tt>GMIO_FALSE == 0</tt> and <tt>GMIO_TRUE == 1</tt>
 *
 *  Otherwise:
 *      \li \c gmio_bool_t is an alias of \c bool
 *      \li \c GMIO_FALSE expands to \c false and \c GMIO_TRUE to \c true
 */
typedef int_or_bool gmio_bool_t;
#endif /* GMIO_HAVE_STDBOOL_H */

/*! Typedef for 32bit real type (float) */
typedef float  gmio_float32_t;

/*! Typedef for 64bit real type (double) */
typedef double gmio_float64_t;

/*! Tells the compiler that a parameter is not used in the body of a function */
#define GMIO_UNUSED(x) (void)x;

#ifndef GMIO_INLINE
#  if defined(__GNUC__)
#    define GMIO_INLINE __inline__ static  /* Compatible with C90 */
#  elif defined(_MSC_VER)
#    define GMIO_INLINE __inline static
#  elif !defined(DOXYGEN)
#    define GMIO_INLINE static
#  else
/*! Expands to the C compiler specific inline keyword (if any) */
#    define GMIO_INLINE
#  endif
#endif /* !GMIO_INLINE */

#ifndef GMIO_RESTRICT
#  if defined(__GNUC__)
#    define GMIO_RESTRICT __restrict__  /* Compatible with C90 */
#  elif defined(_MSC_VER)
#    define GMIO_RESTRICT __restrict
#  elif defined(GMIO_HAVE_C99_RESTRICT) /* TODO: add cmake detectection */
#    define GMIO_RESTRICT restrict
#  else
/*! Expands to the C compiler specific restrict keyword (if any) */
#    define GMIO_RESTRICT
#  endif
#endif /* !GMIO_RESTRICT */

#ifdef __cplusplus
#  define GMIO_C_LINKAGE_BEGIN extern "C" {
#  define GMIO_C_LINKAGE_END   }
#else
/*! Expands to <tt>extern "C" {</tt> when building with a C++ compiler */
#  define GMIO_C_LINKAGE_BEGIN
/*! Expands to \c } when building with a C++ compiler */
#  define GMIO_C_LINKAGE_END
#endif /* __cplusplus */

#endif /* GMIO_GLOBAL_H */
/*! @} */
