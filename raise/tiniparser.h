#ifndef TINIPARSER_H
#define TINIPARSER_H

#include "tstreamreader.h"
#include "tstring.h"
#include "thashmap.h"

// TODO: use dictionary in future?

class TINIClass
{
public:
	str8 Name;

	THashMap<str8*> Variables;

	TINIClass(const str8& _name)
	{
		Name = _name;
	}
};

class TINIParser
{
public:
	TStream* BaseStream;
	TStreamReader* TextStream;
	THashMap<TINIClass*> Classes;

	TINIClass* CurrentClass;

	TINIParser();
	TINIParser(TStream* readStream);
	~TINIParser();

	void SetSource(TStream* readStream);
	void Parse(bool closeStream = true);

	str8* GetValue(const str8& className, const str8& key);

	/**
	* @return TINIClass if available, null if not.
	*/
	TINIClass* GetClass(const str8& className)
	{
		return Classes.GetValue(className);
	}

	bool ContainsClass(const str8& className)
	{
		return Classes.ContainsKey(className);	
	}
};

#endif