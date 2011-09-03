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
};

/**
* Will hold the object member info.
*/
class TMemberInfo
{
public:
	TMember* ObjectName;
	TArray< TMember* > Members;

	/**
	* Sets object name from offset. It should be TString.
	*/
	TMember* SetObjectName(int offset)
	{
		TMember* nmember = new TMember();
		nmember->ID = Members.Count;
		nmember->MemberType = MT_STRING;
		nmember->MemberSubtype = MT_NONE;
		nmember->Offset = offset;
		nmember->ObjectInfo = 0;
		ObjectName = nmember;
		return nmember;
	}

	/**
	* Adds a member with name.
	*/
	TMember* AddMember(const TString& name,int offset, MemberTypes typ, MemberTypes subtyp = MT_NONE,TMemberInfo* minfo = NULL)
	{
		TMember* nmember = new TMember();
		nmember->ID = Members.Count;
		nmember->MemberType = typ;
		nmember->MemberSubtype = subtyp;
		nmember->Name = name;
		nmember->Offset = offset;
		nmember->ObjectInfo = minfo;
		Members.Add(nmember);
		return nmember;
	}

	/**
	* Adds a member with just ID.
	*/
	TMember* AddMember(int offset, MemberTypes typ, MemberTypes subtyp = MT_NONE,TMemberInfo* minfo = NULL)
	{
		TMember* nmember = new TMember();
		nmember->ID = Members.Count;
		nmember->MemberType = typ;
		nmember->MemberSubtype = subtyp;
		nmember->Offset = offset;
		nmember->ObjectInfo = minfo;
		Members.Add(nmember);
		return nmember;
	}

	inline static int GetOffset(void* obj, void* member)
	{
		return (int)((byte*)member - (byte*)obj);
	}
};

/**
* Serialize interface that serializes the objects.
*/
class TSerializer
{
public:
	virtual void Serialize(TMemberInfo* minfo, void* object) = 0;
	virtual void Deserialize(TMemberInfo* minfo, void* object) = 0;
};


#endif