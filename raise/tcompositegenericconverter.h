#ifndef TCOMPOSITEGENERICCONVERTER_H
#define TCOMPOSITEGENERICCONVERTER_H

#include "tcompositebuffer.h"

class TCompositeGenericConverter: public TCompositeConverter
{
public:

	virtual void DoConversion(byte* src, byte* dst,int itemCount) = 0;

	void Convert(TCompositeBuffer* srcBuffer)
	{
		int itemCount = srcBuffer->CapacityItem;
		TCompositeBuffer* tmpBuffer = DestinationFormat->CreateBuffer(itemCount);

		DoConversion(srcBuffer->Data,tmpBuffer->Data,itemCount);

		srcBuffer->ExchangeBuffer(tmpBuffer);
		delete tmpBuffer;
	}

	void Convert(TCompositeBuffer* srcBuffer,TCompositeBuffer* dstBuffer)
	{
		int itemCount = srcBuffer->CapacityItem;
		dstBuffer->AllocateItemCapacity(DestinationFormat,itemCount);

		DoConversion(srcBuffer->Data,dstBuffer->Data,itemCount);
	}
};


#endif