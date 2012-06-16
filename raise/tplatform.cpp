#include "stdafx.h"
#include "tplatform.h"
#include "ttypemanager.h"

TPlatform Platform;

void TPlatform::Initialize()
{
	Types.Initialize();
}