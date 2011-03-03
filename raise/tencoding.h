#ifndef TENCODING_H
#define TENCODING_H

#include "tstring.h"

class TEncoding
{
public:
	str8 Name;
	
	int MinLengthPerChar;
	int MaxLengthPerChar;
	
	int CodePage;

	static TEncoding* ASCII;
	static TEncoding* Unicode;
	static TEncoding* UTF8;

	static byte* Convert(TEncoding* srcEncoding, TEncoding* dstEncoding, byte* bytes,int length);

	TEncoding(int _codePage);

	TEncoding(const str8& encName, int _min, int _max, int _cp)
	{
		Name = encName;
		MinLengthPerChar = _min;
		MaxLengthPerChar = _max;
		CodePage = _cp;
	}
};


#endif