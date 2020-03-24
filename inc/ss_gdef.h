#ifndef __SS_GDEF_H__
#define __SS_GDEF_H__
/*-----------------------------------------------------------------------------
    Copyright(c) Tellabs Transport Group Inc. All rights reserved.

SUBSYSTEM:      Software Services
TARGET:
AUTHOR:         Sep 01, 1998 - Christian Legault
DESCRIPTION:    This file includes general definitions for the Tellabs 7100.
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************/
/*                                                                     */
/*   General Definitions                                               */
/*                                                                     */
/***********************************************************************/
typedef unsigned int uint;
typedef unsigned int bits;

typedef unsigned char bool8;

typedef unsigned char bits8;
typedef unsigned short bits16;
typedef unsigned long bits32;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

#ifdef __GNUC__
#ifdef __CHAR_UNSIGNED__
typedef signed char int8;
#else
typedef char int8;
#endif
#else
typedef char int8;
#endif

typedef short int16;
typedef long int32;

// 64-bit integers on Motorola/Freescale 32-bit PowerPC processors are
// software-based (supported by the compiler), but are generally faster
// than floating point for handling large numbers.  Use judiciously.
typedef long long int64;
typedef unsigned long long uint64;
#define COMPILER_SUPPORTS_64BIT_INTS

typedef short bbindex_t; /*Used as type for index in container.*/

/***********************************************************************/
/*                                                                     */
/*   Old definitions used in the BSP                                   */
/*                                                                     */
/***********************************************************************/

// #if defined(LINUX) || defined(LINUX_SIM)
// typedef unsigned char UINT8;
// typedef unsigned short UINT16;
// typedef unsigned int UINT32;
// typedef int INT32;
// #ifndef INT8
// typedef char INT8;
// #endif
// typedef short INT16;
// #endif


#ifndef __cplusplus
/* bool is not defined in the C compiler */
typedef char bool;
#define true 1
#define false 0

/* Old definitions */
typedef char Bool;
#ifndef BOOLEANDEF
typedef char BOOLEAN;
#define BOOLEANDEF
#endif
#endif /* __cplusplus */

/***********************************************************************/
/*                                                                     */
/*   Error definitions                                                 */
/*                                                                     */
/***********************************************************************/
#ifndef NO_ERR
#define NO_ERR 0
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE 1
#endif

#ifdef __cplusplus
}
#endif

#endif /*  __SS_GDEF_H__ */
