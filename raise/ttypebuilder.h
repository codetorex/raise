#ifndef TTYPEBUILDER_H
#define TTYPEBUILDER_H

#include "ttype.h"

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

	inline void SetName(const TString& value)
	{
		TargetInfo->Name = value;
		TargetInfo->FullName = value;
	}

	TFieldInfo* AddField(const TString& name, void* offset, TType* fieldType )
	{
		TFieldInfo* field = AddField(offset,fieldType);
		field->Name = name;
		return field;
	}

	TFieldInfo* AddField(void* offset, TType* fieldType )
	{
		TFieldInfo* field = new TFieldInfo();
		field->ID = TargetInfo->Fields.Count;
		field->Offset = GetOffset(offset);
		field->FieldType = fieldType;
		TargetInfo->Fields.Add(field);
		TargetInfo->IsClass = true; // ONLY CLASSES CAN HAVE FIELDS
		return field;
	}

	TFieldInfo* AddTemplateField(void* offset, TType* baseType, TType* templatedType)
	{
		TFieldInfo* field = AddField(offset, baseType->GetTemplatedType(templatedType) );
		return field;
	}

	TFieldInfo* AddTemplateField(const TString& name, void* offset, TType* baseType, TType* templatedType)
	{
		TFieldInfo* field = AddField(name, offset, baseType->GetTemplatedType(templatedType) );
		return field;
	}

};

#endif