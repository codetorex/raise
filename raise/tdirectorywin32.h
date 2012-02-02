#ifndef TDIRECTORYWIN32_H
#define TDIRECTORYWIN32_H

#include "tdirectoryinfo.h"

#ifdef WIN32

#include "twintools.h"

class TDirectoryInfo: public IDirectory
{
public:
	TString OriginalPath;
	TString FullPath;

	TDirectoryInfo()
	{
		OriginalPath.Allocate(512);
		FullPath.Allocate(512);
	}

	TDirectoryInfo(const TString& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(path);
		TPath::AppendDirectorySeprator(FullPath);
	}
	
	/**
	* Creates all directories and subdirectories in the specified path.
	*/
	static TDirectoryInfo CreateDir(const TString& path)
	{
		char tmp[512];
		TString tmpstr(tmp);
		throw NotImplementedException();
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

	TString GetFullName()
	{
		return FullPath;
	}

	inline TString GetName()
	{
		return TPath::GetDirectoryName(FullPath);
	}

	inline FileAttribute GetAttributes()
	{
		ch16 tmp[1024];
		TWinTools::SystemString16(FullPath,tmp,1024);
		return GetFileAttributesW(tmp);
	}

	inline void SetAttributes(FileAttribute attributes)
	{
		throw "Not Implemented";
	}

	static bool Exists(const TString& path)
	{
		ch16 tmp[1024];
		TWinTools::SystemString16(path,tmp,1024);
		FileAttribute fa = GetFileAttributesW(tmp);
		if ( (fa & fa_DIRECTORY) != 0)
		{
			return true;
		}
		return false;
	}

	bool Exists()
	{
		FileAttribute fa = GetAttributes();
		if ( (fa & fa_DIRECTORY) != 0)
		{
			return true;
		}
		return false;
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

	TArray<IFile*>* GetFiles() 
	{ 
		throw "Not Implemented";
	}

	TArray<IDirectory*>* GetDirectories() 
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

	bool MoveTo(TString& destDirName) 
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