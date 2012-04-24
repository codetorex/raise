#ifndef TCOMPOSITECONVERTER_H
#define TCOMPOSITECONVERTER_H

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
	* In-place converting virtual function.
	*/
	virtual void Convert(TCompositeBuffer* srcBuffer) = 0;

	/**
	* Uses new buffer for conversion result.
	*/
	virtual void Convert(TCompositeBuffer* srcBuffer,TCompositeBuffer* dstBuffer) = 0;

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