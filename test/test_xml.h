#ifndef TEST_XML_H
#define TEST_XML_H

#include "ttest.h"
#include "txmlwriter.h"
#include "txmlreader.h"
#include "tstreamwriterutf8.h"
#include "tfile.h"
#include "tdictionary.h"

class TestTXML: public UnitTest
{
public:
	TestTXML(): UnitTest("XML Writer/Reader check"){}

	void Execute()
	{
		TFileStream* fs = File::OpenWrite("test.xml");

		TXMLWriter xw(fs);

		xw.WriteStartElement("xmlbase");

		for (int i=0;i<5;i++)
		{
			xw.WriteStartElement( "testnode" );
			xw.WriteAttributeString("id", Convert::ToString(i));
			xw.WriteAttributeString("random", Convert::ToString(Math.Random()));
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

		Result(xr.RootNode.Name == "xmlbase", "Xml writer/reader root node name");
		Result(xr.RootNode.Nodes.Count == 15, "Xml writer/reader root's child node count");

		TXMLNode* singleNode = xr.RootNode.SelectSingleNode("host");

		Result(singleNode != 0, "Select single node existing node");

		Result(singleNode && singleNode->Nodes.Count == 2, "Selected node child count");
		Result(singleNode && singleNode->Nodes.GetLast()->Name == "FakeID", "Selected node last child");



		singleNode = xr.RootNode.SelectSingleNode("dafaf");

		Result(singleNode == 0, "Select single node not existing node");

		/*TFileStream* tsf = File::OpenWrite("output.txt");
		TStreamWriterUTF8* ts = new TStreamWriterUTF8(tsf);

		currentLevel = 0;
		DumpNode(ts,xr.RootNode);
		
		tsf->Close();


		TXMLReader xr2( File::OpenRead("son-of-obsidian.htm"));
		xr2.Parse();

		TFileStream* tsf2 = File::OpenWrite("output.txt");
		TStreamWriterUTF8* ts2 = new TStreamWriterUTF8(tsf2);
		currentLevel = 0;
		DumpNode(ts2,xr2.RootNode);
		tsf2->Close();*/
	}

	/*int currentLevel;
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
	}*/

} TXMLCheck;


#endif