#ifndef TENCODING_H
#define TENCODING_H

#include "tencodinginfo.h"

class TEncoding: public TEncodingInfo
{
public:
	static TEncoding* ASCII;
	static TEncoding* Unicode;
	static TEncoding* UTF7;
	static TEncoding* UTF8;
	static TEncoding* UTF32;
	
	int GetByteCount(byte* data,int length);



};

#endif