#include "stdafx.h"
#include "trmlreader.h"
#include "tstack.h"

TRMLReader::TRMLReader( TStream* readStream )
{
	BaseStream = readStream;
	TextStream = new TStreamReader ( readStream, (TEncoding&)Encodings.Latin1 );
}

void TRMLReader::Parse( bool closeStream )
{
	String data;
	int interrupt;

	bool keySet = false;
	String curKey;

	bool valueSet = false;
	String curValue;

	TRMLNode* curNode;
	bool waitingValue = false;
	bool waitingEnd = false;

	String interruptChars = "{}:;/\"";
	String ignoreChars = "\x20\x09\x0A\x0D\0x0B";
	
	String quotChars = "\x22";

	TStack<TRMLNode*> nodeStack(128);
	nodeStack.Push(&BaseNode);

	while(!TextStream->EndOfStream)
	{
		data = TextStream->ReadUntil(interruptChars,ignoreChars,interrupt);

		switch (interrupt)
		{
			case '{':
			{
				if (!keySet) curKey = data;
				TRMLNode* newNode = new TRMLNode(curKey);
				nodeStack.Peek()->Nodes.Add(newNode);
				nodeStack.Push(newNode);
				curNode = newNode;
				keySet = false;
				break;
			}


		case ':':
			curKey = data;
			keySet = true;
			waitingValue = true;
			break;

		case ';':
			if (waitingValue)
			{
				if (!valueSet) curValue = data;
				curNode->Values.Add(curKey,new String(curValue));
				valueSet = false;
				keySet = false;
				waitingValue = false;
			}
			break;

		case '"':
			data = TextStream->ReadUntil(quotChars,String::Empty,interrupt);
			if (waitingValue)
			{
				curValue = data;
				valueSet = true;
			}
			else
			{
				curKey = data;
				keySet = true;
			}
			break;

		case '}':
			nodeStack.Pop();
			curNode = nodeStack.Peek();
			break;

		case '/':
			if (TextStream->Peek() == '/')
			{
				TextStream->ReadLine(); // so it will skip that line
			}
			break;
		}
	}

	if (closeStream)
	{
		TextStream->Close();
	}
}

void TRMLNode::DeserializeObject( TType* minfo, void* object )
{

}