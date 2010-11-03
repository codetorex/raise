#ifndef TPATH_H
#define TPATH_H

#include "tstring.h"
#include "tstack.h"

#ifdef WIN32


class TPathDriver
{
public:
	static void CurrentDirectory(str8& buffer)
	{
		GetCurrentDirectoryA(buffer.Capacity,buffer.Chars);
	}

	static void TempDirectory(str8& buffer)
	{
		GetTempPathA(buffer.Capacity,buffer.Chars);
	}
};

#else

class TPathDriver
{
public:
	static void CurrentDirectory(str8& buffer)
	{
		getcwd(buffer.Chars,buffer.Capacity);
	}

	static void TempDirectory(str8& buffer)
	{
		buffer = "/tmp/";
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
			TempFolder.Allocate(256);
			TPathDriver::TempDirectory(TempFolder);
		}
	}

	inline static bool IsDirectorySeprator(ch8 chr)
	{
		if (chr == AltDirectorySeprator || chr == DirectorySeprator)
		{
			return true;
		}
		return false;
	}



	inline static bool IsRelativePathWin32(str8& path)
	{
		if (path.Chars[0] == '.')
		{
			return true;
		}
		if (path.Chars[1] == ':' && IsDirectorySeprator(path.Chars[2]))
		{
			return false;
		}
		return true;			
	}

	inline static bool IsRelativePathLinux(str8& path)
	{
		if (path.Chars[0] == '.')
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

	static ch8 AltDirectorySeprator;
	static ch8 DirectorySeprator;

	static str8 TempFolder;
	static str8 CurrentFolder;

	static str8& ChangeExtension(str8& path, str8& extension)
	{
		int i = path.Length;
		while(i--)
		{
			if (path.Chars[i] == '.')
			{
				str8 tempStr(path.Length + extension.Length + 8);
				tempStr = path;
				tempStr.Truncate(i);
				tempStr += extension;
				return tempStr;
			}
			else if (IsDirectorySeprator(path.Chars[i]))
			{
				return path;
			}
		}
		return path;
	}

	static str8& GetExtension(str8& path)
	{
		int i = path.Length;
		while(i--)
		{
			if (path.Chars[i] == '.')
			{
				return path.Substring(i+1);
			}
			else if (IsDirectorySeprator(path.Chars[i]))
			{
				// return empty string.
				return str8::Empty;
			}
		}
		return str8::Empty;
	}

	static str8& GetDirectoryName(str8& path)
	{
		int f = path.Length;
		int i = path.Length;
		if (IsDirectorySeprator(path.Chars[i]))
		{
			i--;
			f = i;
		}

		while(i--)
		{
			if (IsDirectorySeprator(path.Chars[i]))
			{
				break;
			}
		}

		return path.Substring(i,f-i);
	}

	static str8& GetFileName(str8& path)
	{
		int i = path.Length;
		while(i--)
		{
			if (IsDirectorySeprator(path.Chars[i]))
			{
				return path.Substring(i+1);
			}
		}
		return path; // path is already filename?
	}

	static str8& GetFileNameWithoutExtension(const str8& path) // correct usage is const
	{
		if (path == str8::Empty)
		{
			return str8::Empty;
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

		return path.Substring(i,k-i);
	}


	/**
	* If path starts or ends with separator, this function will remove them.
	*/
	static str8& GetPathCleared(str8& path)
	{
		int start = 0;
		int end = path.Length;

		if (IsDirectorySeprator(path.GetFirst()))
			start++;
		
		if (IsDirectorySeprator(path.GetLast()))
			end--;

		return path.Substring(start,end-start);
	}

	static str8& EliminateParents(str8& path)
	{
		ch8 seprators[3] = {DirectorySeprator,AltDirectorySeprator,0};
		str8 cleared = GetPathCleared(path);

		TArray<str8*> psplit = cleared.Split(seprators);
		TStack<str8*> stk;


		for (int i=0;i<psplit.Count;i++)
		{
			if (*psplit[i] != "..")
			{
				stk.Push(psplit[i]);
			}
			else
			{
				stk.Pop();
			}
		}

		str8 result(path.Length+16);
		while(stk.Count > 0)
		{
			result += DirectorySeprator;
			result += *stk.Pop();
			result += result;
		}

		if (IsDirectorySeprator(path.GetLast()))
		{
			result+= DirectorySeprator;
		}

		if (!IsDirectorySeprator(path.GetFirst()))
		{
			result = result.Substring(1);
		}

		return result;
	}

	static void AppendDirectorySeprator(str8& path)
	{
		if (IsDirectorySeprator(path.GetLast()))
		{
			return;
		}
		path += DirectorySeprator;
	}

	static str8& GetFullPath(str8& path)
	{	
		// TODO: resolve relativity.
		if (CurrentFolder.Length == 0)
		{
			CurrentFolder.Allocate(256);
			TPathDriver::CurrentDirectory(CurrentFolder);
		}
		str8 tmpPath(CurrentFolder.Length + path.Length + 16);
		tmpPath += CurrentFolder;
		tmpPath += path;
		if (IsRelativePath(path))
		{
			return tmpPath;
		}
		return path;
	}

	static str8& GetTempFileName()
	{
		// TODO: implement something that uses system api. GetTempFileName for win.tempnam for linux.
		CheckTempFolder();
		return TempFolder + str8::Random(8);
	}

	static str8& GetTempPath()
	{
		CheckTempFolder();
		return TempFolder;
	}
};


#endif