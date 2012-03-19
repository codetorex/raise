#ifndef TENCODINGS_H
#define TENCODINGS_H

#include "tutf8encoding.h"
#include "tutf16encoding.h"
#include "tlatin1encoding.h"

class TEncodings
{
public:
	UTF8Encoding UTF8;
	UTF16Encoding UTF16;
	TLatin1Encoding Latin1;
};

extern TEncodings Encodings;

#endif