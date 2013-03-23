#ifndef WMVCHANDLER_H
#define WMVCHANDLER_H

#include "whttphandler.h"
#include "wcontroller.h"
#include "tarray.h"

#include "tfileinfo.h"

class WMVCApplication: public WHttpHandler
{
public:

	TString ApplicationPath; // like /var/www/codetorex.com/

	TArray< WController* > Controllers; // TODO: make this dictionary or hashset?


	WController* RegisterController(WController* controller)
	{
		Controllers.Add(controller);
		controller->MVCApplication = this;
		return controller;
	}

	WController* GetController(const TString& controllerName)
	{
		for (int i=0;i < Controllers.Count;i++)
		{
			WController* curController = Controllers[i];
			if (curController->Name == controllerName)
			{
				return curController;
			}
		}

		return NULL;
	}


	void ProcessRequest( WHttpContext* ctx ) 
	{
		TString result = ApplicationPath + ctx->Request.Path;
		if ( TFileInfo::Exists(result) )
		{
			// STREAM THAT FILE
		}

		TString mvcPath;

		if (ctx->Request.Path == "/")
		{
			mvcPath = "/Home";
		}
		else
		{
			mvcPath = ctx->Request.Path;
		}

		TArray< TString* >  folders = mvcPath.Split('/',true);
		
		if (folders.Count > 0)
		{
			if (folders.Count == 1)
			{
				folders.Add(new TString("Index"));
			}

			TString currentControllerName = *folders[0];
			WController* controller = GetController(currentControllerName);

			if (controller == NULL)
			{
				throw Exception("No controller to handle this request");
			}

			TString currentActionName = *folders[1];

			controller->ProcessRequest(ctx, currentActionName);
		}
		else
		{
			throw Exception("Cant parse MVC path");
			/// WTF amk
		}

	}

};

#endif