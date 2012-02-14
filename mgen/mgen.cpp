/*

Member info generating preprocessor.

*/
#include "stdafx.h"

#include "tstreamreaderutf8.h"
#include "tlog.h"
#include "tapplication.h"
#include "mgenversion.h"
#include "mcmdlineparsers.h"
#include "tpath.h"
#include "tstreamreader.h"

class MGenDataType
{
public:
	TString			DataType;

	bool			IsInteger; // if true then integer or floating
	bool			Signed; // true when data is signed
	int				BitSize;
};

class MGenClassMember
{
public:
	ui32			Flags;
	TString			ClassMemberName;
	MGenDataType*	DataType;
};

class MGenFunctionParameter
{
public:
	ui32			Flags;
	TString			ParameterName;
	MGenDataType*	DataType;
};

class MGenFunction
{
public:
	ui32			Flags;
	TString			FunctionName;
	MGenDataType*	ReturnType;

	TArray<MGenFunctionParameter*>	Parameters;
};

class MGenClass
{
public:
	TString			ClassName;
	ui32			Type; // if network packet or class?

	ui32			TotalBitLength;
	ui32			TotalByteLength;

	TArray<MGenClassMember*>	Members;

	TArray<MGenFunction*>		Functions;

	void AddMember(MGenClassMember* member)
	{
		Members.Add(member);
		TotalBitLength += member->DataType->BitSize;
		TotalByteLength = TotalBitLength / 8;
	}
};

class MGenEnumItem
{
public:
	TString			ItemName;
	ui32			Value;
};

class MGenEnum
{
public:
	TString			EnumName;

};

class MGenSourceLine
{
public:
	int				LineNumber;
	TString			Line;
};

class MGenDatabase;

class MGenToken
{
public:

	MGenToken*			Parent;
	TString				Token;
	TArray<MGenToken*>	Childs;

	inline void AddChild(MGenToken* child)
	{
		Childs.Add(child);
		child->Parent = this;
	}

	inline MGenToken* AddChild(const TString& value)
	{
		MGenToken* nchild = new MGenToken();
		nchild->Token = value;
		AddChild(nchild);
		return nchild;
	}
};


class MGenSource
{
public:
	MGenDatabase* Parent;

	TString SourcePath;

	TArray<MGenSourceLine*> Lines;

	/// Holds source pointers that this source done #include
	TArray<MGenSource*> Includes;

	TArray<MGenClass*>	Classes;

	TArray<MGenEnum*>	Enums;

	MGenToken			Root;
};


class MGenSourceParser
{

private:
	void PushColumnState()
	{
		ColumnState.CharIndex = Column.CharIndex;
		ColumnState.StrData = Column.StrData;
		ColumnState.Current = Column.Current;
	}

	void PopColumnState()
	{
		Column.CharIndex = ColumnState.CharIndex;
		Column.Current = ColumnState.Current;
		Column.StrData = ColumnState.StrData;
	}

public:

	TString ignoreChars;
	TString interruptChars;
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
		interruptChars = ignoreChars + ":!&^%'{}()[]+-.,-><=/*;\"";
		LongCommentMode = false;
		StringLiteralMode = false;
		NoMoreWords = false;
	}

	bool IsIdentifierCharacter(ch32 chr)
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

	bool LoadLine(int index)
	{
		if (index >= Source->Lines.Count)
		{
			return false;
			// TODO: there is a bug if string starts with % wtf?
			//throw Exception("LoadLine out of bounds Source: % Request: %", sfs(Source->SourcePath), sfi(index));
		}
		Line = Source->Lines.Item[index];
		Column.Initialise(Line->Line);

		return true;
	}

	bool LoadNextLine()
	{
		return LoadLine( Line->LineNumber + 1 );
	}

	void GoDown()
	{
		CurrentLevel = CurrentLevel->Childs.GetLast();
	}

	void GoUp()
	{
		CurrentLevel = CurrentLevel->Parent;
	}

	TString ParseWord()
	{
		TString result(64);
		
		while(Column.MoveNext())
		{
			/*if (LongCommentMode)
			{
				if (Column.Current == '*')
				{
					bool movingNext = Column.MoveNext();
					if (movingNext)
					{
						if (Column.Current == '/')
						{
							LongCommentMode = false;
							continue;
						}
					}
					else
					{
						break;
					}
				}
			}*/

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
			
			PushColumnState();

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

	void Parse()
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

};

class MGenDatabase
{
public:
	TArray< MGenSource* > Sources;

	void LoadCPPSource(const TString& sourcePath)
	{
		MGenSource* msrc = new MGenSource();
		
		TStreamReader* src;
		src = new TStreamReader(sourcePath);

		int lineno = 0;
		while(!src->EndOfStream)
		{
			TString currentLine = src->ReadLine();
			MGenSourceLine* mline = new MGenSourceLine();
			mline->LineNumber = lineno;
			mline->Line = currentLine;
			msrc->Lines.Add(mline);
			lineno++;
		}

		Sources.Add(msrc);
		src->Close();

		MGenSourceParser msp(msrc);
		msp.Parse();
	}

	void SaveDatabase(const TString& targetPath);
	void LoadDatabase(const TString& targetPath);
} Database;

int _tmain(int argc, _TCHAR* argv[])
{
	Application.Begin( "MGen", TVersion(MGEN_BUILD,MGEN_PHASE) );

	TCommandLine CmdLine(&Params);

	try
	{
		CmdLine.RegisterHandler(&ExePathHandler);
		CmdLine.RegisterHandler(&VersionHandler);
		CmdLine.RegisterHandler(&HelpHandler);

		CmdLine.RegisterHandler(&InputHandler);
		CmdLine.RegisterHandler(&DatabaseHandler);

		CmdLine.Initialize(argc,argv);
		CmdLine.ParseCommandLine();
	}
	catch(Exception& e)
	{
		Console.WriteLine("Error occurred while parsing command line parameters:");
		Console.WriteLine(e.Message);

	}

	if ((!Params.InputFileSpecified || !Params.DatabaseFileSpecified) && !Params.InfoCommand)
	{
		Console.WriteLine("Missing parameters");
		Console.Write("type 'mgen -h' for help");
		return 0;
	}

	TString ext = TPath::GetExtension( Params.InputFile );

	if (ext == ".h" || ext == ".cpp")
	{
		try
		{
			Database.LoadCPPSource(Params.InputFile);
		}
		catch(Exception& e)
		{
			Console.WriteLine("Error occurred while parsing source file (%): %", sfs(Params.InputFile),sfs(e.Message));
		}
	}




	// PROCESS FILES HERE

	//Console.ReadKey();

	return 0;
}

