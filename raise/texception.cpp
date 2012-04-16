#include "stdafx.h"
#include "texception.h"
#include "twintools.h"


void SystemErrorWindows::SetErrorMessage( ui32 _oserr )
{
	SetError(_oserr);
	Message = TWinTools::ErrorToStringWithCode(_oserr);
}