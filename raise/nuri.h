#ifndef NURI_H
#define NURI_H

#include "tstring.h"

/// http://en.wikipedia.org/wiki/Percent_encoding
/// http://en.wikipedia.org/wiki/URI_scheme#Generic_syntax

/// WEBREQUEST stuff

/**
 * Similar to TPath but works with web stuff.
 */
class NUri
{
public:
	// THINK LIKE http://www.contoso.com/catalog/shownew.htm?date=today

	String AbsoluteUri; // "http://www.contoso.com/catalog/shownew.htm?date=today"

	String AbsolutePath; // "/catalog/shownew.htm"

	String Host; // "www.contoso.com"

	int Port; // If uri doesnt has the port information, this should be set to default port. for http its 80

	String Query; // "?date=today"

	String Scheme; //  "http"

	String OriginalString; // orginal entry

	String Fragment;

	String UserInfo; // "userName:password"

	inline NUri( const String& value )
	{
		Parse(value);
	}

	NUri()
	{

	}
	
	inline String PathAndQuery();

	void Parse(const String& value);

	inline NUri& operator = ( const String& value)
	{
		Parse(value);
		return *this;
	}

private:
	bool IsUnreserved(ch32 chr);

};

#endif