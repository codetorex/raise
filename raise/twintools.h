#ifndef TWINTOOLS_H
#define TWINTOOLS_H

#include "tutf16encoding.h"

class TWinTools
{
public:
	static void ShowMessage(const char* fmt,...)
	{
		char Temp[4096];

		va_list ap;
		va_start(ap,fmt);
		vsprintf_s(Temp,4096,fmt,ap);
		va_end(ap);

		MessageBoxA(0,Temp,"Message",MB_OK);
	}

	static ch16 TempSystemString[1024];

	/**
	 * Warning will problematic in multi thread. (Maybe made more dangerous by multiple buffers for threads. )
	 */
	inline static ch16* SystemString16(const TString& raiseStr)
	{
		Encoding::UTF16.ConvertString(raiseStr,TempSystemString,1024);
		return TempSystemString;
	}

	inline static TString RaiseString(const ch16* systemStr)
	{
		return Encoding::UTF16.GetString(systemStr);
	}
};

#endif