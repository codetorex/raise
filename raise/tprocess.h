#ifndef TPROCESS_H
#define TPROCESS_H


#include "raisetypes.h"
#include "tstring.h"
#include "tbuffer.h"
#include "tevent.h"

// THESE IMPLEMENTATIONS MAYBE NOT SUPPORTED IN LINUX
#ifdef WIN32

class TProcess
{
public:
	DWORD processID;
	HANDLE processHandle;

	void OpenFromWindow(const TString& windowname);
	void CreateFromExecutable(const TString& exepath,const TString& params = TString::Empty);

	TArray<TString*> GetModules();
};


class TProcessMemory: public TProcess
{
public:
	TVirtualBuffer Buffer;

	TProcessMemory(TProcess& othr)
	{
		processID = othr.processID;
		processHandle = othr.processHandle;
		Buffer.Allocate(4 * 1024); // 4 KB
	}

	TProcessMemory()
	{
		Buffer.Allocate(4 * 1024);
	}

	inline TString ReadString(dword address)
	{
		TString result(512);
		Read(address,result.Data,512);
		result.CountCharsAndBytes();
		return result;
	}

	inline TString ReadLongString(dword address, int size)
	{
		TString result(size+1);
		Read(address,result.Data,size);
		result.CountCharsAndBytes();
		return result;
	}

	//TODO: implement wide char string reading functions

	inline dword Read(dword address, void* buffer, int length)
	{
		DWORD readed;
		ReadProcessMemory(processHandle,(void*)address,buffer,length,&readed);
		return readed;
	}

	inline dword Write(dword address, void* buffer, int length)
	{
		DWORD written;
		WriteProcessMemory(processHandle,(void*)address,buffer,length,&written);
		return written;
	}

	inline byte ReadByte(dword address)
	{
		byte data;
		Read(address,&data,1);
		return data;
	}

	inline word ReadWord(dword address)
	{
		word data;
		Read(address,&data,2);
		return data;
	}

	inline dword ReadDWord(dword address)
	{
		dword data;
		Read(address,&data,4);
		return data;
	}

	inline void WriteByte(dword address,byte value)
	{
		Write(address,&value,1);
	}

	inline void WriteWord(dword address,word value)
	{
		Write(address,&value,2);
	}

	inline void WriteDWord(dword address,dword value)
	{
		Write(address,&value,4);
	}

	inline dword FillBuffer(dword address)
	{
		dword readed = Read(address,Buffer.Data,Buffer.Capacity);
		Buffer.SetVirtual(address,address+Buffer.Capacity);
		Buffer.Index = Buffer.Capacity;
		return readed;
	}

	dword Search(dword start,dword end, const void* needle, int length);
	dword SearchPattern(dword start,dword end, const void* needle, const byte* pattern, int length);
};

#endif

#endif