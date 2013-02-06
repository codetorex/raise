#ifndef WHTTPHANDLER_H
#define WHTTPHANDLER_H


#include "whttpcontext.h"

class WHttpHandler
{
public:
	virtual void ProcessRequest(WHttpContext* ctx) = 0;
};


#endif