
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

#define AttachFunctionPointer(A,B) *(long*)(&(A)) = (long)((void*)(*(long*)((long)&B)));

#ifndef WHEEL_DELTA
#define WHEEL_DELTA			120
#endif


#define MXDWORD				0xFFFFFFFF

#define MAX_UI32			((ui32)~((ui32)0))

#define DIV2POWN(x,y)		( (x) >> y )

#define DIV2(x)				( DIV2POWN(x,1) )
#define DIV4(x)				( DIV2POWN(x,2) )
#define DIV8(x)				( DIV2POWN(x,3) )
#define DIV16(x)			( DIV2POWN(x,4) )
#define DIV32(x)			( DIV2POWN(x,5) )

/**
 * This maybe different in big endian system.
 */
#define MUL2POWN(x,y)		( (x) << y )

// TODO: test these in raise-test
#define MUL2(x)				( MUL2POWN(x,1) )
#define MUL4(x)				( MUL2POWN(x,2) )
#define MUL8(x)				( MUL2POWN(x,3) )
#define MUL16(x)			( MUL2POWN(x,4) )
#define MUL32(x)			( MUL2POWN(x,5) )

// and with ( 2^n - 1 )
#define MOD2(x)				( (x) & 1  )
#define MOD4(x)				( (x) & 3  )
#define MOD8(x)				( (x) & 7  )
#define MOD16(x)			( (x) & 15 )
#define MOD32(x)			( (x) & 31 )

#define DW1BYTE				0x000000FF
#define DW2BYTE				0x0000FF00
#define DW3BYTE				0x00FF0000
#define DW4BYTE				0xFF000000

//#define NOPNGLIB

#endif