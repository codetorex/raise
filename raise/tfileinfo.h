#ifndef TFILEINFO_H
#define TFILEINFO_H

#include "tenumerator.h"
#include "tpath.h"
#include "tfile.h"
#include "tdatetime.h"

typedef ui32 FileAttribute;
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
	virtual TString				GetFullName() = 0;
	virtual TString				GetName() = 0;

	virtual bool				Exists() = 0;

	virtual TDateTime			GetLastAccessTime() = 0;
	virtual TDateTime			GetCreationTime() = 0;
	virtual TDateTime			GetLastWriteTime() = 0;

	virtual FileAttribute		GetAttributes() = 0;
	virtual void				SetAttributes(FileAttribute attributes) = 0;
};

/**
* File interface for file system management.
*/
class IFile: public IFileSystemNode
{
public:
	virtual TString				GetExtension() = 0;

	virtual	IDirectory*			GetParent() = 0;

	virtual ui64				Length() = 0;

	virtual TStream*			Open(FileMode mode) = 0;
	virtual TStream*			Create() = 0;
	virtual	bool				Delete() = 0;
	virtual bool				MoveTo(TString& destFileName) = 0;
};

#ifdef WIN32
#include "tfilewin32.h"
#else

#endif

#endif