#ifndef MGENELEMENTS_H
#define MGENELEMENTS_H

#include "tstring.h"
#include "tarray.h"

class MGenDataType
{
public:
	String			DataType;

	bool			IsInteger; // if true then integer or floating
	bool			Signed; // true when data is signed
	int				BitSize;
};

class MGenClassMember
{
public:
	ui32			Flags;
	String			ClassMemberName;
	MGenDataType*	DataType;
};

class MGenFunctionParameter
{
public:
	ui32			Flags;
	String			ParameterName;
	MGenDataType*	DataType;
};

class MGenFunction
{
public:
	ui32			Flags;
	String			FunctionName;
	MGenDataType*	ReturnType;

	Array<MGenFunctionParameter*>	Parameters;
};

class MGenClass
{
public:
	String			ClassName;
	ui32			Type; // if network packet or class?

	ui32			TotalBitLength;
	ui32			TotalByteLength;

	Array<MGenClassMember*>	Members;

	Array<MGenFunction*>		Functions;

	void AddMember(MGenClassMember* member)
	{
		Members.Add(member);
		TotalBitLength += member->DataType->BitSize;
		TotalByteLength = TotalBitLength / 8;
	}
};

class MGenEnumItem
{
public:
	String			ItemName;
	ui32			Value;
};

class MGenEnum
{
public:
	String					EnumName;

	Array<MGenEnumItem*>	Items;
};


#endif