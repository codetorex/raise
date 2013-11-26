#ifndef TRAISEMARKUP_H
#define TRAISEMARKUP_H

#include "thtmlwriter.h"

enum  TMarkdownElementTypes
{
	MET_HEADING1,
	MET_HEADING2,
	MET_HEADING3,
	MET_HEADING4,
};

class TMarkdownElement
{
public:
	int Type;
	String Text;

	TMarkdownElement()
	{

	}

	TMarkdownElement(int type, const String& text)
	{
		this->Type = type;
		this->Text = text;
	}
};

class TRaiseMarkdown
{
protected:
	Array< String > Lines;


public:
	Array<TMarkdownElement> Elements;


	// TODO: move this to string class?
	int CountContinousChar(const String& value, TChar character, int startIndex = 0)
	{
		int result = 0;
		TCharacterEnumerator tce(value);
		if (startIndex > 0)
		{
			tce.SkipChars(startIndex);
		}

		while (tce.MoveNext())
		{
			if (tce.Current == character)
			{
				result++;
			}
			else
			{
				break;
			}
		}

		return result;
	}

	void AddElement(int type, const String& text)
	{
		Elements.Add(TMarkdownElement(type,text));
	}

	void Parse(TTextReader& reader)
	{
		while (!reader.EndOfStream)
		{
			Lines.Add(reader.ReadLine());
		}
		Parse();
	}

	void Parse(const String& str)
	{
		Array< String* > myLines = str.Split('\n');
		for (int i=0;i<myLines.Count;i++)
		{
			Lines.Add(*myLines[i]);
		}
		Parse();
	}

protected:
	void Parse() 
	{
		for (int i=0;i<Lines.Count;i++)
		{
			String cleanLine = Lines[i].Trim();
			if (cleanLine.StartsWith("=="))
			{
				if (i > 0)
				{
					AddElement(MET_HEADING1, Lines[i-1]);
				}
				continue;
			}

			if (cleanLine.StartsWith("#"))
			{
				int level = CountContinousChar(cleanLine,'#');
				int typ = MET_HEADING1 + (level-1);

				String headingLine = cleanLine.Substring(level+1);
				AddElement(typ, headingLine);
			}
		}

	}

};

#endif