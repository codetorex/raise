#ifndef TEST_XML_H
#define TEST_XML_H

#include "ttest.h"
#include "txmlwriter.h"
#include "txmlreader.h"
#include "tstreamwriterutf8.h"
#include "tfile.h"
#include "tdictionary.h"

class TestTXML: public TTestCheck
{
public:
	TestTXML(): TTestCheck("XML Writer/Reader check"){}

	void Test()
	{
		TFileStream* fs = File::OpenWrite("test.xml");

		TXMLWriter xw(fs);

		xw.WriteStartElement("xmlbase");

		for (int i=0;i<5;i++)
		{
			xw.WriteStartElement( "testnode" );
			xw.WriteAttributeString("id", Convert::ToString(i));
			xw.WriteAttributeString("random", Convert::ToString(MathDriver::Random()));
			xw.WriteEndElement();
		}

		for (int i=0;i<5;i++)
		{
			xw.WriteStartElement( "host" );
			xw.WriteAttributeString("id", Convert::ToString(i));
			xw.WriteElementString("RealID", Convert::ToString(i));
			xw.WriteElementString("FakeID", Convert::ToString(i+10));
			xw.WriteEndElement();
		}

		for (int i=0;i<5;i++)
		{
			xw.WriteElementString("test2node", Convert::ToString(i));
		}

		xw.WriteEndElement();

		xw.Close();

		fs = File::OpenRead("test.xml");

		TXMLReader xr(fs);
		xr.Parse();

		AddResult(xr.RootNode.Name == "xmlbase", "Xml writer/reader root node name");
		AddResult(xr.RootNode.Nodes.Count == 15, "Xml writer/reader root's child node count");

		TFileStream* tsf = File::OpenWrite("output.txt");
		TStreamWriterUTF8* ts = new TStreamWriterUTF8(tsf);

		currentLevel = 0;
		DumpNode(ts,xr.RootNode);
		
		tsf->Close();

	}

	int currentLevel;
	void DumpNode(TStreamWriterUTF8* ts, TXMLNode& xn)
	{
		TStringBuilderStack<512> sb;

		sb.AppendChar('\t',currentLevel);
		sb.Append(" N '");
		sb.Append(xn.Name);
		sb.AppendChar('\'');
		ts->Write(sb);
		ts->WriteLine();
		sb.Clear();

		currentLevel++;

		TDictionaryEnumerator< TString* > de(&xn.Attributes);
		while(de.MoveNext())
		{
			sb.AppendChar('\t',currentLevel);
			sb.Append(" A '");
			sb.Append(de.Current->Key);
			sb.Append("'  ->  '");
			sb.Append(*de.Current->Value);
			sb.AppendChar('\'');
			ts->Write(sb);
			ts->WriteLine();
			sb.Clear();
		}

		if (xn.Value.Length > 0)
		{
			sb.AppendChar('\t',currentLevel);
			sb.Append(" V '");
			sb.Append(xn.Value);
			sb.AppendChar('\'');
			ts->Write(sb);
			ts->WriteLine();
			sb.Clear();
		}

		TArrayEnumerator< TXMLNode* > nodes(xn.Nodes);
		while (nodes.MoveNext())
		{
			
			DumpNode(ts, *nodes.Current);
		}

		currentLevel--;
	}

} TXMLCheck;


#endif