#ifndef TRMLWRITER_H
#define TRMLWRITER_H

#include "tstreamwriterutf8.h"
#include "tstring.h"
#include "tserialization.h"

class TRMLWriter: public TSerializer
{
private:
	TStringBuilderStack<512> LineBuilder;

	int IndentLevel;
	bool JustClosedCurl;
	
	void WriteKeyPart(const String& key);
	void FinishKeyValueLine();

public:
	TStream* BaseStream;
	TStreamWriterUTF8* UTF8Writer; // RML is only UTF8

	TRMLWriter(TStream* output);

	void WriteElementStart(const String& element);
	void WriteKeyValue(const String& key, const String& value);
	void WriteKeyValue(const String& key, int value);
	void WriteElementEnd();

	void Close();

	//void SerializeArray(TFieldInfo* curMember, void* object);
	void SerializeObject(TType* minfo, void* object);
};

#endif