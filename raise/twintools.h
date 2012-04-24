#ifndef TWINTOOLS_H
#define TWINTOOLS_H

#include "tencodings.h"

#ifdef WIN32

#include "texception.h"

/*
 *TODO: Abstractise platform specific functions in single file?
 
 *
 *class TPlatform
{
public:
	virtual void  ShowMessageBox(const TString& message ) = 0;

	virtual void RaiseToSystemString(const TString& message, byte* output, int capacity) = 0;

	virtual TString SystemStringToRaise(byte* output, int length) = 0;

	virtual TString GetErrorString(ui32 ErrorID);

	virtual ui32 CreateThread();

};*/

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

	static void ShowMessage(const TString& str)
	{
		ch16 Temp[4096];
		SystemString16(str,Temp,4096);

		MessageBoxW(0,Temp,L"Message", MB_OK);
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
		Encodings.UTF16.ConvertString(raiseStr,output,outputSize);
	}

	inline static TString RaiseString(const ch16* systemStr)
	{
		return Encodings.UTF16.GetString(systemStr);
	}

	inline static TString ErrorToString(DWORD error)
	{
		ch16 Temp[4096];

		FormatMessageW( 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			Temp,
			4096, NULL );

		return RaiseString(Temp);
	}

	inline static TString ErrorToStringWithCode(DWORD error)
	{
		return TString::Format("(%) %",sfu(error),sfs(ErrorToString(error)));
	}
};

#endif // WIN32

#endif