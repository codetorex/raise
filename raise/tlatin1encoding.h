#ifndef TLATIN1ENCODING_H
#define TLATIN1ENCODING_H

#include "tasciiextended.h"

class TLatin1Encoding: public TASCIIExtendedEncoding
{
public:
	TLatin1Encoding()
	{
		Name = "iso-8859-1";
		DisplayName = "Western European (ISO)";
		CodePage = 28591;
		SetupMapping();
	}

	void SetupMapping()
	{
		for (int i=0;i < 256;i++)
		{
			Mapping[i] = i;
		}
		UpdateUTF8Encoding();
	}
};

#endif