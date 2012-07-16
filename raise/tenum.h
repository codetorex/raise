#ifndef TENUM_H
#define TENUM_H

#include "tdictionary.h"


/**
 * TODO: mgen should generate this
 */
template <class T>
class TEnum
{
public:
	T Value;

	static TArray< TString > Names;
	static TArray< T > Values;

	static TString GetName(T val)
	{
		int index = -1;

		TArrayEnumerator< T > ae(Values);
		while(ae.MoveNext())
		{
			if(ae.Current == Value)
			{
				index = ae.Index;
				break;
			}
		}

		if (index == -1)
		{
			throw Exception("Not found");
		}

		return Names[index];
	}

	static T GetValue( const TString& name )
	{
		int index = -1;

		TArrayEnumerator< T > ae(Names);
		while(ae.MoveNext())
		{
			if(ae.Current == name)
			{
				index = ae.Index;
				break;
			}
		}

		if (index == -1)
		{
			throw Exception("Not found");
		}

		return Values[index];
	}

	static inline TArray< TString >& GetNames()
	{
		return Names;
	}

	static inline TArray< T >& GetValues()
	{
		return Values;
	}

	inline TString ToString()
	{
		GetName(Value);
	}

	inline T Parse(const TString& name)
	{
		GetValue(name);
	}
};


#endif