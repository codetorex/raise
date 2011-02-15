#ifndef TFOLDERINFO_H
#define TFOLDERINFO_H

#include "tarray.h"
#include "tfile.h"
#include "tmemorydriver.h"

class IDirectory;
typedef TEnumerator<IDirectory*> IFolderEnumerator;
typedef TEnumerator<IFile*> IFileEnumerator;

/**
* Directory interface for file system management.
*/
class IDirectory: public IFileSystemNode
{
public:
	virtual IFolderEnumerator*		EnumerateDirectories() = 0;
	virtual IFileEnumerator*		EnumerateFiles() = 0;

	virtual TArray<IFile*>*			GetFiles() = 0;
	virtual TArray<IDirectory*>*	GetDirectories() = 0;

	virtual void					Create() = 0;
	virtual bool					Delete(bool recursive) = 0;
	virtual bool					MoveTo(str8& destDirName) = 0;

	virtual	IDirectory*				GetParent() = 0;
};


#ifdef WIN32

class TDirectory: public IDirectory
{
public:
	str8 OriginalPath;
	str8 FullPath;

	TDirectory(const str8& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(path);
		TPath::AppendDirectorySeprator(FullPath);
	}
	
	/**
	* Creates all directories and subdirectories in the specified path.
	*/
	static TDirectory CreateDir(const str8& path)
	{
		char tmp[512];
		str8 tmpstr(tmp);
		CreateDirSlient(path,tmpstr.Chars);
		return TDirectory(tmpstr);
	}

	static void CreateDirSlient(const str8& path,char* output = 0)
	{
		if (path.Length <= 1)
		{
			return;
		}

		char tmp[512];
		MemoryDriver::Set(tmp,0,512);
		MemoryDriver::Copy(tmp,path.Chars,path.Length);
		str8 tmpstr(tmp);
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
	}

	str8& GetFullName()
	{
		return FullPath;
	}

	inline str8 GetName()
	{
		return TPath::GetDirectoryName(FullPath);
	}

	inline FileAttribute GetAttributes()
	{
		return GetFileAttributesA(FullPath.Chars);
	}

	inline void SetAttributes(FileAttribute attributes)
	{
		throw "Not Implemented";
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

	TDateTime& GetLastAccessTime()
	{
		throw "Not Implemented";
	}

	TDateTime& GetCreationTime()
	{
		throw "Not Implemented";
	}

	TDateTime& GetLastWriteTime()
	{
		throw "Not Implemented";
	}


	IFolderEnumerator* EnumerateDirectories() 
	{
		throw "Not Implemented";
	}

	IFileEnumerator* EnumerateFiles() 
	{ 
		throw "Not Implemented";
	}

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

	bool MoveTo(str8& destDirName) 
	{ 
		throw "Not Implemented";
	}

	IDirectory* GetParent() 
	{ 
		throw "Not Implemented";
	}
};

#else

// LINUX implementation here
#endif


#endif