#ifndef TCOMPOSITIONPRIMITIVE_H
#define TCOMPOSITIONPRIMITIVE_H

#include "raisetypes.h"
#include "tstring.h"


enum TPrimitiveTypes
{
	tc_byte,
	tc_short,
	tc_dword,
	tc_float,
	tc_double,
	tc_bit1,
	tc_bit2,
	tc_bit3,
	tc_bit4,
	tc_bit5,
	tc_bit6,
	tc_bit7,
	tc_bit8,
	tc_group,
	tc_float24,
	tc_notdefined,
};

extern char* PrimitiveNames[];

/**
* Single element of a format definition.
*/
class TCompositionPrimitive
{
public:
	TString Name;
	TString ShortName;
	int DataType;

	
	inline void Set(const TString& _name, const TString& _short, int _dataType)
	{
		Name = _name;
		ShortName = _short;
		DataType = _dataType;
	}
	
	TCompositionPrimitive()
	{
		DataType = tc_notdefined;
	}

	TCompositionPrimitive(const TString& _name, const TString& _short, int _dataType)
	{
		Set(_name,_short,_dataType);
	}
};

#endif