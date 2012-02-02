#ifndef TSERIALIZABLE_H
#define TSERIALIZABLE_H

#include "ttypeinfo.h"
#include "texception.h"
#include "tarray.h"

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

class TMemberInfo;

class TMember
{
public:
	MemberTypes MemberType; // it can be, int float or something or TArray
	MemberTypes MemberSubtype; // if type is array or something, this will be indicate type of array
	TMemberInfo* ObjectInfo; // if anyof types uses MT_OBJECT, then this should be set
	
	int ID;
	int Offset;
	TString Name;

	TString* GetStringPtr(void* object)
	{
		return (TString*)((byte*)object + Offset);
	}

	bool* GetBoolPtr(void* object)
	{
		return (bool*)((byte*)object + Offset);
	}

	int* GetIntPtr(void* object)
	{
		return (int*)((byte*)object + Offset);
	}

	void* GetVoidPtr(void* object)
	{
		return (void*)((byte*)object + Offset);
	}

	void** GetPtrPtr(void* object)
	{
		return (void**)((byte*)object + Offset);
	}
};

/**
* Will hold the object member info.
*/
class TMemberInfo
{
public:
	TMember* ObjectName;
	TArray< TMember* > Members;

	TMemberInfo()
	{
		ObjectName = 0;
	}
};

class TMemberInfoBuilder
{
public:
	TMemberInfo*	TargetInfo;
	void*			RelativeObject;

	TMemberInfoBuilder(TMemberInfo* _Target)
	{
		TargetInfo = _Target;
	}

	TMemberInfoBuilder(TMemberInfo* _Target, void* _RelativeObject)
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

	TMember* SetObjectName(const TString& value)
	{
		TMember* nmember = new TMember();
		nmember->ID = TargetInfo->Members.Count;
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
	TMember* SetObjectName(int offset)
	{
		TMember* nmember = new TMember();
		nmember->ID = TargetInfo->Members.Count;
		nmember->MemberType = MT_STRING;
		nmember->MemberSubtype = MT_NONE;
		nmember->Offset = offset;
		nmember->ObjectInfo = 0;
		TargetInfo->ObjectName = nmember;
		return nmember;
	}

	/**
	* Adds a member with name.
	*/
	TMember* AddMember(const TString& name,int offset, MemberTypes typ, MemberTypes subtyp = MT_NONE,TMemberInfo* minfo = 0)
	{
		TMember* nmember = new TMember();
		nmember->ID = TargetInfo->Members.Count;
		nmember->MemberType = typ;
		nmember->MemberSubtype = subtyp;
		nmember->Name = name;
		nmember->Offset = offset;
		nmember->ObjectInfo = minfo;
		TargetInfo->Members.Add(nmember);
		return nmember;
	}

	/**
	* Adds a member with name and memberPtr.
	*/
	TMember* AddMember(const TString& name,void* MemberPtr, MemberTypes typ, MemberTypes subtyp = MT_NONE,TMemberInfo* minfo = 0)
	{
		int offset = GetOffset(MemberPtr);
		AddMember(name,offset,typ,subtyp,minfo);
	}

	/**
	* Adds a member with just ID.
	*/
	TMember* AddMember(int offset, MemberTypes typ, MemberTypes subtyp = MT_NONE,TMemberInfo* minfo = 0)
	{
		TMember* nmember = new TMember();
		nmember->ID = TargetInfo->Members.Count;
		nmember->MemberType = typ;
		nmember->MemberSubtype = subtyp;
		nmember->Offset = offset;
		nmember->ObjectInfo = minfo;
		TargetInfo->Members.Add(nmember);
		return nmember;
	}
};

/**
* Serialize interface that serializes the objects.
*/
class TSerializer
{
public:
	virtual void Serialize(TMemberInfo* minfo, void* object) = 0;
};

/**
 * Deserialize interface that deserializes the objects.
 */
class TDeserializer
{
public:
	virtual void Deserialize(TMemberInfo* minfo, void* object) = 0;
};


#endif