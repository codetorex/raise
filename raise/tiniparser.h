#ifndef TINIPARSER_H
#define TINIPARSER_H

#include "tstreamreader.h"
#include "tstring.h"
#include "thashmap.h"

// TODO: use dictionary in future?

class TINIClass
{
public:
	String Name;

	THashMap<String*> Variables;

	TINIClass(const String& _name)
	{
		Name = _name;
	}
};

class TINIParser
{
public:
	TStream* BaseStream;
	TTextReader* TextStream;
	THashMap<TINIClass*> Classes;

	TINIClass* CurrentClass;

	TINIParser();
	TINIParser(TStream* readStream);
	~TINIParser();

	// This parameter will set Class keys and Variable keys to lowercase. For avoiding HashMap's missing values.
	bool LowerCaseKeys;

	void SetSource(TStream* readStream);
	void Parse(bool closeStream = true);

	String* GetValue(const String& className, const String& key);

	/**
	* @return TINIClass if available, throws if not.
	*/
	inline TINIClass* GetClass(const String& className)
	{
		return Classes.GetValue(className);
	}

	TINIClass* FindClass(const String& className)
	{
		TINIClass* result;
		if (LowerCaseKeys)
		{
			String lowered = className.ToLower();
			result = Classes.GetValue(lowered);
		}
		else
		{
			result = Classes.GetValue(className);
		}
		return result;
	}

	inline bool ContainsClass(const String& className)
	{
		return Classes.ContainsKey(className);	
	}
};

#endif