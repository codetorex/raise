#include "stdafx.h"
#include "wactionresult.h"
#include "wcontroller.h"
#include "whttpresponse.h"
#include "tstream.h"
#include "wviewrenderer.h"

void WRedirectResult::PerformResult( WController* c )
{
	TStringBuilder response;
	response.Append("HTTP/1.1 302 Found");
	response.Append("Location: ");
	response.Append(RedirectPath);

	c->Response->OutputStream->Write(response.GetData(),1,response.ByteLength);
}

void WViewResult::PerformResult( WController* c )
{
	Renderer->Render(c,Model);
}


void WJsonResult::PerformResult( WController* c )
{

}

