#ifndef TPROCESSDEBUG_H
#define TPROCESSDEBUG_H

#ifdef WIN32

#include "tprocess.h"
#include "tthread.h"
#include "tprocessmodule.h"

#pragma comment ( lib , "psapi.lib")

class TProcessDebug;

class TBreakpoint
{
public:
	typedef delegate1<void,TBreakpoint*> BreakpointEvent; // void bp1_OnBreakpoint( TBreakpoint* sender )

	void*	Address;
	byte	OrginalByte;
	bool	IsSet; // true if breakpoint is set

	TProcessDebug* Parent;

	TBreakpoint( void* _addr,TProcessDebug* _prn )
	{
		Address = _addr;
		Parent = _prn;
		IsSet = false; 
	}

	~TBreakpoint();

	void	SetBreakpoint();
	void	UnsetBreakpoint();

	void	SetHardwareBreakpoint();
	void	UnsetHardwareBreakpoint();

	event<BreakpointEvent>	OnBreakpoint;
};

namespace HardwareBreakpoint
{
	enum HWBRK_TYPE
	{
		TYPE_CODE = 0,
		TYPE_WRITE = 1,
		TYPE_READWRITE = 3,
	};

	enum HWBRK_SIZE
	{
		SIZE_1 = 0,
		SIZE_2 = 1,
		SIZE_4 = 3,
		//HWBRK_SIZE_8,
	};
}

class TProcessDebug: public TProcessMemory
{
public:
	typedef delegate1<void,char*> DebugStringEvent;
	typedef delegate2<void,TProcessDebug*,dword> GenericDebugEvent; // TProcessDebug* sender, dword address

	TProcessDebug();

	TArray<TBreakpoint*> Breakpoints;

	int IsDebug;

	bool EnterDebugMode();
	bool ExitDebugMode();

	TBreakpoint*	CreateBreakpoint	(dword addr); // returns orginal code
	TBreakpoint*	GetBreakpoint		(dword addr);
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
	dword CurrentBreakpointAddr;



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

// More hacking oriented, like detours and stuff
class TProcessHack: public TProcessDebug
{
public:
	// MODULE LIST
	// THREADS LIST
	// WINDOWS LIST
	// MEMORY MAP
	// DISASSEMBLER

	TProcessHack();

	TProcessMemoryRegion AllMemory;
};

#endif

#endif