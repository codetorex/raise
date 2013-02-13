#ifndef WMVCHANDLER_H
#define WMVCHANDLER_H

#include "whttphandler.h"
#include "wcontroller.h"
#include "tarray.h"

class WMVCHandler: public WHttpHandler
{
public:

	TArray< WController* > Controllers;


	void ProcessRequest( WHttpContext* ctx ) 
	{
		// 1. Determine which Controller to use
		// 2. Determine action to use
		// 3. Parse request information HttpRequest
		// 4. Route stuff to this place
	}

};

#endif