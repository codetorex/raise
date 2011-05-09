#ifndef TRMLREADER_H
#define TRMLREADER_H

#include "tstring.h"
#include "tdictionary.h"
#include "tstreamreader.h"

class TRMLNode
{
public:
	str8 Name;

	TDictionary<str8*> Values;
	//TArray< TKeyValue<str8,str8>* > Values; // similar to attributes in XML
	TArray< TRMLNode* > Nodes;

	TRMLNode()
	{
		Name = "BaseNode";
	}

	TRMLNode(const str8& NodeName)
	{
		Name = NodeName;
	}
};

class TRMLReader
{
public:
	TStream* BaseStream;
	StreamReader* TextStream;

	TRMLReader(TStream* readStream);
	~TRMLReader();

	TRMLNode BaseNode;

	void Parse(bool closeStream = true);
};

#endif