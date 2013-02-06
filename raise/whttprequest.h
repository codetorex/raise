#ifndef WHTTPREQUEST_H
#define WHTTPREQUEST_H

#include "tstring.h"
#include "tencoding.h"
#include "nuri.h"

enum WHttpRequestMethod
{
	WHTTP_GET,
	WHTTP_POST,
};

class WHttpRequest
{
public:
	string		Method;

	int			ContentLength;
	Encoding*	ContentEncoding;
	string		ContentType;

	NUri		Url;
};

#endif // !WHTTPREQUEST_H
