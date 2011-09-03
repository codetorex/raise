#include "stdafx.h"
#include "ttimedriver.h"

bool TimeDriver::HighPrecisionSupported = true;

string TimeDriver::LocalStandartName;
string TimeDriver::LocalDaylightName;
int TimeDriver::LocalDaylight;
int TimeDriver::LocalDifference;
dword TimeDriver::LocalLastUpdate = 0;