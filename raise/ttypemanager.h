#ifndef TTYPEMANAGER_H
#define TTYPEMANAGER_H

#include "ttype.h"

class TTypeManager
{
public:
	TType* ST_Bool;

	TType* ST_Int8;
	TType* ST_UInt8;

	TType* ST_Int16;
	TType* ST_UInt16;

	TType* ST_Int32;
	TType* ST_UInt32;

	TType* ST_Float;
	TType* ST_Double;

	TType* ST_Array;
	TType* ST_String;

	TArray< TType* > TypeRegistry;

	TTypeManager()
	{
		Initialize();
	}

	void Initialize()
	{
		ST_Bool = new TType("bool", sizeof(bool));
		ST_Int8 = new TType("char", sizeof(char));
		ST_UInt8 = new TType("byte", sizeof(byte));
		ST_Int16 = new TType("short", sizeof(i16));
		ST_UInt16 = new TType("word",sizeof(ui16));
		ST_Int32 = new TType("int", sizeof(i32));
		ST_UInt32 = new TType("dword",sizeof(ui32));
		ST_Float = new TType("float", sizeof(float));
		ST_Double = new TType("double", sizeof(double));

		ST_Array = new TType("array");
		ST_Array->IsArray = true;
		ST_Array->IsClass = true;

		ST_String = new TType("string");
		ST_String->IsClass = true;
		ST_String->IsSimple = true; // TODO: HMM NOT LEGIT BUT... REMOVE THIS WHEN YOU IMPLEMENT SERIALIZING CLASS
	}

	TType* GetType(const TString& name)
	{

	}
};

extern TTypeManager Types;

#endif