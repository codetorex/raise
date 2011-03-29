#ifndef TFILEINFO_H
#define TFILEINFO_H

#include "tenumerator.h"
#include "tpath.h"
#include "tfilestream.h"
#include "tdatetime.h"

typedef dword FileAttribute;
class IDirectory;

enum FileAttributes
{
	fa_READONLY             = 0x00000001,  
	fa_HIDDEN               = 0x00000002,  
	fa_SYSTEM               = 0x00000004, 
	fa_DIRECTORY            = 0x00000010, 
	fa_ARCHIVE              = 0x00000020, 
	fa_DEVICE               = 0x00000040, 
	fa_NORMAL               = 0x00000080, 
	fa_TEMPORARY            = 0x00000100, 
	fa_SPARSE_FILE          = 0x00000200, 
	fa_REPARSE_POINT        = 0x00000400, 
	fa_COMPRESSED           = 0x00000800, 
	fa_OFFLINE              = 0x00001000, 
	fa_NOT_CONTENT_INDEXED  = 0x00002000, 
	fa_ENCRYPTED            = 0x00004000, 
	fa_VIRTUAL              = 0x00010000, 
	fa_INVALID				= INVALID_FILE_ATTRIBUTES,
};

class IFileSystemNode
{
public:
	virtual str8&				GetFullName() = 0;
	virtual str8				GetName() = 0;

	virtual bool				Exists() = 0;

	virtual TDateTime&			GetLastAccessTime() = 0;
	virtual TDateTime&			GetCreationTime() = 0;
	virtual TDateTime&			GetLastWriteTime() = 0;

	virtual FileAttribute		GetAttributes() = 0;
	virtual void				SetAttributes(FileAttribute attributes) = 0;
};

/**
* File interface for file system management.
*/
class IFile: public IFileSystemNode
{
public:
	virtual str8				GetExtension() = 0;

	virtual	IDirectory*			GetParent() = 0;

	virtual TStream*			Open(FileMode mode) = 0;
	virtual TStream*			Create() = 0;
	virtual	bool				Delete() = 0;
	virtual bool				MoveTo(str8& destFileName) = 0;
};

#ifdef WIN32

/**
* System file implementation of File interface.
*/
class TFile: public IFile
{
public:
	str8 OriginalPath;
	str8 FullPath;

	TFile(const str8& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(OriginalPath);
	}

	str8& GetFullName()
	{
		return FullPath;
	}

	inline str8 GetExtension()
	{
		return TPath::GetExtension(FullPath);
	}

	bool Delete()
	{
		throw "Not Implemented";
	}

	bool MoveTo(str8& destFileName)
	{
		throw "Not Implemented";
	}

	IDirectory*	GetParent()
	{
		throw "Not Implemented";
	}

	TStream* Open(FileMode mode)
	{
		TFileStream* tf = TFileStream::Open(FullPath,mode);
		if (tf == NULL)
		{
			return NULL;
		}
		return tf;
	}

	TStream* Create()
	{
		TFileStream* tf = TFileStream::Open(FullPath,fm_WriteRead);
		if (tf == NULL)
		{
			return NULL;
		}
		return tf;
	}

	inline str8 GetName()
	{
		return TPath::GetFileName(FullPath);
	}

	inline FileAttribute GetAttributes()
	{
		return GetFileAttributesA(FullPath.Chars);
	}

	inline void SetAttributes(FileAttribute attributes)
	{
		throw "Not Implemented";
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

	static bool Exists(const str8& path)
	{
		FileAttribute fa = GetFileAttributesA(path.Chars);

		if (fa == fa_INVALID)
		{
			dword err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND)
			{
				return false;
			}
		}

		if ( (fa & fa_DIRECTORY) != 0) // if its a folder, try to open as file if same named file exists
		{
			TStream* kf = TFileStream::Open(path,fm_Read);
			if (kf!= NULL)
			{
				kf->Close();
				// delete kf; remains of old system. currently close already commits suicide.
				return true;
			}
			return false;
		}

		return true;
	}
	
	inline bool Exists()
	{
		return Exists(FullPath);
	}

};

#else

#endif

#endif