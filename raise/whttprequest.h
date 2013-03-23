#ifndef WHTTPREQUEST_H
#define WHTTPREQUEST_H

#include "tstring.h"
#include "tencoding.h"
#include "nuri.h"
#include "tdictionary.h"

#include "npacketreader.h"
#include "npacketwriter.h"

enum WHttpRequestMethod
{
	WHTTP_GET,
	WHTTP_POST,
};

class NameValueCollection
{
public:
	//TArray < TKeyValueArray<TString, TString> > NameValueArray;
};

class WQueryString: public NameValueCollection
{
public:

	void ParseQueryString(const TString& queryString)
	{

	}

	TString ToString()
	{

	}
};

class TStream;

class WHttpRequest
{
public:
	TStream*	InputStream;

	string		Method;

	int			ContentLength;
	Encoding*	ContentEncoding;
	string		ContentType;

	string		RawUrl;
	string		Path;
	
	string		UserAgent;

	NameValueCollection Cookies;
	WQueryString QueryString;

	NUri		Url;

	/**
	 * Parses request information from incoming packet
	 */
	void ParseRequest(const TString& request);

	/**
	 * Creates request packet from given information
	 */
	void CreateRequest(TArray<byte> data);


};

#endif // !WHTTPREQUEST_H
