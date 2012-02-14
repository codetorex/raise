#ifndef MGENELEMENTS_H
#define MGENELEMENTS_H

#include "tstring.h"
#include "tarray.h"

class MGenDataType
{
public:
	TString			DataType;

	bool			IsInteger; // if true then integer or floating
	bool			Signed; // true when data is signed
	int				BitSize;
};

class MGenClassMember
{
public:
	ui32			Flags;
	TString			ClassMemberName;
	MGenDataType*	DataType;
};

class MGenFunctionParameter
{
public:
	ui32			Flags;
	TString			ParameterName;
	MGenDataType*	DataType;
};

class MGenFunction
{
public:
	ui32			Flags;
	TString			FunctionName;
	MGenDataType*	ReturnType;

	TArray<MGenFunctionParameter*>	Parameters;
};

class MGenClass
{
public:
	TString			ClassName;
	ui32			Type; // if network packet or class?

	ui32			TotalBitLength;
	ui32			TotalByteLength;

	TArray<MGenClassMember*>	Members;

	TArray<MGenFunction*>		Functions;

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
	TString			ItemName;
	ui32			Value;
};

class MGenEnum
{
public:
	TString					EnumName;

	TArray<MGenEnumItem*>	Items;
};


#endif