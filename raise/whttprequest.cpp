#include "stdafx.h"
#include "whttprequest.h"





void WHttpRequest::ParseRequest( const TString& request )
{
	TString data(request);
	
	TArray<TString*> lines = data.SplitInplace('\n',true);

	TArray<TString*> requestLineParts = lines[0]->Split(' ',true);


	this->Method = *requestLineParts[0];
	this->RawUrl = *requestLineParts[1];

	int QueryStart = RawUrl.IndexOf("?");
	if (QueryStart > -1)
	{
		TString Query = RawUrl.Substring(QueryStart+1);
		this->QueryString.ParseQueryString(Query);
		this->Path = Query.Substring(0,QueryStart);
	}
	else
	{
		this->Path = RawUrl;
	}

}
