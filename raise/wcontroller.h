#ifndef WCONTROLLER_H
#define WCONTROLLER_H


#include "whttprequest.h"
#include "whttpresponse.h"
#include "whttphandler.h"
#include "wactionresult.h"
#include "wfalconrenderer.h"

class WMVCApplication;

/**
 * Base class for MVC pattern's controller.
 */
class WController
{
public:
	TString			Name; // controller name


	WMVCApplication* MVCApplication;

	WHttpContext*	Context;
	WHttpRequest*	Request;
	WHttpResponse*	Response;

	WAction*		CurrentAction;

	TArray< WAction* > Actions; // TODO: make this dictionary or hash map

	void RegisterAction(WAction* action)
	{
		Actions.Add(action);

		TStringBuilder sb;
		sb.Append("/Views/");
		sb.Append(Name);
		sb.AppendChar('/');
		sb.Append(action->Name);
		sb.Append(".rhtml"); // fucking raise html biatch!

		action->ViewPath = sb.ToString();
	}

	WAction* GetAction( const TString& actionName )
	{
		for (int i=0;i<Actions.Count;i++)
		{
			WAction* curAction = Actions[i];
			if (curAction->Name == actionName)
			{
				return curAction;
			}
		}

		return NULL;
	}


	void ProcessRequest( WHttpContext* ctx , const TString& action ) 
	{
		Context = ctx;
		Request = &ctx->Request;
		Response = &ctx->Response;


		CurrentAction = GetAction(action);

		if (CurrentAction == NULL)
		{
			throw Exception("No action to handle this request");
		}

		WModel* curModel = NULL; // TODO: build model from request

		WActionResult* result = CurrentAction->ModelAction->call(curModel);
		
		if (result == NULL)
		{
			throw Exception("Action result is null");
			// wtf panpa WRITE CORRECT CODE
		}

		result->PerformResult(this);
	}

	WActionResult* View(WModel* mdl)
	{
		WActionResult* result = new WViewResult(new WFalconRenderer(), mdl);
		return result;
	}
};

#endif