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

	static Array< String > Names;
	static Array< T > Values;

	static String GetName(T val)
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

	static T GetValue( const String& name )
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

	static inline Array< String >& GetNames()
	{
		return Names;
	}

	static inline Array< T >& GetValues()
	{
		return Values;
	}

	inline String ToString()
	{
		GetName(Value);
	}

	inline T Parse(const String& name)
	{
		GetValue(name);
	}
};


#endif