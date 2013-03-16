#ifndef TTESTSUITEPARAMTERS_H
#define TTESTSUITEPARAMTERS_H

#include "tapplet.h"
#include "tstring.h"

class TTestSuiteParameters
{
public:
	TApplet* Applet;

	TString ExportLibraryName;
	TString OutputFile;

	TString ExePath;

};


#endif