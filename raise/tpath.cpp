#include "stdafx.h"
#include "tpath.h"

String TPath::CurrentFolder;
String TPath::TempFolder;

#ifdef WIN32

ch32 TPath::DirectorySeprator = '\\';
ch32 TPath::AltDirectorySeprator = '/';

#else

ch32 TPath::DirectorySeprator = '/';
ch32 TPath::AltDirectorySeprator = '\\';

#endif

