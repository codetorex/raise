#ifndef TFILEUNIX_H
#define TFILEUNIX_H


#include "tfileinfo.h"

#ifdef LINUX

class TFileInfo: public IFile
{
public:
	static bool Exists(const TString& path)
	{
		struct stat sb;
		if (stat((char*)path.Data, &sb) == -1) 
		{
			return false;
		}
		return true;
	}

	TString GetExtension() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	IDirectory* GetParent() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	ui64 Length() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TStream* Open( FileMode mode ) 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TStream* Create() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	bool Delete() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	bool MoveTo( TString& destFileName ) 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TString GetFullName() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TString GetName() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	bool Exists() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TDateTime GetLastAccessTime() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TDateTime GetCreationTime() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	TDateTime GetLastWriteTime() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	FileAttribute GetAttributes() 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

	void SetAttributes( FileAttribute attributes ) 
	{
		LowLevelNotImplemented(__FILE__,__LINE__);
	}

};

#endif // LINUX

#endif