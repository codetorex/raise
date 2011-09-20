#ifndef TREGISTRY_H
#define TREGISTRY_H

#include "tstring.h"


class TRegistryKey
{
public:
	ui32 Handle;

	string GetName();
	int GetSubKeyCount();
	int ValueCount();


	void Close();
	void Flush();

	TArray<string*> GetSubKeyNames();
};

class TRegistry
{
public:

};

#endif