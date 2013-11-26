#include "stdafx.h"
#include "tiniparser.h"



TINIParser::TINIParser()
{
	BaseStream = 0;
	TextStream = 0;
	LowerCaseKeys = false;
}

TINIParser::TINIParser(TStream* readStream)
{
	LowerCaseKeys = false;
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
		String currentLine = TextStream->ReadLine();
		if (currentLine.StartsWith(";"))
		{
			continue;
		}

		if (currentLine.StartsWith("["))
		{
			int lineEnd = currentLine.IndexOf("]");
			TINIClass* newClass = new TINIClass( currentLine.Substring(1,lineEnd-1) );
			CurrentClass = newClass;
			if (LowerCaseKeys)
			{
				CurrentClass->Name.ToLowerInplace();
			}
			Classes.Add(CurrentClass->Name, CurrentClass);
			continue;
		}

		int EqualityOperator = currentLine.IndexOf("=");

		if (EqualityOperator == -1)
		{
			continue; // skip that line if no equality
		}

		String keyName = currentLine.Substring(0,EqualityOperator);
		keyName.TrimStartInplace();

		String* value = new String(currentLine.Substring(EqualityOperator+1,currentLine.Length-EqualityOperator-1));
		value->TrimInplace();

		if (LowerCaseKeys)
		{
			keyName.ToLowerInplace();
		}
		CurrentClass->Variables.Add(keyName,value);
	}

	if (closeStream)
	{
		TextStream->Close();
		TextStream = 0;
		BaseStream = 0;
	}
}

String* TINIParser::GetValue( const String& className, const String& key )
{
	TINIClass* keyClass = Classes.GetValue(className);
	String* value = keyClass->Variables.GetValue(key);
	return value;
}