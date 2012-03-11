#ifndef TWINTOOLS_H
#define TWINTOOLS_H

#include "tutf16encoding.h"

#ifdef WIN32


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
	 * 12-22-2011: Too dangerous to be used in NServer stuff, since they are threaded I removed this function from existence.
	 * What a shame.
	 */
	/*inline static ch16* SystemString16(const TString& raiseStr)
	{
		Encoding::UTF16.ConvertString(raiseStr,TempSystemString,1024);
		return TempSystemString;
	}*/

	inline static void SystemString16(const TString& raiseStr, ch16* output, int outputSize)
	{
		Encoding::UTF16.ConvertString(raiseStr,output,outputSize);
	}

	inline static TString RaiseString(const ch16* systemStr)
	{
		return Encoding::UTF16.GetString(systemStr);
	}

	inline static TString ErrorToString(DWORD error)
	{
		LPVOID lpMsgBuf;

		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR) &lpMsgBuf,
			0, NULL );

		return TString((char*)lpMsgBuf);
	}

	inline static TString ErrorToStringWithCode(DWORD error)
	{
		return TString::Format("(%) %",sfu(error),sfs(ErrorToString(error)));
	}
};

#endif // WIN32

#endif