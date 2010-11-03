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
	virtual str8&				GetName() = 0;

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
	virtual str8&				GetExtension() = 0;

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

	TFile(str8& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(OriginalPath);
	}

	str8& GetFullName()
	{
		return FullPath;
	}

	str8& GetExtension()
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

	TStream* Open(FileMode mode)
	{
		TFileStream* tf = new TFileStream(FullPath,mode);
		if (tf->CurrentMode == FileMode::fm_NotOpened)
		{
			delete tf;
			return NULL;
		}
		return tf;
	}

	TStream* Create()
	{
		TFileStream* tf = new TFileStream(FullPath,FileMode::fm_WriteRead);
		if (!tf->CanWrite())
		{
			delete tf;
			return NULL;
		}
		return tf;
	}

	str8& GetName()
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
	
	bool Exists()
	{
		FileAttribute fa = GetAttributes();

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
			 TStream* kf = Open(fm_Read);
			 if (kf->CanRead())
			 {
				kf->Close();
				delete kf;
				return true;
			 }
			 return false;
		 }
		 
		 return true;
	}

};

#else

#endif

#endif