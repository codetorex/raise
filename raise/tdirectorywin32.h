#ifndef TDIRECTORYWIN32_H
#define TDIRECTORYWIN32_H

#include "tdirectoryinfo.h"

#ifdef WIN32

#include "tplatform.h"

class TDirectoryInfo: public IDirectory
{
public:
	String OriginalPath;
	String FullPath;

	TDirectoryInfo()
	{
		OriginalPath.Allocate(512);
		FullPath.Allocate(512);
	}

	TDirectoryInfo(const String& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(path);
		TPath::AppendDirectorySeprator(FullPath);
	}
	
	/**
	* Creates all directories and subdirectories in the specified path.
	*/
	static TDirectoryInfo CreateDir(const String& path)
	{
		char tmp[512];
		String tmpstr(tmp);
		throw NotImplementedException(__FILE__,__LINE__);
		//CreateDirSlient(path,tmpstr.Chars);
		return TDirectoryInfo(tmpstr);
	}

	/*static void CreateDirSlient(const TString& path,char* output = 0)
	{
		if (path.Length <= 1)
		{
			return;
		}

		char tmp[512];
		MemoryDriver::Set(tmp,0,512);
		MemoryDriver::Copy(tmp,path.Chars,path.Length);
		TString tmpstr(tmp);
		TPath::CorrectSeprators(tmpstr);
		TPath::StripFilename(tmpstr);

		// Create all the directories in the string...
		dword orgLength = tmpstr.Length;
		for (int i =1;i<tmpstr.Length;i++)
		{
			if (tmpstr.Chars[i] == TPath::DirectorySeprator)
			{
				if ( tmpstr.Chars[i-1] == ':' ) // if drive letter then pass it
					continue;
				
				char org = tmpstr.Chars[i+1];
				tmpstr.Chars[i+1] = 0;
				tmpstr.Length = i+1;
				TPathDriver::CreateFolder(tmpstr);
				tmpstr.Chars[i+1] = org;
				tmpstr.Length = orgLength;
			}
		}

		if (output != 0)
		{
			MemoryDriver::Copy(output,tmpstr.Chars,tmpstr.Length);
		}
	}*/

	String GetFullName()
	{
		return FullPath;
	}

	inline String GetName()
	{
		return TPath::GetDirectoryName(FullPath);
	}

	inline FileAttribute GetAttributes()
	{
		ch16 tmp[1024];
		Platform.RaiseToSystemString(FullPath,(byte*)tmp,sizeof(tmp));
		return GetFileAttributesW(tmp);
	}

	inline void SetAttributes(FileAttribute attributes)
	{
		throw "Not Implemented";
	}

	static bool Exists(const String& path)
	{
		ch16 tmp[1024];
		Platform.RaiseToSystemString(path,(byte*)tmp,sizeof(tmp));
		FileAttribute fa = GetFileAttributesW(tmp);
		if (fa != INVALID_FILE_ATTRIBUTES)
		{
			if ( (fa & fa_DIRECTORY) != 0)
			{
				return true;
			}
		}
		return false;
	}

	bool Exists()
	{
		return Exists(FullPath);
	}

	TDateTime GetLastAccessTime()
	{
		throw "Not Implemented";
	}

	TDateTime GetCreationTime()
	{
		throw "Not Implemented";
	}

	TDateTime GetLastWriteTime()
	{
		throw "Not Implemented";
	}


	IFolderEnumerator* EnumerateDirectories();

	IFileEnumerator* EnumerateFiles();

	Array<IFile*>* GetFiles() 
	{ 
		throw "Not Implemented";
	}

	Array<IDirectory*>* GetDirectories() 
	{ 
		throw "Not Implemented";
	}

	void Create() 
	{ 
		CreateDir(FullPath);
	}

	bool Delete(bool recursive) 
	{ 
		throw "Not Implemented";
	}

	bool MoveTo(String& destDirName) 
	{ 
		throw "Not Implemented";
	}

	IDirectory* GetParent() 
	{ 
		throw "Not Implemented";
	}
};


#endif // WIN32

#endif