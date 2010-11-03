#include "stdafx.h"
#include "tpath.h"

#ifdef WIN32

ch8 TPath::DirectorySeprator = '\\';
ch8 TPath::AltDirectorySeprator = '/';

#else

ch8 TPath::DirectorySeprator = '/';
ch8 TPath::AltDirectorySeprator = '\\';

#endif

