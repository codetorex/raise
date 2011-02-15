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

	void OpenFromWindow(const str8& windowname);
	void CreateFromExecutable(const str8& exepath,const str8& params = 0);

	TArray<str16*> GetModules();
};


class TProcessMemory: public TProcess
{
public:
	TVirtualBuffer Buffer;

	TProcessMemory(TProcess& othr)
	{
		processID = othr.processID;
		processHandle = othr.processHandle;
		Buffer.Initialize(4 * 1024); // 4 KB
	}

	TProcessMemory()
	{
		Buffer.Initialize(4 * 1024);
	}

	inline str8 ReadString(dword address)
	{
		str8 result(512);
		Read(address,result.Chars,512);
		result.Length = StringDriver::Length(result.Chars);
		return result;
	}

	inline str8 ReadLongString(dword address, int size)
	{
		str8 result(size+1);
		Read(address,result.Chars,size);
		result.Length = size;
		return result;
	}

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