#include "stdafx.h"
#include "texception.h"
#include "twintools.h"

#ifdef WIN32

void SystemErrorWindows::SetErrorMessage( ui32 _oserr )
{
	SetError(_oserr);
	Message = TWinTools::ErrorToStringWithCode(_oserr);
}

#endif