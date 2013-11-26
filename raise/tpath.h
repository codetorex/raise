#ifndef TPATH_H
#define TPATH_H

#include "tstring.h"
#include "tqueue.h"
#include "tconvert.h"

#ifdef WIN32

#include "tplatform.h"

class TPathDriver
{
public:
	static String CurrentDirectory()
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

	static String TempDirectory()
	{
		ch16 tmp[1024];
		GetTempPathW(1024,tmp);
		return Platform.RaiseString(tmp);
	}

	static void CreateFolder(const String& path)
	{
		ch16 tmp[1024];
		Platform.RaiseToSystemString(path,(byte*)tmp,sizeof(tmp));
		if ( CreateDirectoryW(tmp,NULL) == FALSE )
		{
			ui32 errorId = GetLastError();
			if (errorId != ERROR_ALREADY_EXISTS)
			{
				throw PlatformException("Creating directory failed.", errorId);
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
	static String CurrentDirectory()
	{
		String result(1024);
		char* opr = getcwd((char*)result.Data,result.Capacity); // TODO: Not forget to set string length after loading.
		if (opr == NULL)
		{
			throw Exception(SOURCENAME(0),__LINE__,0,"getcwd encountered with an error");
		}
		result.UpdateLengths();
		return result;
	}

	static String TempDirectory()
	{
		String result = "/tmp/";
		return result;
	}

	static void CreateFolder(String& path)
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

	inline static bool IsRelativePathWin32(const String& path)
	{
		TCharacterEnumerator schars(path);
		if (schars.ReadChar() == '.') return true;

		if (schars.ReadChar() == ':' && IsDirectorySeprator(schars.ReadChar()))
		{
			return false;
		}
		return true;			
	}

	inline static bool IsRelativePathLinux(const String& path)
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

	static String TempFolder;
	static String CurrentFolder;

	static ui32 GetExtensionAsDword(const String& path)
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

	static void CorrectSepratorsInplace(String& path)
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

	static bool IsDirectory(const String& path)
	{
		if (IsDirectorySeprator(path.GetLast()))
		{
			return true;
		}
		return false;
	}

	static String ChangeExtension(const String& path, const String& extension)
	{
		String tempStr(path.Length + extension.Length + 8);
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

	static String GetExtension(const String& path)
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
				return String::Empty;
			}
		}
		return String::Empty;

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
	static String StripFilename(const String& path)
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
			return String::Empty;
		}

		return path.Substring(0,schars.CharIndex+1);
	}

	static String GetDirectoryName(String& path)
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

	static String GetFileName(String& path)
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

	static String GetFileNameWithoutExtension(const String& path) // correct usage is const
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
	static String GetPathCleared(String& path)
	{
		int start = 0;
		int end = path.Length;

		if (IsDirectorySeprator(path.GetFirst()))
			start++;
		
		if (IsDirectorySeprator(path.GetLast()))
			end--;

		return path.Substring(start,end-start);
	}

	static bool HasParents(const String& path)
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

	static String EliminateParents(String& path)
	{
		ch32 seprators[3] = {DirectorySeprator,AltDirectorySeprator,0};
		String cleared = GetPathCleared(path);

		Array<String*> psplit = cleared.Split(seprators);
		Array<String*> ary;

		for (ui32 i=0;i<psplit.Count;i++)
		{
			String* curpart = psplit.Items[i]; 
			if (*curpart == "..")
			{
				ary.RemoveLast();
			}
			else
			{
				ary.Add(psplit.Items[i]);
			}
		}

		String result(path.Length+16);

		for (ui32 i=0;i<ary.Count;i++)
		{
			result += *ary.Items[i];
			result += DirectorySeprator;
		}

		return result;
	}

	static bool IsEndsWithDirectorySeprator(const String& path)
	{
		if ( IsDirectorySeprator(path.GetLast()) )
		{
			return true;
		}
		return false;
	}

	static void AppendDirectorySeprator(String& path)
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
	static String GetFullPath(const String& path)
	{	
		// TODO: resolve relativity.
		if (CurrentFolder.Length == 0)
		{
			CurrentFolder = TPathDriver::CurrentDirectory();
		}
		String tmpPath(CurrentFolder.Length + path.Length + 16);
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

	static String GetTempFileName()
	{
		// TODO: implement something that uses system api. GetTempFileName for win.tempnam for linux.
		CheckTempFolder();
		throw NotImplementedException(__FILE__,__LINE__);
		//return TempFolder + TString::Random(8);
	}

	static String GetTempPath()
	{
		CheckTempFolder();
		return TempFolder;
	}
};


#endif