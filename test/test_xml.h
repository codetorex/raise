#ifndef TEST_XML_H
#define TEST_XML_H

#include "ttest.h"
#include "txmlwriter.h"
#include "tfile.h"

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
	}

} TXMLCheck;


#endif