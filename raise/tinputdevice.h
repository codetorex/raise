#ifndef TINPUTDEVICE_H
#define TINPUTDEVICE_H

#include "tstring.h"

class RDLL TInputDevice
{
public:
	TString inputName;
	bool connected;

	TInputDevice()
	{
		connected = false;
	}
};

#endif