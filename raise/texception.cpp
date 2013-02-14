#include "stdafx.h"
#include "texception.h"
#include "tplatform.h"

#ifdef WIN32

void SystemErrorWindows::SetErrorMessage( ui32 _oserr )
{
	SetError(_oserr);
	Message = Platform.GetErrorDescription(_oserr);
}

#endif