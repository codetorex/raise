#include "stdafx.h"
#include "tiniparser.h"



TINIParser::TINIParser()
{
	BaseStream = 0;
	TextStream = 0;
}

TINIParser::TINIParser(TStream* readStream)
{
	SetSource(readStream);
}

void TINIParser::SetSource( TStream* readStream )
{
	BaseStream = readStream;
	TextStream = new TStreamReader(readStream);
}

void TINIParser::Parse(bool closeStream)
{
	while(!TextStream->EndOfStream)
	{
		str8 currentLine = TextStream->ReadLine();
		if (currentLine.StartsWith(";"))
		{
			continue;
		}

		if (currentLine.StartsWith("["))
		{
			int lineEnd = currentLine.IndexOf("]");
			TINIClass* newClass = new TINIClass( currentLine.Substring(1,lineEnd-1) );
			CurrentClass = newClass;
			Classes.Add(CurrentClass->Name, CurrentClass);
			continue;
		}

		int EqualityOperator = currentLine.IndexOf("=");

		if (EqualityOperator == -1)
		{
			continue; // skip that line if no equality
		}

		str8 keyName = currentLine.Substring(0,EqualityOperator);
		keyName.TrimStart();

		str8* value = new str8(currentLine.Substring(EqualityOperator+1,currentLine.Length-EqualityOperator-1));
		value->Trim();

		CurrentClass->Variables.Add(keyName,value);
	}

	if (closeStream)
	{
		TextStream->Close();
		TextStream = 0;
		BaseStream = 0;
	}
}

str8* TINIParser::GetValue( const str8& className, const str8& key )
{
	TINIClass* keyClass = Classes.GetValue(className);
	str8* value = keyClass->Variables.GetValue(key);
	return value;
}