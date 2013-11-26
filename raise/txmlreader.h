#ifndef TXMLREADER_H
#define TXMLREADER_H

#include "tstring.h"
#include "tdictionary.h"
#include "tstreamreader.h"
#include "tserialization.h"

class TXMLNode: public TDeserializer
{
public:
	String Name;
	String Value;
	
	TDictionary< String* > Attributes;
	Array< TXMLNode* > Nodes;

	TXMLNode* Parent;

	void AddChild(TXMLNode* newChild)
	{
		Nodes.Add(newChild);
		newChild->Parent = this;
	}

	void DeserializeObject(TType* minfo, void* object);

	/**
	 * Currently only supports simple node stuff like y/x/z
	 */
	TXMLNode* SelectSingleNode(const String& xpath);
};

class TXMLReader
{
	String SkipToNodeStart();

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