#include "stdafx.h"
#include "tpath.h"

str8 TPath::CurrentFolder;
str8 TPath::TempFolder;

#ifdef WIN32

ch8 TPath::DirectorySeprator = '\\';
ch8 TPath::AltDirectorySeprator = '/';

#else

ch8 TPath::DirectorySeprator = '/';
ch8 TPath::AltDirectorySeprator = '\\';

#endif

