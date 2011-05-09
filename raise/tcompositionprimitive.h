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

/**
* Single element of a format definition.
*/
class TCompositionPrimitive
{
public:
	str8 Name;
	str8 ShortName;
	int DataType;

	TCompositionPrimitive()
	{
		DataType = tc_notdefined;
	}

	TCompositionPrimitive(const str8& _name, const str8& _short, int _dataType)
	{
		Name = _name;
		ShortName = _short;
		DataType = _dataType;
	}
};

#endif