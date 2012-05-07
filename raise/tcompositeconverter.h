#ifndef TCOMPOSITECONVERTER_H
#define TCOMPOSITECONVERTER_H

#include "tcompositebuffer.h"

class TCompositeConverter;
class TComposition;
class TCompositeBuffer;

typedef TComposition TBufferFormat;

/**
* Will be interface of converter providers.
*/
class TCompositeConverter
{
public:
	TBufferFormat* SourceFormat; // TODO: use reference here?
	TBufferFormat* DestinationFormat;


	/**
	 * Simple converting mode for direct memory conversion.
	 */
	virtual void Convert(byte* src, byte* dst,int itemCount) = 0;

	/**
	 * In-place converting virtual function.
	 */
	virtual void Convert(TCompositeBuffer* srcBuffer);

	/**
	 * Uses new buffer for conversion result.
	 */
	virtual void Convert(TCompositeBuffer* srcBuffer,TCompositeBuffer* dstBuffer);

	inline void SetSource(TBufferFormat* src)
	{
		SourceFormat = src;
	}

	inline void SetDestination(TBufferFormat* dst)
	{
		DestinationFormat = dst;
	}

	inline void SetDirection(TBufferFormat* src, TBufferFormat* dst)
	{
		SourceFormat = src;
		DestinationFormat = dst;
	}
};



#endif