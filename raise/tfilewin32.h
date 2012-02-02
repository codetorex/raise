#ifndef TFILEWIN32_H
#define TFILEWIN32_H

#include "tfileinfo.h"

#ifdef WIN32


/**
* System file implementation of File interface.
*/
class TFileInfo: public IFile
{
public:
	TString OriginalPath;
	TString FullPath;

	/// These values will be set after first request of them.
	bool FileAttributeCache;
	DWORD Attributes;
	TDateTimeWindows CreationTime;
	TDateTimeWindows LastAccessTime;
	TDateTimeWindows LastWriteTime;
	ui64 FileSize;
	bool FileExists;

	TFileInfo()
	{
		// LETS PRE ALLCOTE THESE SHIT
		OriginalPath.Allocate(512);
		FullPath.Allocate(512);
		FileAttributeCache = false;
	}

	TFileInfo(const TString& path)
	{
		OriginalPath = path;
		FullPath = TPath::GetFullPath(OriginalPath);
		FileAttributeCache = false;
	}

	inline void UpdateCache()
	{
		ch16 tmp[1024];
		TWinTools::SystemString16(FullPath,tmp,1024);
		WIN32_FILE_ATTRIBUTE_DATA fileAttributes;

		FileAttributeCache = true;
		
		if ( GetFileAttributesExW(tmp,GetFileExInfoStandard,&fileAttributes) != 0 )
		{
			FileExists = true;
			Attributes = fileAttributes.dwFileAttributes;
			CreationTime = TDateTimeWindows(fileAttributes.ftCreationTime);
			LastAccessTime = TDateTimeWindows(fileAttributes.ftLastAccessTime);
			LastWriteTime = TDateTimeWindows(fileAttributes.ftLastWriteTime);
			FileSize = ((ui64)(fileAttributes.nFileSizeHigh) << 32) | fileAttributes.nFileSizeLow;
		}
		else
		{
			FileExists = false;
		}
	}

	TString GetFullName()
	{
		return FullPath;
	}

	inline ui64 Length()
	{
		if (!FileAttributeCache) UpdateCache();
		return FileSize;
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
		if (!FileAttributeCache) UpdateCache();
		return Attributes;
	}

	inline void SetAttributes(FileAttribute attributes)
	{
		throw "Not Implemented";
	}

	TDateTime GetLastAccessTime()
	{
		if (!FileAttributeCache) UpdateCache();
		return TDateTime(LastAccessTime);
	}

	TDateTime GetCreationTime()
	{
		if (!FileAttributeCache) UpdateCache();
		return TDateTime(CreationTime);
	}

	TDateTime GetLastWriteTime()
	{
		if (!FileAttributeCache) UpdateCache();
		return TDateTime(LastWriteTime);
	}

	static bool Exists(const TString& path)
	{
		ch16 tmp[1024];
		TWinTools::SystemString16(path,tmp,1024);

		FileAttribute fa = GetFileAttributesW(tmp);

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

#endif // WIN32

#endif