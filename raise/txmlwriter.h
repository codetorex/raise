#ifndef TXMLWRITER_H
#define TXMLWRITER_H

#include "tstreamwriterutf8.h"
#include "tstring.h"
#include "tserializable.h"

/*class TRMLWriter: public TSerializer
{
private:
	int IndentLevel;
	bool JustClosedCurl;
	TStringBuilder LineBuilder;

	void WriteKeyPart(const TString& key);
	void FinishKeyValueLine();

public:
	TStream* BaseStream;
	TStreamWriterUTF8* UTF8Writer; // RML is only UTF8

	TRMLWriter(TStream* output);

	void WriteElementStart(const TString& element);
	void WriteKeyValue(const TString& key, const TString& value);
	void WriteKeyValue(const TString& key, int value);
	void WriteElementEnd();

	void Close();

	void SerializeArray(TMember* curMember, void* object);
	void Serialize(TMemberInfo* minfo, void* object);
};*/

class TXMLWriter: public TSerializer
{
private:
	int IndentLevel;
	TStringBuilderStack<512> Buffer;
	bool ElementCap;
	bool ElementWritten;
	bool AttributeMode;

	TArray<TString> Elements;
	TArray<int> ElementItemCounts;

	inline void WriteBuffer()
	{
		UTF8Writer->Write(Buffer);
		Buffer.Clear();
	}

	inline void WriteBufferLine()
	{
		WriteBuffer();
		UTF8Writer->WriteLine();
	}

	void WriteVersion()
	{
		UTF8Writer->Write("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
		UTF8Writer->WriteLine();
	}

public:
	TStream* BaseStream;
	TStreamWriterUTF8* UTF8Writer;

	TXMLWriter( TStream* output )
	{
		BaseStream = output;
		IndentLevel = 0;
		AttributeMode = false;
		UTF8Writer = new TStreamWriterUTF8(output);
		ElementCap = false;

		WriteVersion();
	}

	void WriteStartElement( const TString& elemName )
	{
		if (ElementCap)
		{
			Buffer.Append(">");
			WriteBufferLine();
		}

		if (ElementItemCounts.Count > 0)
		{
			ElementItemCounts.GetLast() += 1;
		}

		Buffer.AppendChar('\t',IndentLevel);
		Buffer.AppendChar('<');
		Buffer.Append(elemName);
		ElementCap = true;
		Elements.Add(elemName);
		ElementItemCounts.Add(0);
		IndentLevel++;
	}

	void WriteEndElement()
	{
		IndentLevel--;
		if (ElementCap)
		{
			Buffer.Append(" />");
		}
		else
		{
			if (ElementItemCounts.GetLast() > 0)
			{
				Buffer.AppendChar('\t',IndentLevel);
			}
		
			Buffer.AppendChar('<');
			Buffer.AppendChar('/');
			Buffer.Append(Elements.GetLast());
			Buffer.AppendChar('>');
		}
		ElementCap = false;
		if (IndentLevel == 0)
		{
			WriteBuffer();
		}
		else
		{
			WriteBufferLine();
		}
		Elements.RemoveLast();
		ElementItemCounts.RemoveLast();
	}

	void WriteElementString(const TString& elemName, const TString& value)
	{
		WriteStartElement(elemName);
		WriteString(value);
		WriteEndElement();
	}

	void WriteString(const TString& text)
	{
		if (ElementCap && !AttributeMode)
		{
			Buffer.AppendChar('>');
			WriteBuffer();
			ElementCap = false;
		}

		Buffer.Append(text);
		//UTF8Writer->Write(text);
	}

	void WriteAttributeString( const TString& attrName, const TString& value)
	{
		WriteStartAttribute(attrName);
		WriteString(value);
		WriteEndAttribute();
	}

	void WriteStartAttribute( const TString& attrName )
	{
		if (!ElementCap)
		{
			throw Exception("Too late");
		}

		Buffer.AppendChar(' ');
		Buffer.Append(attrName);
		Buffer.AppendChar('=');
		Buffer.AppendChar('"');
		AttributeMode = true;
	}

	void WriteEndAttribute()
	{
		Buffer.AppendChar('"');
		AttributeMode = false;
	}

	void Close()
	{
		UTF8Writer->Close();
	}

	void SerializeArray(TMember* curMember, void* object);
	void Serialize(TMemberInfo* minfo, void* object);
};


#endif