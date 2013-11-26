#ifndef TRMLREADER_H
#define TRMLREADER_H

#include "tstring.h"
#include "tdictionary.h"
#include "tstreamreader.h"
#include "tserialization.h"

class TRMLNode: public TDeserializer
{
public:
	String Name;

	TDictionary<String*> Values;
	//TArray< TKeyValue<TString,TString>* > Values; // similar to attributes in XML
	Array< TRMLNode* > Nodes;

	TRMLNode()
	{
		Name = "BaseNode";
	}

	TRMLNode(const String& NodeName)
	{
		Name = NodeName;
	}

	void DeserializeObject(TType* minfo, void* object);
};

class TRMLReader
{
public:
	TStream* BaseStream;
	TStreamReader* TextStream;

	TRMLReader(TStream* readStream);
	~TRMLReader();

	TRMLNode BaseNode;

	void Parse(bool closeStream = true);
};

#endif