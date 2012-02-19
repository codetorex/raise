#include "stdafx.h"
#include "texceptionlow.h"
#include "texception.h"
#include "tlog.h"



void LowLevelException( const char* msg )
{
	throw Exception(msg);
}

void LowLevelException( ui16 _file, ui16 _line, ui32 _error, const char* msg )
{
	throw Exception(_file,_line,_error,msg);
}

void LowLevelNotImplemented()
{
	throw NotImplementedException();
}

void LowLevelNotImplemented( ui16 _file, ui16 _line )
{
	throw NotImplementedException(_file,_line);
}

void LowLevelLogWarn( const char* msg )
{
	Log.Output(LG_WRN,msg);
}

void LowLevelLogError( const char* msg )
{
	Log.Output(LG_ERR,msg);
}