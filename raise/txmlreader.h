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

	void DeserializeObject(TMemberInfo* minfo, void* object);

	/**
	 * Currently only supports simple node stuff like y/x/z
	 */
	TXMLNode* SelectSingleNode(const TString& xpath);
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