#ifndef TPATH_H
#define TPATH_H

#include "tstring.h"
#include "tqueue.h"
#include "texception.h"
#include "tconvert.h"

#ifdef WIN32

#include "tplatform.h"

class TPathDriver
{
public:
	static TString CurrentDirectory()
	{
		ch16 tmp[512];
		int tmplength = GetCurrentDirectoryW(512,tmp);
		ch16 lastChar = tmp[tmplength-1];
		if ( lastChar != '/' && lastChar != '\\' )
		{
			tmp[tmplength] = '\\';
			tmplength++;
			tmp[tmplength] = 0;
		}
		return Platform.RaiseString(tmp);
	}

	static TString TempDirectory()
	{
		ch16 tmp[1024];
		GetTempPathW(1024,tmp);
		return Platform.RaiseString(tmp);
	}

	static void CreateFolder(const TString& path)
	{
		ch16 tmp[1024];
		Platform.RaiseToSystemString(path,(byte*)tmp,sizeof(tmp));
		if ( CreateDirectoryW(tmp,NULL) == FALSE )
		{
			ui32 errorId = GetLastError();
			if (errorId != ERROR_ALREADY_EXISTS)
			{
				throw OSException("Creating directory failed.", errorId);
				//throw Exception("Creating directory failed. " + Convert::ToString(errorId));
				//throw Exception(TString::FormatNew("Creating directory failed. Error: 0x%X",errorId));
			}
		}
	}
};

#else

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

class TPathDriver
{
public:
	static TString CurrentDirectory()
	{
		TString result(1024);
		char* opr = getcwd((char*)result.Data,result.Capacity); // TODO: Not forget to set string length after loading.
		if (opr == NULL)
		{
			throw Exception(SOURCENAME(0),__LINE__,0,"getcwd encountered with an error");
		}
		result.UpdateLengths();
		return result;
	}

	static TString TempDirectory()
	{
		TString result = "/tmp/";
		return result;
	}

	static void CreateFolder(TString& path)
	{
		if ( mkdir((char*)path.Data,NULL) == 0 )
		{
			throw Exception("Creating directory failed");
		}
	}
};

#endif


class TPath
{
private:
	inline static void CheckTempFolder()
	{
		if (TempFolder.Length == 0)
		{
			TempFolder = TPathDriver::TempDirectory();
		}
	}

	inline static bool IsDirectorySeprator(ch32 chr)
	{
		if (chr == AltDirectorySeprator || chr == DirectorySeprator)
		{
			return true;
		}
		return false;
	}

	inline static bool IsRelativePathWin32(const TString& path)
	{
		TCharacterEnumerator schars(path);
		if (schars.ReadChar() == '.') return true;

		if (schars.ReadChar() == ':' && IsDirectorySeprator(schars.ReadChar()))
		{
			return false;
		}
		return true;			
	}

	inline static bool IsRelativePathLinux(const TString& path)
	{
		if (path.GetFirst() == '.')
		{
			return true;
		}
		return false;			
	}

#ifdef WIN32
#define IsRelativePath	IsRelativePathWin32
#else
#define IsRelativePath	IsRelativePathLinux
#endif


public:

	static ch32 AltDirectorySeprator;
	static ch32 DirectorySeprator;

	static TString TempFolder;
	static TString CurrentFolder;

	static ui32 GetExtensionAsDword(const TString& path)
	{
		if (!path.IsASCII())
		{
			throw NotImplementedException(__FILE__,__LINE__);
		}

		if (path.Length < 4)
		{
			throw Exception("Path length cant be smaller than 4");
		}

		ch8 dw[4];
		for (ui32 i= path.Length-4,k=0;i<path.Length;i++,k++)
		{
			ch8 curChr = path.Data[i];
			if (curChr >= 'a' && curChr <= 'z' ) curChr -= 32; // lowercase
			dw[k] = curChr;
		}
		return *(ui32*)dw;
	}

	static void CorrectSepratorsInplace(TString& path)
	{
		path.DetachToEdit();
		for (ui32 i=0;i< path.Length;i++)
		{
			if (path.Data[i] == AltDirectorySeprator)
			{
				path.Data[i] = DirectorySeprator;
			}
		}
	}

	static bool IsDirectory(const TString& path)
	{
		if (IsDirectorySeprator(path.GetLast()))
		{
			return true;
		}
		return false;
	}

	static TString ChangeExtension(const TString& path, const TString& extension)
	{
		TString tempStr(path.Length + extension.Length + 8);
		TCharacterReverseEnumerator schars(path);
		while(schars.MoveNext())
		{
			if (schars.Current == '.')
			{
				tempStr = path;
				tempStr.Truncate(schars.CharIndex);
				tempStr += extension;
				return tempStr;
			}
			else if (IsDirectorySeprator(schars.Current))
			{
				tempStr = path;
			}
		}
		return tempStr;

		/*while(i--)
		{
			if (path.Chars[i] == '.')
			{
				tempStr = path;
				tempStr.Truncate(i);
				tempStr += extension;
				return tempStr;
			}
			else if (IsDirectorySeprator(path.Chars[i]))
			{
				tempStr = path;
			}
		}
		return tempStr;*/
	}

	static TString GetExtension(const TString& path)
	{
		TCharacterReverseEnumerator schars(path);
		while(schars.MoveNext())
		{
			if (schars.Current == '.')
			{
				return path.Substring(schars.CharIndex); // charIndex + 1 wasnt used for .net compability?
			}
			else if (IsDirectorySeprator(schars.Current))
			{
				return TString::Empty;
			}
		}
		return TString::Empty;

		/*int i = path.Length;
		while(i--)
		{
			if (path.Chars[i] == '.')
			{
				return path.Substring(i+1);
			}
			else if (IsDirectorySeprator(path.Chars[i]))
			{
				// return empty string.
				return TString::Empty;
			}
		}
		return TString::Empty;*/
	}

	/**
	 * Strips filename from path.
	 * Example: for given path "c:\windows\system32\test.dll" it will return "c:\windows\system32\"
	 */
	static TString StripFilename(const TString& path)
	{
		TCharacterReverseEnumerator schars(path);

		while(schars.MoveNext())
		{
			if (IsDirectorySeprator(schars.Current))
			{
				break;
			}
		}

		if (schars.CharIndex == 0)
		{
			return TString::Empty;
		}

		return path.Substring(0,schars.CharIndex+1);
	}

	static TString GetDirectoryName(TString& path)
	{
		int f = path.Length;
		TCharacterReverseEnumerator schars(path);

		if (IsDirectorySeprator(path.GetLast()))
		{
			f--;
			schars.MoveNext();
		}
		
		while(schars.MoveNext())
		{
			if (IsDirectorySeprator(schars.Current))
			{
				break;
			}
		}
		return path.Substring(schars.CharIndex+1,(f-schars.CharIndex)-1);
	}

	static TString GetFileName(TString& path)
	{
		TCharacterReverseEnumerator schars(path);
		while(schars.MoveNext())
		{
			if (IsDirectorySeprator(schars.Current))
			{
				return path.Substring(schars.CharIndex+1);
			}
		}
		return path; // path is already filename?
	}

	static TString GetFileNameWithoutExtension(const TString& path) // correct usage is const
	{
		/*if (path == TString::Empty)
		{
			return TString::Empty;
		}

		int i = path.Length;
		while(i--)
		{
			if (IsDirectorySeprator(path.Chars[i]))
			{
				break;
			}
		}

		int k;
		for ( k = i;k<path.Length;k++)
		{
			if (path.Chars[k] == '.')
			{
				break;
			}
		}

		return path.Substring(i,k-i);*/
		throw NotImplementedException(__FILE__,__LINE__);
	}


	/**
	* If path starts or ends with separator, this function will remove them.
	*/
	static TString GetPathCleared(TString& path)
	{
		int start = 0;
		int end = path.Length;

		if (IsDirectorySeprator(path.GetFirst()))
			start++;
		
		if (IsDirectorySeprator(path.GetLast()))
			end--;

		return path.Substring(start,end-start);
	}

	static bool HasParents(const TString& path)
	{
		TCharacterReverseEnumerator schars(path);
		while(schars.MoveNext())
		{
			if (IsDirectorySeprator(schars.Current))
			{
				if (schars.ReadChar() == '.')
				{
					if ( schars.ReadChar() == '.')
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	static TString EliminateParents(TString& path)
	{
		ch32 seprators[3] = {DirectorySeprator,AltDirectorySeprator,0};
		TString cleared = GetPathCleared(path);

		TArray<TString*> psplit = cleared.Split(seprators);
		TArray<TString*> ary;

		for (ui32 i=0;i<psplit.Count;i++)
		{
			TString* curpart = psplit.Item[i]; 
			if (*curpart == "..")
			{
				ary.RemoveLast();
			}
			else
			{
				ary.Add(psplit.Item[i]);
			}
		}

		TString result(path.Length+16);

		for (ui32 i=0;i<ary.Count;i++)
		{
			result += *ary.Item[i];
			result += DirectorySeprator;
		}

		return result;
	}

	static bool IsEndsWithDirectorySeprator(const TString& path)
	{
		if ( IsDirectorySeprator(path.GetLast()) )
		{
			return true;
		}
		return false;
	}

	static void AppendDirectorySeprator(TString& path)
	{
		if (IsDirectorySeprator(path.GetLast()))
		{
			return;
		}
		path += DirectorySeprator;
	}

	/**
	 * Retrieves full path for given relative path
	 */
	static TString GetFullPath(const TString& path)
	{	
		// TODO: resolve relativity.
		if (CurrentFolder.Length == 0)
		{
			CurrentFolder = TPathDriver::CurrentDirectory();
		}
		TString tmpPath(CurrentFolder.Length + path.Length + 16);
		tmpPath += CurrentFolder;
		
		if (!IsEndsWithDirectorySeprator(tmpPath))
		{
			tmpPath += DirectorySeprator;
		}

		tmpPath += path;
		if (!IsRelativePath(path))
		{
			tmpPath = path;
		}

		CorrectSepratorsInplace(tmpPath);

		if (HasParents(tmpPath))
		{
			tmpPath = EliminateParents(tmpPath);
		}

		return tmpPath;
	}

	static TString GetTempFileName()
	{
		// TODO: implement something that uses system api. GetTempFileName for win.tempnam for linux.
		CheckTempFolder();
		throw NotImplementedException(__FILE__,__LINE__);
		//return TempFolder + TString::Random(8);
	}

	static TString GetTempPath()
	{
		CheckTempFolder();
		return TempFolder;
	}
};


#endif