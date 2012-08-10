#include "stdafx.h"
#include "tprocess.h"

#ifdef WIN32

#include "twintools.h"
#include "tthread.h"

void TBreakpoint::SetBreakpoint()
{
	if (!IsSet)
	{
		OrginalByte = Process->Memory.ReadByte((ui32)Address);
		Process->Memory.WriteByte((ui32)Address,0xCC);
		IsSet = true;
	}
}

void TBreakpoint::UnsetBreakpoint()
{
	if (IsSet)
	{
		Process->Memory.WriteByte((ui32)Address,OrginalByte);
		IsSet = false;
	}
}

TBreakpoint::~TBreakpoint()
{
	if (IsSet)
	{
		UnsetBreakpoint();
	}
}

bool TProcessDebug::EnterDebugMode()
{
	IsDebug = DebugActiveProcess(processID);
	if (!IsDebug)
	{
		return false;
	}
	return true;
}

bool TProcessDebug::ExitDebugMode()
{
	IsDebug = DebugActiveProcessStop(processID);
	if (!IsDebug)
	{
		return false;
	}
	return true;
}

TBreakpoint* TProcessDebug::CreateBreakpoint( ui32 addr )
{
	TBreakpoint* tb = GetBreakpoint(addr);
	if (tb == NULL)
	{
		tb = new TBreakpoint((void*)addr,this->Process);
		Breakpoints.Add(tb);
	}
	return tb;
}



TBreakpoint* TProcessDebug::GetBreakpoint( ui32 addr )
{
	int i = Breakpoints.Count;
	while(i--)
	{
		TBreakpoint* tb = Breakpoints.Item[i];
		if ((ui32)tb->Address == addr)
		{
			return tb;
		}
	}

	return NULL;
}

void TProcessDebug::RemoveBreakpoint( TBreakpoint* bp )
{
	delete bp;
	Breakpoints.Remove(bp);
}

void TProcessDebug::RemoveBreakpoints()
{
	int i=Breakpoints.Count;
	while(i--)
	{ 
		TBreakpoint* bp = Breakpoints.Item[i];
		delete bp;
	}
	Breakpoints.Clear();
}


void TProcessDebug::DisableBreakpoints()
{
	int i=Breakpoints.Count;
	while(i--)
	{ 
		TBreakpoint* bp = Breakpoints.Item[i];
		bp->UnsetBreakpoint();
	}
}


void TProcessDebug::EnableBreakpoints()
{
	int i=Breakpoints.Count;
	while(i--)
	{ 
		TBreakpoint* bp = Breakpoints.Item[i];
		bp->SetBreakpoint();
	}
}


void TProcessDebug::BeginDebugging()
{
	DebugThread = new TThread( GetHandler(this, &TProcessDebug::DebuggerLoop ) );
	DebugThread->Start();
}

TProcessDebug::TProcessDebug()
{
	IsDebug = 0;
	DebugThread = NULL;
	QueueExitDebug = false;
}

void TProcessDebug::GetDebuggerPrivileges()
{
	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	// Get the current process token handle...
	if( !OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES| TOKEN_QUERY, &token ))
	{
		throw Exception("Error can't get debug privileges!");
	}

	// Get the LUID for shutdown privilege...
	LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &privileges.Privileges[ 0 ].Luid );

	// Set parameters for AdjustTokenPrivileges...
	privileges.PrivilegeCount = 1;
	privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

	// Enable shutdown privilege...
	AdjustTokenPrivileges( token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0 );
}

void TProcessDebug::EndDebugging()
{
	if ( !ExitDebugMode() )
	{
		DWORD err = GetLastError();
		TString r = TWinTools::ErrorToString(err);
		throw Exception("Failed to leave debug mode: %", sfs(r));
	}
	IsDebug = 0;
	//DebugThread->Join();
}

void TProcessDebug::DebuggerLoop()
{
	DEBUG_EVENT DebugEvent;

	EnterDebugMode();

	while(IsDebug)
	{
		if (WaitForDebugEvent(&DebugEvent, INFINITE))
		{
			if ( DebugEvent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
			{
				IsDebug = false;
				break;
			}

			if ( DebugEvent.dwDebugEventCode == OUTPUT_DEBUG_STRING_EVENT)
			{
				if ( DebugEvent.u.DebugString.fUnicode == 0 )
				{
					char CurrentDebugString[2048];
					Process->Memory.Read((DWORD)DebugEvent.u.DebugString.lpDebugStringData,(byte*)CurrentDebugString,DebugEvent.u.DebugString.nDebugStringLength);
					OnDebugString.call(CurrentDebugString);
				}

				ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
				continue;
			}

			if (DebugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
				{
					OnAccessViolation.call(this,(ui32)DebugEvent.u.Exception.ExceptionRecord.ExceptionAddress);
					ContinueDebugEvent(DebugEvent.dwProcessId,DebugEvent.dwThreadId, DBG_CONTINUE);
					continue;
				}

				CurrentThread = OpenThread( THREAD_GET_CONTEXT | THREAD_SET_CONTEXT,0, DebugEvent.dwThreadId );
				CurrentContext.ContextFlags = CONTEXT_ALL;
				IsContextActive = true;
				GetContext();
				

				if (DebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
				{
					CurrentBreakpointAddr = (ui32)DebugEvent.u.Exception.ExceptionRecord.ExceptionAddress;
					CurrentBreakpoint = GetBreakpoint(CurrentBreakpointAddr); // TODO: 32 bit 64 bit fucks
					if (CurrentBreakpoint != 0)
					{
						CurrentBreakpoint->UnsetBreakpoint();
						CurrentContext.Eip--;
						SetSingleStep();
						
						CurrentBreakpoint->OnBreakpoint.call(CurrentBreakpoint);
						
						SetContext();
					}
					/*PVOID eAddr = DebugEvent.u.Exception.ExceptionRecord.ExceptionAddress;
					if ((DWORD)eAddr == luaLoadFunction )
					{
						gmod.RemoveBreakpoints();
						ctx.Eip--;
						ctx.EFlags |= 0x100;
						SetThreadContext(ThreadHandle,&ctx);
					}
					else
					{
						ContinueDebugEvent(DebugEvent.dwProcessId,DebugEvent.dwThreadId, DBG_CONTINUE);
						continue;
					}

					if (unpatchit)
					{
						//gmod.WriteDword(allowaddr,oldval);
						gmod.WriteByte(targetfck,oldval);
						unpatchit = false;
					}

					CurrentLua.Clear();
					gmod.ReadMemory(ctx.Ebp,(byte*)TempArray,512);
					CurrentLua = TempArray;
					CurrentLua.ToLower();

					if (IsAllowedLua(CurrentLua))
					{
						//allowaddr = ctx.Ecx + 0x40;
						ccolor(0xB,0);
						printf("ALLOWING LUA: %s\n",CurrentLua);
						ccolor(0xA,0);

						oldval = gmod.ReadByte(targetfck);
						gmod.WriteByte(targetfck,1);

						unpatchit = true;
					}
					else
					{
						//ccolor(0xA,0);
						//printf("CHECKING LUA: %s\n",CurrentLua.Chars);
					}*/

					IsContextActive = false;
					ContinueDebugEvent(DebugEvent.dwProcessId,DebugEvent.dwThreadId, DBG_CONTINUE);
					continue;
				}

				if (DebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP)
				{
					GetContext();
					
					if (CurrentBreakpoint != 0)
					{
						CurrentBreakpoint->SetBreakpoint();
					}
					UnsetSingleStep();
					SetContext();

					CurrentBreakpoint = 0;

					IsContextActive = false;
					ContinueDebugEvent(DebugEvent.dwProcessId,DebugEvent.dwThreadId, DBG_CONTINUE);

					if (QueueExitDebug)
					{
						EndDebugging();
					}

					continue;
				}

			}

			//cout << DebugEvent.dwDebugEventCode << " ";

			ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);

		}
		else
		{
			RemoveBreakpoints();
			ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
			ExitDebugMode();
			IsDebug = 0;
			//cout << "Hack deactivated" << NLINE;
		}


		Sleep(1);
	}
	
}


/*dword WINAPI TProcessDebug::DebuggerThread( void* param )
{

}*/


/*
//http://www.logix.cz/michal/doc/i386/chp12-02.htm
int TProcessDebug::SetHardwareBreakpoint( CONTEXT& ctx, int slot, dword addr, dword bptype,dword bpsize )
{
	dword enable = 0x1 << (slot * 2);
	if (ctx.Dr7 & enable != 0)
	{
		return -1; // Slot is already used
	}

	dword opr = ((bpsize << 2) | bptype) << (16 + (slot * 4));
	dword mask = opr | enable;

	switch(slot)
	{
	case 0: ctx.Dr0 = addr; break;
	case 1: ctx.Dr1 = addr; break;
	case 2: ctx.Dr2 = addr; break;
	case 3: ctx.Dr3 = addr; break;
	}

	ctx.Dr7 |= mask;
}

void TProcessDebug::UnsetHardwareBreakpoint( CONTEXT& ctx,int slot )
{
	dword enable = 0x1 << (slot *2);
	ctx.Dr7 &= ~enable;
	}*/


#endif