#ifndef TTESTSUITEPARAMTERS_H
#define TTESTSUITEPARAMTERS_H

#include "tapplet.h"
#include "tstring.h"

class TTestSuiteParameters
{
public:
	TApplet* Applet;

	String ExportLibraryName;
	String OutputFile;

	String ExePath;

};


#endif