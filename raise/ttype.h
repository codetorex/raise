#ifndef TTYPE_H
#define TTYPE_H

#include "texception.h"
#include "tarray.h"

class TType;
class TFieldInfo;

/**
 * Objects that should have a definition, or real time information should derive from this
 */
class TObject
{
public:
	virtual TType* GetType() = 0;
};

class TTypeSerializer
{
public:
	virtual TString SerializeString(void* object) = 0;
	virtual void DeserializeString(void* object, TString& output) = 0;

	virtual void SerializeBinary(void* object, byte* output, ui32 outputCapacity) = 0;
	virtual void DeserializeBinary(void* object, byte* input, ui32 inputLength) = 0;
};


/**
* Will hold the object member info.
*/
class TType
{
public:
	TString Name;
	TString FullName;

	TArray< TFieldInfo* > Fields;

	bool IsSimple;

	/// Is type a pointer
	bool IsPointer;
	TType* PointedType;

	/// If its a container or collection set this to true
	bool IsArray;

	/// Is type a class
	bool IsClass;

	/// Template related stuff
	bool IsTemplate;
	TType* TemplateBase;
	TArray< TType* > TemplateArguments;

	/// If this is derived from something else, it should be given in BaseClass
	TType* BaseClass;

	/// Size that holds in memory
	ui32 Size;

	TType()
	{
		InitializeType();
	}

	/**
	 * Just initializes the type
	 */
	TType( const TString& name )
	{
		InitializeType();
		this->Name = name;
		this->FullName = Name;
	}

	/**
	 * Simple system type constructor.
	 */
	TType( const TString& name, ui32 size )
	{
		InitializeType();
		this->Name = name;
		this->FullName = Name;
		this->Size = size;
		IsSimple = true;
	}

	void InitializeType()
	{
		PointedType = 0;
		IsPointer = false;	
		BaseClass = 0;
		IsTemplate = false;
		TemplateBase = 0;
		IsTemplate = false;
		IsClass = false;
		IsArray = false;
		IsSimple = false;
		
		
	}
	
	/// TODO: use type registry here instead creating everytime

	/**
	 * Creates a pointer type of this type
	 */
	TType* CreatePointerType()
	{
		TStringBuilderStack<256> sb;
		sb.Append(Name);
		sb.AppendChar('*');
		TString templateName = sb.ToString();

		TType* tempType = new TType();
		tempType->Name = templateName;
		tempType->FullName = templateName;
		tempType->IsPointer = true;
		tempType->PointedType = this;
		return tempType;
	}

	TType* GetTemplatedType(TType* temType)
	{
		TStringBuilderStack<256> sb;
		sb.Append(Name);
		sb.AppendChar('<');
		sb.Append(temType->Name);
		sb.AppendChar('>');
		TString templateName = sb.ToString();

		TType* tempType = new TType();
		tempType->Name = templateName;
		tempType->FullName = templateName;
		tempType->IsTemplate = true;
		tempType->IsClass = true;
		tempType->TemplateBase = temType;
		tempType->TemplateArguments.Add(temType);

		return tempType;
	}
};


class TFieldInfo
{
public:
	int Offset;
	TType* FieldType;
	TString Name;
	ui32 ID;

	inline void* GetPointer(void* object)
	{
		return (void*)((byte*)object + Offset);
	}

	inline void GetValue(void* object, void* output)
	{
		if (!FieldType->IsSimple)
		{
			throw Exception("Field is not simple");
		}
		void* trg = GetPointer(object);
		MemoryDriver::ShortCopy(output,trg, FieldType->Size);
	}

	inline void SetValue(void* object, void* input)
	{
		if (!FieldType->IsSimple)
		{
			throw Exception("Field is not simple");
		}
		void* trg = GetPointer(object);
		MemoryDriver::ShortCopy(trg,input, FieldType->Size);
	}
};

#endif