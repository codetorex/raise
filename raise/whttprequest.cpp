#include "stdafx.h"
#include "whttprequest.h"





void WHttpRequest::ParseRequest( const String& request )
{
	String data(request);
	
	Array<String*> lines = data.SplitInplace('\n',true);

	Array<String*> requestLineParts = lines[0]->Split(' ',true);


	this->Method = *requestLineParts[0];
	this->RawUrl = *requestLineParts[1];

	int QueryStart = RawUrl.IndexOf("?");
	if (QueryStart > -1)
	{
		String Query = RawUrl.Substring(QueryStart+1);
		this->QueryString.ParseQueryString(Query);
		this->Path = Query.Substring(0,QueryStart);
	}
	else
	{
		this->Path = RawUrl;
	}

}
