#include "stdafx.h"
#include "ttimedriver.h"

bool TimeDriver::HighPrecisionSupported = true;

string TimeDriver::LocalStandartName;
string TimeDriver::LocalDaylightName;
int TimeDriver::LocalDaylight;
int TimeDriver::LocalDifference;
ui32 TimeDriver::LocalLastUpdate = 0;