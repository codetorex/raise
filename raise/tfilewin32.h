#ifndef TFILEWIN32_H
#define TFILEWIN32_H

#include "tfileinfo.h"

/**
* System file implementation of File interface.
*/
class TFileInfo: public IFile
{
public:
	TString OriginalPath;
	TString FullPath;

	TFileInfo()
	{
		// LETS PRE ALLCOTE THESE SHIT
		OriginalPath.Allocate(512);
		FullPath.Allocate(512);
	}

	TFileInfo(const TString& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(OriginalPath);
	}

	TString GetFullName()
	{
		return FullPath;
	}

	inline TString GetExtension()
	{
		return TPath::GetExtension(FullPath);
	}

	bool Delete()
	{
		throw "Not Implemented";
	}

	bool MoveTo(TString& destFileName)
	{
		throw "Not Implemented";
	}

	IDirectory*	GetParent()
	{
		throw "Not Implemented";
	}

	TStream* Open(FileMode mode)
	{
		TFileStream* tf = File::Open(FullPath,mode);
		if (tf == NULL)
		{
			return NULL;
		}
		return tf;
	}

	TStream* Create()
	{
		TFileStream* tf = File::Open(FullPath,fm_WriteRead);
		if (tf == NULL)
		{
			return NULL;
		}
		return tf;
	}

	inline TString GetName()
	{
		return TPath::GetFileName(FullPath);
	}

	inline FileAttribute GetAttributes()
	{
		return GetFileAttributesW(TWinTools::SystemString16(FullPath));
	}

	inline void SetAttributes(FileAttribute attributes)
	{
		throw "Not Implemented";
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

	static bool Exists(const TString& path)
	{
		FileAttribute fa = GetFileAttributesW(TWinTools::SystemString16(path));

		if (fa == fa_INVALID)
		{
			ui32 err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND)
			{
				return false;
			}
		}

		if ( (fa & fa_DIRECTORY) != 0) // if its a folder, try to open as file if same named file exists
		{
			TStream* kf = File::Open(path,fm_Read);
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

#endif