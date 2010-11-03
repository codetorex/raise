#ifndef TFOLDERINFO_H
#define TFOLDERINFO_H

#include "tarray.h"
#include "tfile.h"

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

	virtual bool					Create() = 0;
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

	TDirectory(str8& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(path);
		TPath::AppendDirectorySeprator(FullPath);
	}

	str8& GetFullName()
	{
		return FullPath;
	}

	str8& GetName()
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

	bool Create() 
	{ 
		throw "Not Implemented";
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