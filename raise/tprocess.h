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

	inline TString ReadString(ui32 address)
	{
		TString result(512);
		Read(address,result.Data,512);
		result.CountCharsAndBytes();
		return result;
	}

	inline TString ReadLongString(ui32 address, int size)
	{
		TString result(size+1);
		Read(address,result.Data,size);
		result.CountCharsAndBytes();
		return result;
	}

	//TODO: implement wide char string reading functions

	inline ui32 Read(ui32 address, void* buffer, int length)
	{
		DWORD readed;
		ReadProcessMemory(processHandle,(void*)address,buffer,length,&readed);
		return readed;
	}

	inline ui32 Write(ui32 address, void* buffer, int length)
	{
		DWORD written;
		WriteProcessMemory(processHandle,(void*)address,buffer,length,&written);
		return written;
	}

	inline byte ReadByte(ui32 address)
	{
		byte data;
		Read(address,&data,1);
		return data;
	}

	inline uint16 ReadWord(ui32 address)
	{
		uint16 data;
		Read(address,&data,2);
		return data;
	}

	inline ui32 ReadDWord(ui32 address)
	{
		ui32 data;
		Read(address,&data,4);
		return data;
	}

	inline void WriteByte(ui32 address,byte value)
	{
		Write(address,&value,1);
	}

	inline void WriteWord(ui32 address,uint16 value)
	{
		Write(address,&value,2);
	}

	inline void WriteDWord(ui32 address,ui32 value)
	{
		Write(address,&value,4);
	}

	inline ui32 FillBuffer(ui32 address)
	{
		ui32 readed = Read(address,Buffer.Data,Buffer.Capacity);
		Buffer.SetVirtual(address,address+Buffer.Capacity);
		Buffer.Index = Buffer.Capacity;
		return readed;
	}

	ui32 Search(ui32 start,ui32 end, const void* needle, int length);
	ui32 SearchPattern(ui32 start,ui32 end, const void* needle, const byte* pattern, int length);
};

#endif

#endif