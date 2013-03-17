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

};

class WHttpRequest
{
public:
	string		Method;

	int			ContentLength;
	Encoding*	ContentEncoding;
	string		ContentType;
	
	NameValueCollection Cookies;

	NUri		Url;

	/**
	 * Parses request information from incoming packet
	 */
	void ParseRequest(NPacketReader* pck);

	/**
	 * Creates request packet from given information
	 */
	void CreateRequest(TArray<byte> data);


};

#endif // !WHTTPREQUEST_H
