#ifndef THTMLWRITER_H
#define THTMLWRITER_H

#include "tstring.h"
#include "txmlwriter.h"


class THTMLWriter
{
private:
	/// I am going to use this cheat in this
	TXMLWriter* XmlWriter;

public:

	THTMLWriter(TStream* output)
	{
		XmlWriter = new TXMLWriter(output,false);
	}

	THTMLWriter(TTextWriter& textWriter)
	{
		// TODO: fix this
		XmlWriter->UTF8Writer = (TStreamWriterUTF8*)&textWriter;
	}

	void WriteBeginTag(const TString& tag)
	{
		XmlWriter->WriteStartElement(tag);
	}

	void WriteAttribute(const TString& attribute, const TString& value)
	{
		XmlWriter->WriteAttributeString(attribute,value);
	}

	void Write(const TString& value)
	{
		XmlWriter->WriteString(value);
	}

	void WriteEndTag()
	{
		XmlWriter->WriteEndElement();
	}

	void WriteLine()
	{
		XmlWriter->UTF8Writer->WriteLine();
	}
};

#endif