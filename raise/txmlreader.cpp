#include "stdafx.h"
#include "txmlreader.h"


TXMLReader::TXMLReader( TStream* readStream )
{
	BaseStream = readStream;
	TextStream = new TStreamReader ( readStream, (TEncoding&)Encodings.UTF8 );
}

void TXMLReader::Parse( bool closeStream /*= true*/ )
{
	SkipToNodeStart();
	ParseNode(RootNode);

	if (closeStream)
	{
		TextStream->Close();
	}
}

int TXMLReader::ParseAttributes( TXMLNode& node )
{
	TString attributeInterrupt = "=\"/>";
	TString attributeIgnore = " ";
	TString valueInterrupt = "\"";

	TString data;
	int interrupt;

	TString attributeName;

	while(1)
	{
		data = TextStream->ReadInterrupted(attributeInterrupt,attributeIgnore,interrupt);

		switch(interrupt)
		{
		case '=':
			attributeName = data;
			break;

		case '"':
			data = TextStream->ReadInterrupted(valueInterrupt,TString::Empty,interrupt);
			node.Attributes.Add(attributeName, new TString(data));
			break;

		case '/':
		case '>':
			return interrupt;
		}
	}
}

int TXMLReader::ParseNode( TXMLNode& node )
{
	TString nameInterrupt = " />";
	TString mainIgnore = "> \t";

	TString endInterrupt = ">";
	TString endIgnore = "/";

	TString data;
	TString nodeName;
	int interrupt;
	
	nodeName = TextStream->ReadInterrupted(nameInterrupt,TString::Empty,interrupt);

	if (nodeName == "?xml")
	{
		DeclarationNode.Name = nodeName;
		ParseAttributes(DeclarationNode);
		SkipToNodeStart();
		return ParseNode(node);
	}

	node.Name = nodeName;

	if (interrupt == ' ')
	{
		interrupt = ParseAttributes(node);
		if (interrupt == '/') // it just an empty node
		{
			return interrupt;
		}
	}

	data = SkipToNodeStart();
	if (TextStream->Peek() == '/') // do we readed data?
	{
		node.Value = data;

		data = TextStream->ReadInterrupted(endInterrupt,endIgnore,interrupt);
		if (!(data == node.Name))
		{
			throw Exception("Error while reading XML");
		}
		return interrupt;
	}

	//now we should parse the child nodes
	while(1)
	{
		// we got new nodes
		TXMLNode* newNode = new TXMLNode();
		node.AddChild(newNode);
		ParseNode(*newNode);
		data = SkipToNodeStart();
		if (TextStream->Peek() == '/')
		{
			data = TextStream->ReadInterrupted(endInterrupt,endIgnore,interrupt);
			if (!(data == node.Name))
			{
				throw Exception("Error while reading XML");
			}
			return interrupt;
		}
	}

	throw Exception("WTF Error 31");
}

TXMLReader::~TXMLReader()
{

}

TString TXMLReader::SkipToNodeStart()
{
	TString mainInterrupt = "<";
	int interrupt;
	return TextStream->ReadInterrupted(mainInterrupt,TString::Empty,interrupt);
}

TXMLNode* TXMLNode::SelectSingleNode( const TString& xpath )
{
	if (xpath.IndexOf("/") > -1)
	{
		// split it and find it
		ch32 delim = '/';
		TArray<ch32> delimeter(&delim,1);
		TArray< TString* > splitResult =  xpath.Split(delimeter);

		TXMLNode* curNode = this;
		for (int i=0;i<splitResult.Count;i++)
		{
			curNode = curNode->SelectSingleNode(*splitResult.Item[i]);
			if (curNode == 0)
			{
				return 0; // not found
			}
		}
		return curNode;
	}

	TString find = xpath;
	TXMLNode* result = 0;

	TArrayEnumerator<TXMLNode*> ae(Nodes);
	while(ae.MoveNext())
	{
		if (ae.Current->Name == find)
		{
			result = ae.Current;
			break;
		}
	}

	return result;
}

void TXMLNode::Deserialize( TMemberInfo* minfo, void* object )
{

}