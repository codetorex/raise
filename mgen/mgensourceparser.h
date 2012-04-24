#ifndef MGENSOURCEPARSER_H
#define MGENSOURCEPARSER_H

#include "mgensource.h"
#include "texception.h"

class MGenSourceParser
{
public:

	TString ignoreChars;
	MGenSource* Source;
	MGenToken* CurrentLevel;

	bool LongCommentMode;
	bool StringLiteralMode;

	MGenSourceLine* Line; // current line
	TCharacterEnumerator Column;

	TCharacterEnumerator ColumnState;

	bool NoMoreWords;



	MGenSourceParser(MGenSource* src)
	{
		Source = src;
		ignoreChars = "\x20\x09\x0A\x0D\0x0B";
		LongCommentMode = false;
		StringLiteralMode = false;
		NoMoreWords = false;
	}

	bool IsIdentifierCharacter(ch32 chr);

	bool LoadLine(ui32 index)
	{
		if (index >= Source->Lines.Count)
		{
			return false;
			// TODO: there is a bug if string starts with % wtf?
			//throw Exception("LoadLine out of bounds Source: % Request: %", sfs(Source->SourcePath), sfi(index));
		}
		Line = Source->Lines.Item[index];
		Column.Initialize(Line->Line);

		return true;
	}

	inline bool LoadNextLine()
	{
		return LoadLine( Line->LineNumber + 1 );
	}

	inline void GoDown()
	{
		CurrentLevel = CurrentLevel->Childs.GetLast();
	}

	inline void GoUp()
	{
		CurrentLevel = CurrentLevel->Parent;
	}

	TString ParseWord();

	void Parse();

};

#endif