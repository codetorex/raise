#ifndef WHTTPCONTEXT_H
#define WHTTPCONTEXT_H

#include "whttprequest.h"
#include "whttpresponse.h"

class WHttpContext
{
public:
	WHttpRequest Request;
	WHttpResponse Response;
};

#endif