#ifndef MGENVERSION_H
#define MGENVERSION_H

// to get revision from bzr: bzr log --line -r-1
// will result like 26: .....
// so just substring 0 to indexof(':')

//#include "raiseversion.h"

#define MGEN_BUILD					100
#define MGEN_BUILD_STRING			"100"

#define MGEN_PHASE					&TVersionPhase::PreAlpha

#define MGEN_DEFAULT_COMPANYNAME	"Doycka"

#endif

