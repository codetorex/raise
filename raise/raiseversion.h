#ifndef RAISEVERSION_H
#define RAISEVERSION_H

// to get revision from bzr: bzr log --line -r-1
// will result like 26: .....
// so just substring 0 to indexof(':')

#define RAISE_BUILD					429
#define RAISE_BUILD_STRING			"429"

#define RAISE_PHASE					&TVersionPhase::PreAlpha

#define RAISE_DEFAULT_COMPANYNAME	"Doychka"

#define RAISEMODULE					TModule("Raise",TVersion(RAISE_BUILD,RAISE_PHASE), RAISE_DEFAULT_COMPANYNAME)

#define USESRAISE					Application.Modules.Add( new RAISEMODULE )

#endif
