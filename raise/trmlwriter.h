#ifndef TRMLWRITER_H
#define TRMLWRITER_H

#include "tstreamwriterutf8.h"
#include "tstring.h"
#include "tserializable.h"

class TRMLWriter: public TSerializer
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
};

#endif