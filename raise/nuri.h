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

	TString AbsoluteUri; // "http://www.contoso.com/catalog/shownew.htm?date=today"

	TString AbsolutePath; // "/catalog/shownew.htm"

	TString Host; // "www.contoso.com"

	int Port; // If uri doesnt has the port information, this should be set to default port. for http its 80

	TString Query; // "?date=today"

	TString Scheme; //  "http"

	TString OriginalString; // orginal entry

	TString Fragment;

	TString UserInfo; // "userName:password"

	inline NUri( const TString& value )
	{
		Parse(value);
	}

	NUri()
	{

	}
	
	inline TString PathAndQuery();

	void Parse(const TString& value);

	inline NUri& operator = ( const TString& value)
	{
		Parse(value);
		return *this;
	}

private:
	bool IsUnreserved(ch32 chr);

};

#endif