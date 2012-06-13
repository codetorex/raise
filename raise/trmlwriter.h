#ifndef TRMLWRITER_H
#define TRMLWRITER_H

#include "tstreamwriterutf8.h"
#include "tstring.h"
#include "tserializable.h"

class TRMLWriter: public TSerializer
{
private:
	TStringBuilderStack<512> LineBuilder;

	int IndentLevel;
	bool JustClosedCurl;
	
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

	void SerializeArray(TFieldInfo* curMember, void* object);
	void SerializeObject(TType* minfo, void* object);
};

#endif