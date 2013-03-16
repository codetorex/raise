#include "stdafx.h"
#include "nuri.h"
#include "texception.h"

bool NUri::IsUnreserved( ch32 chr )
{
	if (chr >= 'A' && chr <= 'Z')
		return true;

	if (chr >= 'a' && chr <= 'z')
		return true;

	if (chr >= '0' && chr <= '9')
		return true;

	if (chr == '_')
		return true;

	return false;
}


void NUri::Parse( const TString& value )
{
	if ( !value.IsASCII() )
	{
		throw Exception ( "Unicode character in uri" );
	}

	throw NotImplementedException(__FILE__,__LINE__);

	for (int i=0;i<value.Length;i++)
	{
		ch32 curChar = value.Data[i];


	}
}
