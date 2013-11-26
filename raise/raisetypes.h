
#ifndef RAISETYPES_H
#define RAISETYPES_H

#include <assert.h>

#define CPU_LITTLE_ENDIAN

// #define CPU_BIG_ENDIAN

// NOTE: Maybe needed  -std=c++0x for linux compilations

//////////////////////////////////////////////////////////////////////////
/* Dont forget to Define LINUX for linux build, and M64 for 64 bit */

// Custom Types And Generic Macros Comes Here


#define SOURCENAME(x)		x

typedef long long			int64;
typedef int					int32;
typedef short				int16;
typedef char				int8;

typedef unsigned long long	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;

typedef int64				i64;
typedef int32				i32;
typedef int16				i16;
typedef int8				i8;

typedef uint64				ui64;
typedef uint32				ui32;
typedef uint16				ui16;
typedef uint8				ui8;

typedef uint8				byte;
typedef uint16				word;
typedef uint32				dword;
typedef uint64				qword;

typedef float				fp32;
typedef double				fp64;

#define DWORDSTR(x)			(*(ui32*)x)

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

#include "texception.h"

//#define NOPNGLIB

#endif