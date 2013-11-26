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
	String attributeInterrupt = "=\"/>";
	String attributeIgnore = " ";
	String valueInterrupt = "\"";

	String data;
	int interrupt;

	String attributeName;

	while(1)
	{
		data = TextStream->ReadUntil(attributeInterrupt,attributeIgnore,interrupt);

		switch(interrupt)
		{
		case '=':
			attributeName = data;
			break;

		case '"':
			data = TextStream->ReadUntil(valueInterrupt,String::Empty,interrupt);
			node.Attributes.Add(attributeName, new String(data));
			break;

		case '/':
		case '>':
			return interrupt;
		}
	}
}

int TXMLReader::ParseNode( TXMLNode& node )
{
	String nameInterrupt = " />";
	String mainIgnore = "> \t";

	String endInterrupt = ">";
	String endIgnore = "/";

	String data;
	String nodeName;
	int interrupt;
	
	nodeName = TextStream->ReadUntil(nameInterrupt,String::Empty,interrupt);

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

		data = TextStream->ReadUntil(endInterrupt,endIgnore,interrupt);
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
			data = TextStream->ReadUntil(endInterrupt,endIgnore,interrupt);
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

String TXMLReader::SkipToNodeStart()
{
	String mainInterrupt = "<";
	int interrupt;
	return TextStream->ReadUntil(mainInterrupt,String::Empty,interrupt);
}

TXMLNode* TXMLNode::SelectSingleNode( const String& xpath )
{
	if (xpath.IndexOf("/") > -1)
	{
		// split it and find it
		Array< String* > splitResult =  xpath.Split('/');

		TXMLNode* curNode = this;
		for (int i=0;i<splitResult.Count;i++)
		{
			curNode = curNode->SelectSingleNode(*splitResult.Items[i]);
			if (curNode == 0)
			{
				return 0; // not found
			}
		}
		return curNode;
	}

	String find = xpath;
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

void TXMLNode::DeserializeObject( TType* minfo, void* object )
{

}