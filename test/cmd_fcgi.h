#ifndef CMD_FCGI_H
#define CMD_FCGI_H

#define RAISE_FCGI

#ifdef RAISE_FCGI


#include "tcommandline.h"
#include "tapplet.h"
#include "ttestsuiteparameters.h"

#include <fcgi_config.h>

#ifdef WIN32
#include <process.h>
#else
extern char **environ;
#endif // WIN32



#define DLLAPI
#include <fcgiapp.h>

#include "wmvc.h"

class HomeController: public WController
{
public:

	HomeController()
	{
		Name = "Home";
		RegisterAction(new WAction("Index", GetHandler(this, &HomeController::Index)));
		RegisterAction(new WAction("Test", GetHandler(this, &HomeController::Test)));
	}

	WActionResult* Index( WModel* model )
	{
		return View(model);
	}

	WActionResult* Test( WModel* model )
	{
		return NULL;
	}

};


/*

REDIRECT


HTTP/1.1 302 Found
Cache-Control: private
Content-Type: text/html; charset=utf-8
Location: /
Server: Microsoft-IIS/8.0
X-AspNetMvc-Version: 4.0
X-AspNet-Version: 4.0.30319
X-Powered-By: ASP.NET
Date: Thu, 21 Mar 2013 11:26:24 GMT
Content-Length: 118
	  
*/


class TFastCGIApplet: public TApplet
{
public:
	void Run(void* ParameterObj) 
	{
		FCGX_Init();

		int sockfd = FCGX_OpenSocket(":9345", 500);

		FCGX_Request request;

		
		FCGX_InitRequest(&request, sockfd, 0);


		WMVCApplication mvcApp;
		mvcApp.ApplicationPath = "C:\\Library\\Projects\\raise\\bin\\codetorex\\";
		mvcApp.RegisterController(new HomeController());

		while (FCGX_Accept_r(&request) == 0)
		{

			WHttpContext* httpCtx = new WHttpContext();
			
			mvcApp.ProcessRequest(httpCtx);

			FCGX_FPrintF(request.out, "Content-type: text/html\r\n\r\nHello");
			

			

			FCGX_Finish_r(&request);
		}
	}
};

class CmdFastCgi: public TCommandLineHandlerSimple
{
public:
	CmdFastCgi()
	{
		Parameter = "--fastcgi";
		ShortParameter = "-fcgi";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		TTestSuiteParameters* prm = (TTestSuiteParameters*)cmd->OptionObject;
		prm->Applet = new TFastCGIApplet();
	}

} CmdFastCgiHandler;



#endif // RAISE_FCGI

#endif