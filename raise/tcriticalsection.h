#ifndef TCRITICALSECTION_H
#define TCRITICALSECTION_H

#ifdef WIN32

#include "stdafx.h"

class TCriticalSection
{
public:
	CRITICAL_SECTION CSection;


	inline void Finalize()
	{
		DeleteCriticalSection(&CSection);
	}

	inline void Initialize()
	{
		InitializeCriticalSection(&CSection);
	}

	inline void Lock()
	{
		EnterCriticalSection(&CSection);
	}

	inline void Unlock()
	{
		LeaveCriticalSection(&CSection);
	}
};

#else

class TCriticalSection
{
public:
	pthread_mutex_t cs_mutex;

	inline void Finalize()
	{
		cs_mutex = NULL; // is this correct?
	}

	inline void Initialize()
	{
		cs_mutex = PTHREAD_MUTEX_INITIALIZER;
	}

	inline void Lock()
	{
		pthread_mutex_lock( &cs_mutex );
	}

	inline void Unlock()
	{
		pthread_mutex_unlock( &cs_mutex );
	}
};

#endif

#endif

