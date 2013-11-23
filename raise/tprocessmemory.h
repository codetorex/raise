#ifndef TPROCESSMEMORY_H
#define TPROCESSMEMORY_H

#include "tbuffer.h"

class TProcess;
class TProcessMemory2;

class TProcessMemoryRegion2
{
public:
	TProcessMemory2* Memory;

	ui64 Start;
	ui64 Length;
	ui64 End;

	ui32 State;
	ui32 Protect;
	ui32 Type;

	TString Path;

	TProcessMemoryRegion2()
	{
		Start = Length = End = 0;
		State = Protect = Type = 0;
	}

	TProcessMemoryRegion2(ui64 start, ui64 length, TProcessMemory2* mem)
	{
		this->Start = start;
		this->Length = length;
		this->End = start + length;
		this->Memory = mem;
	}
};

/**
 * New version
 */
class TProcessMemory2: public TProcessInfo
{
public:
	TProcess* Process;

	TArray< TProcessMemoryRegion2* > Modules;
	TArray< TProcessMemoryRegion2* > Regions;
	ui64 CommitSize;
	int CommitCount;
	ui64 TotalSize;

	void InitializeMemory2(TProcess* process);

	void UpdateMemoryRegions();

	void UpdateModules();


	inline int Read( ui64 src, void* dst, ui64 length)
	{
		DWORD readed;
		ReadProcessMemory(processHandle,(void*)src,dst,length,&readed);
		return readed;
	}

	template <class T>
	inline T Read( ui64 src )
	{
		DWORD readed;
		T result;
		ReadProcessMemory(processHandle,(void*)src,&result, sizeof(T),&readed);
		if (readed == sizeof(T))
		{
			return result;
		}
		else
		{
			throw Exception("Read failed");
		}
	}

	template <class T>
	inline T Write( ui64 src, T value )
	{
		DWORD written;
		WriteProcessMemory(processHandle,(void*)src,&value, sizeof(T),&written);
		if (written == sizeof(T))
		{
			return written;
		}
		else
		{
			throw Exception("Write failed");
		}
	}

	float	ReadFloat( ui64 src ) { return Read<float>(src); }
	int		ReadInt32( ui64 src ) { return Read<int>(src); }
	ui32	ReadUInt32( ui64 src ) { return Read<ui32>(src); }
	ui16	ReadUInt16( ui64 src ) { return Read<ui16>(src); }
	i16		ReadInt16( ui64 src ) { return Read<i16>(src); }
	byte	ReadByte( ui64 src ) { return Read<byte>(src); }

	int		FindAll( const void* needle, int length, TArray< ui64 >& result );
	int		FindAll( ui64 start, ui64 end, const void* needle, int length, TArray< ui64 >& result );

	int		FindAll( float needle, float epsilon, TArray< ui64 >& result );
	int		FindAll( ui64 start, ui64 end, float needle, float epsilon, TArray< ui64 >& result );
};




#endif