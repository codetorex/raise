#ifndef THASHCODEPROVIDER_H
#define THASHCODEPROVIDER_H

#include "raisetypes.h"

class TString;

/**
 * This class provides a hashing function that can be used for generating DWORD hash for a string.
 * Useful for HashMaps and quick string checks and generic mapping stuff.
 */
class THashCodeProvider
{
public:
	virtual dword GetHashCode(const TString& value) = 0;
};

class TBasicHashCodeProvider: public THashCodeProvider
{
public:

	static TBasicHashCodeProvider Instance;

	dword GetHashCode(const TString& value);
};

#endif