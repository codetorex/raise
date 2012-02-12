#ifndef TEXCEPTIONLOW_H
#define TEXCEPTIONLOW_H

// low level exception throwing support for base classes like string array and stuff like that

#include "raisetypes.h"

void LowLevelException( const char* msg);

void LowLevelException(ui16 _file, ui16 _line, ui32 _error, const char* msg);

void LowLevelNotImplemented();

void LowLevelNotImplemented(ui16 _file, ui16 _line);

#endif