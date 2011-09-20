
#ifndef RAISETYPES_H
#define RAISETYPES_H

#include <assert.h>

// NOTE: Maybe needed  -std=c++0x for linux compilations

//////////////////////////////////////////////////////////////////////////
/* Dont forget to Define LINUX for linux build, and M64 for 64 bit */

// Custom Types And Generic Macros Comes Here

#define NIL			((void*)0)

typedef unsigned char	byte;
typedef unsigned short	uint16;
typedef unsigned int	ui32; // It was unsigned long before but in linux longs have 8 bytes. (64 bit stuff?)

#define DWORDSTR(x) (*(ui32*)x)

#define PI			3.1415926535897f
#define HALFPI		1.5707963267948f

#define RADTODEG(rad)		((rad) * (180.0f / PI))
#define DEGTORAD(deg)		((deg) * (PI / 180.0f))

#define KB			(1024)
#define MB			(1048576)

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

#define AttachFunctionPointer(a,b) *(long*)(&(a)) = (long)((void*)(*(long*)((long)&b)));

#ifndef WHEEL_DELTA
#define WHEEL_DELTA			120
#endif


#define MXDWORD		0xFFFFFFFF

#define DIV2(x)				( (x) >> 1 )
#define DIV4(x)				( (x) >> 2 )
#define DIV8(x)				( (x) >> 3 )
#define DIV16(x)			( (x) >> 4 )
#define DIV32(x)			( (x) >> 5 )

#define MUL2(x)				( (x) << 1 )
#define MUL4(x)				( (x) << 2 )
#define MUL8(x)				( (x) << 3 )
#define MUL16(x)			( (x) << 4 )
#define MUL32(x)			( (x) << 5 )

// and with ( 2^n - 1 )
#define MOD2(x)				( (x) & 1  )
#define MOD4(x)				( (x) & 3  )
#define MOD8(x)				( (x) & 7  )
#define MOD16(x)			( (x) & 15 )
#define MOD32(x)			( (x) & 31 )

#define DW1BYTE			0x000000FF
#define DW2BYTE			0x0000FF00
#define DW3BYTE			0x00FF0000
#define DW4BYTE			0xFF000000

//#define NOPNGLIB

#endif