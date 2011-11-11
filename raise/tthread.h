#ifndef TTHREAD_H
#define TTHREAD_H

#include "tstring.h"
#include "tevent.h"

class TThread
{
public:

	enum Priorities
	{
		p_Lowest,
		p_BelowNormal,
		p_Normal,
		p_AboveNormal,
		p_Highest
	};

	enum States
	{

	};

	TString		Name;
	bool		IsAlive;
	int			ThreadState;
	Priorities	Priority;
	ui32		ID;
	int			MaximumStackSize;
	bool		IsParamterized;
	HANDLE		ThreadHandle;
	void*		ParameterObject;

	typedef delegate0<void> ThreadStart;
	typedef delegate1<void,void*> ParameterizedThreadStart;

	union
	{
		ThreadStart*				TStart;
		ParameterizedThreadStart*	PStart;
	};

	TThread(ThreadStart* start,int maxStackSize = 0)
	{
		TStart = start;
		MaximumStackSize = maxStackSize;
		IsParamterized = false;
		IsAlive = false;
	}

	TThread(ParameterizedThreadStart* start, int maxStackSize = 0)
	{
		PStart = start;
		MaximumStackSize = maxStackSize;
		IsParamterized = true;
		IsAlive = false;
	}

	static ui32 get_CurrentThreadID();

	void SetPriority(Priorities newPriority)
	{

	}

	void Join(ui32 timeout = INFINITE)
	{

	}

	void Interrupt()
	{

	}

	void Abort()
	{

	}

	void Start();

	inline void Start(void* parameter)
	{
		ParameterObject = parameter;
		Start();
	}
};

#endif