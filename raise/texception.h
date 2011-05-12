#ifndef TEXCEPTION_H
#define TEXCEPTION_H

#include "tstringfixedwidth.h"

class Exception
{
public:
	str8 Message;

	Exception(const str8& msg)
	{
		Message = msg;
	}
};

class ObjectException: public Exception
{
public:
	void* Object;

	ObjectException(const str8& msg, void* obj): Exception(msg)
	{
		Object = obj;
	}
};



#endif