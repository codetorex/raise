#ifndef TFOLDERINFO_H
#define TFOLDERINFO_H

#include "tarray.h"
#include "tfileinfo.h"
#include "tmemorydriver.h"
#include "tenumerator.h"

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

	virtual Array<IFile*>*			GetFiles() = 0;
	virtual Array<IDirectory*>*	GetDirectories() = 0;

	virtual void					Create() = 0;
	virtual bool					Delete(bool recursive) = 0;
	virtual bool					MoveTo(String& destDirName) = 0;

	virtual	IDirectory*				GetParent() = 0;
};


#ifdef WIN32
#include "tdirectorywin32.h"
#else

// LINUX implementation here
#endif


#endif