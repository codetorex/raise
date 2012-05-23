#ifndef TXMLREADER_H
#define TXMLREADER_H

#include "tstring.h"
#include "tdictionary.h"
#include "tstreamreader.h"
#include "tserializable.h"

class TXMLNode: public TDeserializer
{
public:
	TString Name;
	TString Value;
	
	TDictionary< TString* > Attributes;
	TArray< TXMLNode* > Nodes;

	TXMLNode* Parent;

	void AddChild(TXMLNode* newChild)
	{
		Nodes.Add(newChild);
		newChild->Parent = this;
	}

	void Deserialize(TMemberInfo* minfo, void* object);
};

class TXMLReader
{
	TString SkipToNodeStart();

public:
	TStream* BaseStream;
	TStreamReader* TextStream;

	TXMLReader(TStream* readStream);
	~TXMLReader();

	TXMLNode DeclarationNode;

	TXMLNode RootNode;

	void Parse(bool closeStream = true);

	int ParseAttributes(TXMLNode& node);

	/**
	 * Call this function if you encounter <
	 */
	int ParseNode(TXMLNode& node);
};

#endif