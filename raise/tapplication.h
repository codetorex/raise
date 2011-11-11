#ifndef TAPPLICATION_H
#define TAPPLICATION_H

#include "tstring.h"

class TCultureInfo;

class TApplication
{
public:
	TString Name;
	TString Version;
	TString Company;
	
	TString ExecutablePath;
	TString StartupPath;

	static TCultureInfo& CurrenctCulture();

	void Exit();
};

extern TApplication Application;

/*
class TModule
{
public:

};
*/

#endif