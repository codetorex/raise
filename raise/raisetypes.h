
#ifndef RAISETYPES_H
#define RAISETYPES_H

#include <assert.h>

#define CPU_LITTLE_ENDIAN

// #define CPU_BIG_ENDIAN

// NOTE: Maybe needed  -std=c++0x for linux compilations

//////////////////////////////////////////////////////////////////////////
/* Dont forget to Define LINUX for linux build, and M64 for 64 bit */

// Custom Types And Generic Macros Comes Here

#define _RT(x)				TString( #x )
#define _RW(x)				TString( L ## x )

#define SOURCENAME(x)		x

#define NIL					((void*)0)

typedef unsigned long long	ui64;
typedef unsigned int		ui32; // It was unsigned long before but in linux longs have 8 bytes. (64 bit stuff?)
typedef unsigned short		ui16;
typedef unsigned char		ui8;
typedef signed long long	i64;
typedef int					i32;
typedef short				i16;
typedef char				i8;
typedef unsigned char		byte; // technically ui8

#define DWORDSTR(x)			(*(ui32*)x)

#define PI					3.1415926535897f
#define DOUBLEPI			6.2831853071794f
#define HALFPI				1.5707963267948f



#define KB					(1024)
#define MB					(1048576)

#define RDLL

#ifdef WIN32
#ifdef DLLMODE

#ifdef RAISE_EXPORTS
#define RDLL __declspec(dllexport)
#else
#define RDLL __declspec(dllimport)
#endif

#endif

#else // If not win32 then GCC class visibility...
#define RDLL __attribute__ ((visibility("default")))
#endif

#ifndef WHEEL_DELTA
#define WHEEL_DELTA			120
#endif


#define MXDWORD				0xFFFFFFFF

#define MAX_UI32			((ui32)~((ui32)0))

//#define NOPNGLIB

#endif