#ifndef WCONTROLLER_H
#define WCONTROLLER_H


#include "whttprequest.h"
#include "whttpresponse.h"
#include "whttphandler.h"

/**
 * Base class for MVC pattern's controller.
 */
class WController
{
public:
	TString			Name; // controller name

	WHttpContext*	Context;
	WHttpRequest*	Request;
	WHttpResponse*	Response;


	void ProcessRequest( WHttpContext* ctx ) 
	{
		Context = ctx;
		Request = &ctx->Request;
		Response = &ctx->Response;

		// TODO: read query and run the action function
	}
};

#endif