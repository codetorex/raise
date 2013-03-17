#ifndef WCONTROLLER_H
#define WCONTROLLER_H


#include "whttprequest.h"
#include "whttpresponse.h"
#include "whttphandler.h"


#include "tevent.h"

class WModel
{
public:

};

class WAction
{
public:
	typedef delegate1<WActionResult*,WModel*> ActionDelegate;

	TString Name;
	ActionDelegate* ModelAction;

	WAction(const TString& name, ActionDelegate* modelaction )
	{
		this->Name = name;
		ModelAction = modelaction;
	}
};

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

	WAction*		CurrentAction;

	TArray< WAction* > Actions;


	void ProcessRequest( WHttpContext* ctx ) 
	{
		Context = ctx;
		Request = &ctx->Request;
		Response = &ctx->Response;

		// TODO: read query and run the action function
	}

	WActionResult* View()
	{
		
	}
};

#endif