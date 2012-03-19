#ifndef RAISEVERSION_H
#define RAISEVERSION_H

// to get revision from bzr: bzr log --line -r-1
// will result like 26: .....
// so just substring 0 to indexof(':')

#define RAISE_BUILD					291
#define RAISE_BUILD_STRING			"291"

#define RAISE_PHASE					&TVersionPhase::PreAlpha

#define RAISE_DEFAULT_COMPANYNAME	"Doychka"

#include "tapplication.h"

extern TModule RaiseModule;


#endif

