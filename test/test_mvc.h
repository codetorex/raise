#ifndef TEST_MVC_H
#define TEST_MVC_H

#include "ttest.h"
#include "wmvc.h"

// LETS DEVELOP THIS BITCH TEST DRIVEN HELL YEAH

class HomeController: public WController
{
public:

	HomeController()
	{
		Actions.Add(new WAction("Index", GetHandler(this, HomeController::Index)));
	}

	WActionResult* Index( WModel* model )
	{
		
	}

};

class TestMVC: public TTestCheck
{
public:
	TestMVC(): TTestCheck("Web MVC check") {};

	void Test()
	{
		TStringBuilder requestHeader;
		requestHeader.AppendLine("GET /Home/Index HTTP/1.1");
		requestHeader.AppendLine("Host: codetorex.com");
		requestHeader.AppendLine("User-Agent: RaiseTest");
		requestHeader.AppendLine();
		TString request = requestHeader.ToString();




	}

} WMVCCheck;


#endif