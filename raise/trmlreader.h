#ifndef TRMLREADER_H
#define TRMLREADER_H

#include "tstring.h"
#include "tdictionary.h"
#include "tstreamreader.h"

class TRMLNode
{
public:
	TString Name;

	TDictionary<TString*> Values;
	//TArray< TKeyValue<TString,TString>* > Values; // similar to attributes in XML
	TArray< TRMLNode* > Nodes;

	TRMLNode()
	{
		Name = "BaseNode";
	}

	TRMLNode(const TString& NodeName)
	{
		Name = NodeName;
	}
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