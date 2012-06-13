#ifndef TSERIALIZABLE_H
#define TSERIALIZABLE_H

#include "ttypeinfo.h"
#include "texception.h"
#include "tarray.h"

/// TODO: these guys should be TTypeInfo too
enum MemberTypes
{
	MT_NONE,
	MT_BOOL,
	MT_CHAR,
	MT_BYTE,
	MT_SHORT,
	MT_WORD,
	MT_INT,
	MT_DWORD,
	MT_FLOAT,
	MT_DOUBLE,
	MT_POINTER,
	MT_OBJECT,
	MT_STRING,
	MT_ARRAY,
};

class TType;
class TFieldInfo;


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

	bool IsClass;
	bool IsTemplate;
	TType* TemplateType;
	TType* BaseClass;

	/// Size that holds in memory
	ui32 Size;

	TType()
	{
		
		BaseClass = 0;
		TemplateType = 0;
		IsTemplate = false;
		IsClass = false;
		IsSimple = false;
		Size = 0;
	}
};

class TSystemTypes
{
public:
	TType ST_Bool;
	
	TType ST_Int8;
	TType ST_UInt8;
	
	TType ST_Int16;
	TType ST_UInt16;
	
	TType ST_Int32;
	TType ST_UInt32;
	
	TType ST_Float;
	TType ST_Double;

	TType ST_Array;
	TType ST_String;
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
		void* trg = GetPointer(object);
		MemoryDriver::ShortCopy(output,trg, FieldType->Size);
	}

	inline void SetValue(void* object, void* input)
	{
		void* trg = GetPointer(object);
		MemoryDriver::ShortCopy(trg,input, FieldType->Size);
	}
};

class TTypeBuilder
{
public:
	TType*	TargetInfo;
	void*	RelativeObject;

	TTypeBuilder(TType* _Target)
	{
		TargetInfo = _Target;
	}

	TTypeBuilder(TType* _Target, void* _RelativeObject)
	{
		TargetInfo = _Target;
		RelativeObject = _RelativeObject;
	}

	inline void SetRelativeObject(void* relObj)
	{
		RelativeObject = relObj;
	}

	inline int GetOffset(void* member)
	{
		return (int)((byte*)member - (byte*)RelativeObject);
	}

	TFieldInfo* SetObjectName(const TString& value)
	{
		TFieldInfo* nmember = new TFieldInfo();
		nmember->ID = TargetInfo->Fields.Count;
		nmember->MemberType = MT_NONE;
		nmember->MemberSubtype = MT_NONE;
		nmember->Offset = -1;
		nmember->Name = value;
		nmember->ObjectInfo = 0;
		TargetInfo->ObjectName = nmember;
		return nmember;
	}

	/**
	* Sets object name from offset. It should be TString.
	*/
	TFieldInfo* SetObjectName(int offset)
	{
		TFieldInfo* nmember = new TFieldInfo();
		nmember->ID = TargetInfo->Fields.Count;
		nmember->MemberType = MT_STRING;
		nmember->MemberSubtype = MT_NONE;
		nmember->Offset = offset;
		nmember->ObjectInfo = 0;
		TargetInfo->ObjectName = nmember;
		return nmember;
	}

	inline TFieldInfo* SetObjectName(TString* tstrptr)
	{
		return SetObjectName(GetOffset(tstrptr));
	}

	/**
	* Adds a member with name.
	*/
	TFieldInfo* AddMember(const TString& name,int offset, MemberTypes typ, MemberTypes subtyp = MT_NONE,TType* minfo = 0)
	{
		TFieldInfo* nmember = new TFieldInfo();
		nmember->ID = TargetInfo->Fields.Count;
		nmember->MemberType = typ;
		nmember->MemberSubtype = subtyp;
		nmember->Name = name;
		nmember->Offset = offset;
		nmember->ObjectInfo = minfo;
		TargetInfo->Fields.Add(nmember);
		return nmember;
	}

	/**
	* Adds a member with name and memberPtr.
	*/
	TFieldInfo* AddMember(const TString& name,void* MemberPtr, MemberTypes typ, MemberTypes subtyp = MT_NONE,TType* minfo = 0)
	{
		int offset = GetOffset(MemberPtr);
		return AddMember(name,offset,typ,subtyp,minfo);
	}

	/**
	* Adds a member with just ID.
	*/
	TFieldInfo* AddMember(int offset, MemberTypes typ, MemberTypes subtyp = MT_NONE,TType* minfo = 0)
	{
		TFieldInfo* nmember = new TFieldInfo();
		nmember->ID = TargetInfo->Fields.Count;
		nmember->MemberType = typ;
		nmember->MemberSubtype = subtyp;
		nmember->Offset = offset;
		nmember->ObjectInfo = minfo;
		TargetInfo->Fields.Add(nmember);
		return nmember;
	}
};

class TSerializable
{
public:
	virtual TType* GetType() = 0;
};

/**
* Serialize interface that serializes the objects.
*/
class TSerializer
{
public:
	virtual void SerializeObject(TType* minfo, void* object) = 0;

	inline void Serialize(TSerializable* obj)
	{
		TType* minfo = obj->GetType();
		SerializeObject(minfo,obj);
	}
};

/**
 * Deserialize interface that deserializes the objects.
 */
class TDeserializer
{
public:
	virtual void DeserializeObject(TType* minfo, void* object) = 0;
	
	inline void Deserialize(TSerializable* obj)
	{
		TType* minfo = obj->GetType();
		DeserializeObject(minfo,obj);
	}
};


#endif