#ifndef TMANUALRESETEVENT_H
#define TMANUALRESETEVENT_H

#include "tstring.h"

class TResetEvent
{
public:
	HANDLE EventHandle;

	TResetEvent(bool manualReset, bool initialState, const TString& name  )
	{
		
	}
};

class TManualResetEvent: public TResetEvent
{
public:
	

	TManualResetEvent()
	{

	}
};

class TAutoResetEvent: public TResetEvent
{
public:

};

#endif