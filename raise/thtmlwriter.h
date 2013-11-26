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

	void WriteBeginTag(const String& tag)
	{
		XmlWriter->WriteStartElement(tag);
	}

	void WriteAttribute(const String& attribute, const String& value)
	{
		XmlWriter->WriteAttributeString(attribute,value);
	}

	void Write(const String& value)
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