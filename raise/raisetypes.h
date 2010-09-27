
#ifndef RAISETYPES_H
#define RAISETYPES_H

// Custom Types And Generic Macros Comes Here

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;

// FILE PERMISSION DEFINITIONS BIT BY BIT
#define P_READABLE			1
#define P_WRITABLE			2

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


#endif