#ifndef TPROCESS_H
#define TPROCESS_H

#include "tbuffer.h"
#include "tevent.h"

// THESE IMPLEMENTATIONS MAYBE NOT SUPPORTED IN LINUX
#ifdef WIN32
#pragma comment ( lib , "psapi.lib")
#include "twindowhandle.h"

class TProcess;
class TProcessMemory;
class TProcessDebug;
class TThread;



class TProcessInfo
{
public:
	DWORD processID;
	HANDLE processHandle;

	void InitializeFrom(const TProcessInfo& othr)
	{
		processID = othr.processID;
		processHandle = othr.processHandle;
	}
};

class TProcessPointerOffset
{
public:
	ui32 Offset;

	TProcessPointerOffset()
	{
		Offset = 0;
	}

	TProcessPointerOffset(ui32 offset)
	{
		Offset = offset;
	}

	TProcessPointerOffset& operator = (ui32 newOffset)
	{
		Offset = newOffset;
		return *this;
	}

	operator ui32()
	{
		return Offset;
	}
};

class TProcessPointer
{
public:
	ui32 Address;

	TProcessPointer()
	{
		Address = 0;
	}

	TProcessPointer(ui32 addr)
	{
		Address = addr;
	}

	TProcessPointer& operator = (ui32 newAddress)
	{
		Address = newAddress;
		return *this;
	}

	operator ui32()
	{
		return Address;
	}
};

/**
 * Represents multilevel offsetted pointer
 */
class TProcessPointerMultilevel
{
public:
	TProcessPointer BasePointer;
	TProcessPointer RealPointer;
	TArrayStack<TProcessPointerOffset,16> Offsets;

	TProcessPointerMultilevel()
	{
	
	}

	TProcessPointerMultilevel(ui32 basePtr)
	{
		BasePointer = basePtr;
	}

	inline void AddOffset(ui32 offset)
	{
		Offsets.Add(offset);
	}
};

class TProcessMemoryRegion
{
public:
	TString ModuleName;
	TString FileName;

	//dword BaseAddress; ?
	ui32 StartAddress;
	ui32 EndAddress;
	ui32 EntryPointAddress;
	ui32 ModuleMemorySize;

	TProcessMemory* Memory;

	TProcessMemoryRegion();
	TProcessMemoryRegion( ui32 _start,ui32 _end,TProcessMemory* prn);
	TProcessMemoryRegion( const TString& _name, ui32 _start,ui32 _end,TProcessMemory* _prn);
	TProcessMemoryRegion( const TString& _name, const TString& _file,ui32 _start,ui32 _end,TProcessMemory* _prn);

	void Initialize(const TString& _name, ui32 _start,ui32 _end,TProcessMemory* _prn);

	/**
	 * Searchs an ascii string.
	 */
	ui32 FindString(const TString& src);

	/**
	 * Finds an unicode (utf-16) string.
	 */
	ui32 FindUnicodeString(const TString& src);
	ui32 FindXref(ui32 addr);
	ui32 FindFunctionBegin(ui32 addr,int intCount = 3);
	ui32 FindFunctionFromString(const TString& src);
	ui32 FindFunctionFromString(const TString& src, byte firstByte);

	ui32 Search( const void* needle, int length);
	ui32 SearchPattern(const void* needle, const byte* pattern, int length);
};

typedef TProcessMemoryRegion TProcessModule;

class TProcessMemory: public TProcessInfo
{
public:
	TVirtualBuffer Buffer;
	TProcess* Process;
	TProcessMemoryRegion MainRegion;

	// ADD MEMORY USAGE FUNCTIONS

	void InitializeMemory(TProcess* pprocess);

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

	inline ui16 ReadWord(ui32 address)
	{
		ui16 data;
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

	inline void WriteWord(ui32 address,ui16 value)
	{
		Write(address,&value,2);
	}

	inline void WriteDWord(ui32 address,ui32 value)
	{
		Write(address,&value,4);
	}

	/**
	 * Returns 1 when everything went smooth
	 * 0 and negative numbers means which offset it got null pointer
	 */
	int GetPointer(TProcessPointerMultilevel& p);

	ui32 GetPointerOffset(ui32 basePtr, ui32 offset1);
	
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

class TBreakpoint
{
public:
	typedef delegate1<void,TBreakpoint*> BreakpointEvent; // void bp1_OnBreakpoint( TBreakpoint* sender )

	void*	Address;
	byte	OrginalByte;
	bool	IsSet; // true if breakpoint is set

	TProcess* Process;

	TBreakpoint( void* _addr,TProcess* _prn )
	{
		Address = _addr;
		Process = _prn;
		IsSet = false; 
	}

	~TBreakpoint();

	void	SetBreakpoint();
	void	UnsetBreakpoint();

	void	SetHardwareBreakpoint();
	void	UnsetHardwareBreakpoint();

	event<BreakpointEvent>	OnBreakpoint;
};

class TBreakpointInt3: public TBreakpoint
{
public:
	byte	OrginalByte;

	/**
	 * these guys should be virtual
	 */
	void	SetBreakpoint();
	void	UnsetBreakpoint();
};

class TBreakpointHardware: public TBreakpoint
{
public:
	enum HWBRK_SIZE
	{
		SIZE_1 = 0,
		SIZE_2 = 1,
		SIZE_4 = 3,
		//HWBRK_SIZE_8,
	};

	enum HWBRK_TYPE
	{
		TYPE_CODE = 0,
		TYPE_WRITE = 1,
		TYPE_READ = 2,
		TYPE_READWRITE = 3,
	};

	int Slot; // SLOT 1-4
	HWBRK_SIZE Size; // 1 2 4 8 how much bytes?
	HWBRK_TYPE Type; // READ WRITE, EXECUTE

	/**
	 * these guys should be virtual
	 */
	void	SetBreakpoint();
	void	UnsetBreakpoint();
};

class TProcessDebug: public TProcessInfo
{
public:
	typedef delegate1<void,char*> DebugStringEvent;
	typedef delegate2<void,TProcessDebug*,ui32> GenericDebugEvent; // TProcessDebug* sender, dword address

	TProcess* Process;

	TProcessDebug();

	TArray<TBreakpoint*> Breakpoints;

	void InitializeDebug(TProcess* pprocess);

	int IsDebug;
	bool QueueExitDebug;

	bool EnterDebugMode();
	bool ExitDebugMode();

	TBreakpoint*	CreateBreakpoint	(ui32 addr); // returns orginal code
	TBreakpoint*	GetBreakpoint		(ui32 addr);
	void			RemoveBreakpoint	(TBreakpoint* bp);
	
	void RemoveBreakpoints();
	void DisableBreakpoints();
	void EnableBreakpoints();


	TThread* DebugThread;
	void BeginDebugging();
	void EndDebugging();

	
	event<DebugStringEvent>		OnDebugString;
	event<GenericDebugEvent>	OnAccessViolation;

	static void GetDebuggerPrivileges();

	void DebuggerLoop();

	bool IsContextActive;
	CONTEXT CurrentContext;
	HANDLE CurrentThread;
	TBreakpoint* CurrentBreakpoint;
	ui32 CurrentBreakpointAddr;



	void SetSingleStep()
	{
		CheckContext();
		CurrentContext.EFlags |= 0x100;
	}

	void UnsetSingleStep()
	{
		CheckContext();
		CurrentContext.EFlags &= ~0x100;
	}

	void SetContext()
	{
		CheckContext();
		SetThreadContext(CurrentThread,&CurrentContext);
	}

	void GetContext()
	{
		CheckContext();
		GetThreadContext(CurrentThread,&CurrentContext); // EBP den script adini alcaz
	}

	//static dword WINAPI DebuggerThread(void* param);
	/*int SetHardwareBreakpoint(CONTEXT& ctx, int slot, dword addr, dword bpsize );
	void UnsetHardwareBreakpoint(CONTEXT& ctx,int slot);*/

private:
	inline void CheckContext()
	{
		if (!IsContextActive)
		{
			throw Exception("Debug function call while context not2 active");
		}
	}
};

// MODULE LIST
// THREADS LIST
// WINDOWS LIST
// MEMORY MAP
// DISASSEMBLER

class TProcess: public TProcessInfo
{
public:
	TProcessMemory Memory;
	TProcessDebug Debug;

	TProcess()
	{

	}

	TProcess(ui32 pid)
	{
		processID = pid;
		processHandle = 0;
	}

	TString GetModulePath();

	ui32 GetBaseAddress();

	/**
	 * Opens handle for process so we can do stuff
	 */
	void OpenProcessHandle();

	void OpenProcessHandle(ui32 access);

	void OpenProcessHandle(bool memoryRead, bool memoryWrite, bool debug = false);

	/**
	 * Call this when you got nothing to do left with this process
	 */
	void CloseProcessHandle();
};

class TProcessManager
{
public:
	static TProcess FindByWindow(const TWindowHandle& winHandle);

	static TProcess FindByWindowName(const TString& windowname);

	static TProcess FindByClassName(const TString& className);

	/**
	 * Enter part of a window title and it will finds process of that window
	 */
	static TProcess FindByWindowNamePart(const TString& windownamePart);

	/**
	 * Part of window name and exe name is required by this function
	 */
	static TProcess FindByWindowNamePartExePathPart(const TString& windownamePart, const TString& exenamePart);

	static TProcess CreateFromExecutable(const TString& exepath,const TString& params = TString::Empty);
};



#endif

#endif