#ifndef TEXCEPTION_H
#define TEXCEPTION_H

class TStringFormatElementBase;

class Exception
{
public:
	const char* FileName;
	ui32 Line;

	const char* Message;
	ui32 Code;

	Exception()
	{
		FileName = 0;
		Line = 0;
		Message = 0;
		Code = 0;
	}

	Exception( ui32 code )
	{
		this->Code = code;
		this->Message = 0;
	}

	Exception( const char* message )
	{
		this->Message = message;
	}

	Exception( ui32 code, const char* message )
	{
		this->Message = message;
		this->Code = code;
	}

	Exception(const char* message, TStringFormatElementBase& p0);
	Exception(const char* message, TStringFormatElementBase& p0, TStringFormatElementBase& p1);
	Exception(const char* message, TStringFormatElementBase& p0, TStringFormatElementBase& p1, TStringFormatElementBase& p2);
};

class NotImplementedException: public Exception
{
public:
	NotImplementedException(): Exception("Not Implemented")
	{

	}

	NotImplementedException(const char* fileName, ui32 line)
	{
		this->FileName = fileName;
		this->Line = line;
	}
};

class PlatformException: public Exception
{
public:
	ui32 PlatformErrorID;

	PlatformException()
	{
		Code = 2;
	}
	
	PlatformException(ui32 _oserr);

	PlatformException(const char* msg, ui32 _oserr): Exception(msg)
	{
		Code = 2;
		PlatformErrorID = _oserr;
	}


};

class ObjectException: public Exception
{
public:
	void* Object;

	ObjectException(const char* msg, void* obj): Exception(msg)
	{
		Code = 3;
		Object = obj;
	}
};

#endif