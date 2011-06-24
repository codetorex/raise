#ifndef TENCODINGINFO_H
#define TENCODINGINFO_H

#include "tstringfixedwidth.h"
#include "tarray.h"

class TEncodingInfo
{
public:
	/// The encoding name
	str8 Name;

	/// Encoding name in human readable form.
	str8 DisplayName;
	
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
	TEncodingInfo(const str8& encName, const str8& _displayName, int _min, int _max, int _cp)
	{
		Name = encName;
		DisplayName = _displayName;
		MinLengthPerChar = _min;
		MaxLengthPerChar = _max;
		CodePage = _cp;
	}
};


#endif