#include "stdafx.h"
#include "tstring.h"

/*#ifdef WIN32

void SystemErrorWindows::SetErrorMessage( ui32 _oserr )
{
	SetError(_oserr);
	Message = Platform.GetErrorDescription(_oserr);
}

#endif
*/

// TODO: implement these

Exception::Exception( const char* message, TStringFormatElementBase& p0 )
{
	this->Message = message;
}

Exception::Exception( const char* message, TStringFormatElementBase& p0, TStringFormatElementBase& p1 )
{
	this->Message = message;
}

Exception::Exception( const char* message, TStringFormatElementBase& p0, TStringFormatElementBase& p1, TStringFormatElementBase& p2 )
{
	this->Message = message;
}

PlatformException::PlatformException( ui32 _oserr )
{
	Code = 2;
	PlatformErrorID = _oserr;
	// TODO: set message from Platform.TranslateError(_oserror)
}
