#ifndef TEXCEPTION_H
#define TEXCEPTION_H

//#include "raisetypes.h"
#include "tstring.h"

class Exception
{
public:
	union
	{
		struct
		{
			ui16 FileID;
			ui16 Line;
		};

		ui32 Codepoint;
	};
	

	ui32		ErrorID;
	TString		Message;

	Exception()
	{
		Codepoint = 0;
		ErrorID = 0;
	}

	Exception(const TString& _msg)
	{
		Codepoint = 0;
		ErrorID = 0;
		Message = _msg;
	}

	Exception(const TString& msg, sfp arg0)
	{
		Codepoint = 0;
		ErrorID = 0;
		Message = TString::Format(msg,arg0);
	}

	Exception(const TString& msg, sfp arg0, sfp arg1)
	{
		Codepoint = 0;
		ErrorID = 0;
		Message = TString::Format(msg,arg0,arg1);
	}

	Exception(const TString& msg, sfp arg0, sfp arg1, sfp arg2)
	{
		Codepoint = 0;
		ErrorID = 0;
		Message = TString::Format(msg,arg0,arg1,arg2);
	}

	Exception(ui32 _error, const TString& _msg)
	{
		Codepoint = 0;
		ErrorID = _error;
		Message = _msg;
	}

	Exception(ui16 _file, ui16 _line, const TString& _msg)
	{
		FileID = _file;
		Line = _line;
		ErrorID = 0;
		Message = _msg;
	}

	Exception(ui16 _file, ui16 _line, ui32 _error, const TString& _msg)
	{
		FileID = _file;
		Line = _line;
		ErrorID = _error;
		Message = _msg;
	}


};

/*class ExceptionMessage: public Exception
{
public:
	TString Message;

	ExceptionMessage(const TString& msg)
	{
		Message = msg;
	}
};*/

class NotImplementedException: public Exception
{
public:
	NotImplementedException(): Exception("Not Implemented")
	{
		ErrorID = 1;
	}

	NotImplementedException(ui16 _file, ui16 _line): Exception(_file,_line,"Not Implemented")
	{
		ErrorID = 1;
	}
};

class OSException: public Exception
{
public:
	ui32 OSErrorID;

	OSException(ui32 _oserr)
	{
		ErrorID = 2;
		OSErrorID = _oserr;
	}

	OSException(const char* msg, ui32 _oserr): Exception(msg)
	{
		ErrorID = 2;
		OSErrorID = _oserr;
	}
};

class ObjectException: public Exception
{
public:
	void* Object;

	ObjectException(const char* msg, void* obj): Exception(msg)
	{
		ErrorID = 3;
		Object = obj;
	}
};



#endif