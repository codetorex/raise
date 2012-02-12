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

#include <pthread.h>

class TCriticalSection
{
public:
	pthread_mutex_t cs_mutex;

	inline void Finalize()
	{
		pthread_mutex_destroy(&cs_mutex);
	}

	inline void Initialize()
	{
		cs_mutex = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_init(&cs_mutex,NULL);
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

