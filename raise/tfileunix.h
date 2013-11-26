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
		throw NotImplementedException(__FILE__,__LINE__);
	}

	IDirectory* GetParent() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	ui64 Length() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TStream* Open( FileMode mode ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TStream* Create() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	bool Delete() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	bool MoveTo( TString& destFileName ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TString GetFullName() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TString GetName() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	bool Exists() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TDateTime GetLastAccessTime() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TDateTime GetCreationTime() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	TDateTime GetLastWriteTime() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	FileAttribute GetAttributes() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void SetAttributes( FileAttribute attributes ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

};

#endif // LINUX

#endif