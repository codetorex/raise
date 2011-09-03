#ifndef TENCODINGINFO_H
#define TENCODINGINFO_H

#include "tstring.h"
#include "tarray.h"

class TEncodingInfo
{
public:
	/// The encoding name
	TString Name;

	/// Encoding name in human readable form.
	TString DisplayName;
	
	/// The minimum bit length per character
	int MinLengthPerChar;

	/// The maximum bit length per character
	int MaxLengthPerChar;

	int CodePage;

	TEncodingInfo()
	{

	}

	/**
	 * @brief Encoding constructor.
	 * @param encName Name of the encoding.
	 * @param _min The minimum bit per character.
	 * @param _max The maximum bit per character.
	 * @param _cp The code page.
	 */
	TEncodingInfo(const TString& encName, const TString& _displayName, int _min, int _max, int _cp)
	{
		Name = encName;
		DisplayName = _displayName;
		MinLengthPerChar = _min;
		MaxLengthPerChar = _max;
		CodePage = _cp;
	}
};


#endif