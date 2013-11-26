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

	void ParseQueryString(const String& queryString)
	{

	}

	String ToString()
	{

	}
};

class TStream;

class WHttpRequest
{
public:
	TStream*	InputStream;

	String		Method;

	int			ContentLength;
	Encoding*	ContentEncoding;
	String		ContentType;

	String		RawUrl;
	String		Path;
	
	String		UserAgent;

	NameValueCollection Cookies;
	WQueryString QueryString;

	NUri		Url;

	/**
	 * Parses request information from incoming packet
	 */
	void ParseRequest(const String& request);

	/**
	 * Creates request packet from given information
	 */
	void CreateRequest(Array<byte> data);


};

#endif // !WHTTPREQUEST_H
