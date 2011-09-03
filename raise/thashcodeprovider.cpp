#include "stdafx.h"
#include "thashcodeprovider.h"

#include "tstring.h"

TBasicHashCodeProvider TBasicHashCodeProvider::Instance;

dword TBasicHashCodeProvider::GetHashCode( const TString& value )
{
	register dword hash = 0;
	int i = value.ByteLength;
	while(i--)
	{
		hash += value.Data[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}