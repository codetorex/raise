#ifndef TREGISTRY_H
#define TREGISTRY_H

#include "tstring.h"


class TRegistryKey
{
public:
	ui32 Handle;

	String GetName();
	int GetSubKeyCount();
	int ValueCount();


	void Close();
	void Flush();

	Array<String*> GetSubKeyNames();
};

class TRegistry
{
public:

};

#endif