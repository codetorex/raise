#ifndef TEST_MVC_H
#define TEST_MVC_H

#include "ttest.h"
#include "wmvc.h"

// LETS DEVELOP THIS BITCH TEST DRIVEN HELL YEAH

/*class HomeController: public WController
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

};*/

class TestMVC: public UnitTest
{
public:
	TestMVC(): UnitTest("Web MVC check") {};

	void Execute()
	{
		/*TStringBuilder requestBuilder;
		requestBuilder.AppendLine("GET /Home/Index HTTP/1.1");
		requestBuilder.AppendLine("Host: codetorex.com");
		requestBuilder.AppendLine("User-Agent: RaiseTest");
		requestBuilder.AppendLine();
		TString requestString = requestBuilder.ToString();

		WHttpContext httpContext;
		httpContext.Request.ParseRequest(requestString);


		WMVCApplication mvcApp;
		mvcApp.ApplicationPath = "C:\\Library\\Projects\\raise\\bin\\codetorex\\";
		mvcApp.RegisterController(new HomeController());

		mvcApp.ProcessRequest(&httpContext);*/
	}

} WMVCCheck;


#endif