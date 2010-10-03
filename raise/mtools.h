
#ifndef MTOOLS_H
#define MTOOLS_H
#include "raisetypes.h"

// Raise Min Max templates
template <class T> 
inline const T& rmax ( const T& a, const T& b ) 
{
	return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
};

template <class T> 
inline const T& rmin ( const T& a, const T& b ) 
{
	return (a<b)?a:b;     // or: return comp(a,b)?a:b; for the comp version
};

template <class T> 
inline const T& rmin ( const T& a, const T& b , const T& c) 
{
	return rmin(a,rmin(b,c));
};

template <class T> 
inline const T& rmax ( const T& a, const T& b , const T& c) 
{
	return rmax(a,rmax(b,c));
};


/**
* Clamp Add Byte
* Adds two byte values and clamps to limit.
*/
inline int cadd(byte a,byte b,int limit)
{
	int r = a + b;
	if ( r > limit) r = limit;
	return r;
}

/**
* Clamp Add Byte
* Adds two byte values and clamps to 255.
*/
inline byte caddb(const byte a,const byte b)
{
	int r = a + b;
	if ( r > 255 ) r = 255;
	return (byte)r;
}

#endif

