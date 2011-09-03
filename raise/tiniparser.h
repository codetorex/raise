#ifndef TINIPARSER_H
#define TINIPARSER_H

#include "tstreamreader.h"
#include "tstring.h"
#include "thashmap.h"

// TODO: use dictionary in future?

class TINIClass
{
public:
	TString Name;

	THashMap<TString*> Variables;

	TINIClass(const TString& _name)
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

	TString* GetValue(const TString& className, const TString& key);

	/**
	* @return TINIClass if available, null if not.
	*/
	TINIClass* GetClass(const TString& className)
	{
		return Classes.GetValue(className);
	}

	bool ContainsClass(const TString& className)
	{
		return Classes.ContainsKey(className);	
	}
};

#endif