#include "stdafx.h"
#include "mgensourceparser.h"
#include "tconsole.h"

bool MGenSourceParser::IsIdentifierCharacter( ch32 chr )
{
	if (chr >= 'A' && chr <= 'Z')
		return true;

	if (chr >= 'a' && chr <= 'z')
		return true;

	if (chr >= '0' && chr <= '9')
		return true;

	if (chr == '_')
		return true;

	return false;
}

TString MGenSourceParser::ParseWord()
{
	TString result(64);

	while(Column.MoveNext())
	{


		if (Column.Current == '/')
		{
			bool movingNext = Column.MoveNext();
			if (movingNext )
			{
				// Line comment
				if (Column.Current == '/')
				{
					break;
				}

				// Long comment
				if (Column.Current == '*')
				{
					LongCommentMode = true;

					continuecomment:
					while(Column.MoveNext())
					{
						if (Column.Current == '*')
						{
							bool movingNext = Column.MoveNext();
							if (movingNext)
							{
								if (Column.Current == '/')
								{
									LongCommentMode = false;
									break;
								}
							}
						}
					}

					if (LongCommentMode)
					{
						if ( !LoadNextLine())
						{
							throw Exception("End of file without completing comment");
						}
						goto continuecomment;
					}
					continue;
				}

				// ADD AS DIVISION TOKEN
			}
		}

		if (ignoreChars.Have(Column.Current)) // like trimming beginning of line
		{
			continue;
		}

		if (Column.Current == '"')
		{
			StringLiteralMode = true;
			result += Column.Current;
			
			continueloading:
			while ( Column.MoveNext())
			{
				if (Column.Current == '\\')
				{
					result += Column.Current;
					bool movingNext = Column.MoveNext();
					if (movingNext)
					{
						if (Column.Current == '"')
						{	
							result += Column.Current;
							continue;
						}
					}
				}
				else if ( Column.Current == '"')
				{
					StringLiteralMode = false;
					result += Column.Current;
					break;
				}

				result += Column.Current;
			}

			if (StringLiteralMode)
			{
				// continue on next line
				if (!LoadNextLine())
				{
					throw Exception("End of file without completing string literal");
				}

				goto continueloading;
			}

			return result;
		}


		result += Column.Current;

		if (!IsIdentifierCharacter(Column.Current))
		{	
			return result;
		}

		int additionalChars = 0;
		while(Column.MoveNext())
		{
			if (!IsIdentifierCharacter(Column.Current))
			{
				Column.MoveBack();
				break;
			}
			/*if (ignoreChars.Have(Column.Current))
			{
			break;
			}*/

			// implement interrupts too like ( ) ; 

			// implement string literal parsing
			result += Column.Current;
		}
		return result;
	}

	if ( !LoadNextLine() )
	{
		NoMoreWords = true;
		return TString::Empty;
	}
	return ParseWord();
}

void MGenSourceParser::Parse()
{
	Source->Root.Token = "<ROOT>";

	MGenToken* CurrentLevel = &Source->Root;

	int wordIndex = 0;

	LoadLine(0);
	while(!NoMoreWords)
	{
		TString curWord = ParseWord();
		if (NoMoreWords)
			break;

		Console.WriteLine("% = %",sfi(wordIndex,3,'0'), sfs(curWord));
		wordIndex++;
		if (wordIndex == 354)
		{
			Console.WriteLine("Error");
		}
	}
}

