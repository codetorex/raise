#ifndef TEXCEPTION_H
#define TEXCEPTION_H

#include "tstring.h"

class Exception
{
public:
	TString Message;

	Exception(const TString& msg)
	{
		Message = msg;
	}
};

class NotImplementedException: public Exception
{
public:
	NotImplementedException(): Exception("Not Implemented")
	{

	}
};

class ObjectException: public Exception
{
public:
	void* Object;

	ObjectException(const TString& msg, void* obj): Exception(msg)
	{
		Object = obj;
	}
};



#endif