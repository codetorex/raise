#include "stdafx.h"
#include "tdirectorywin32.h"

#ifdef WIN32


class TNodeEnumeratorWin32
{
public:
	bool FindDataOpen;
	ch16 FullPath[512];

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	size_t length_of_arg;
	HANDLE hFind;
	DWORD dwError;

	bool NoFilesLeft;

	ui32 EnumerateFilter;

	TFileInfo CurFile;
	TDirectoryInfo CurDirectory;

	TDirectoryInfo* ParentInfo;

	enum EnumerationFilters
	{
		FEF_FILES,
		FEF_DIRECTORIES,
		FEF_BOTH,
	};

	void Setup(TDirectoryInfo* parent)
	{
		TWinTools::SystemString16(parent->FullPath,FullPath,1024);
		FullPath[parent->FullPath.Length] = L'*';
		FullPath[parent->FullPath.Length+1] = 0;
		FindDataOpen = false;
		NoFilesLeft = false;
		this->ParentInfo = parent;

		CurFile.FullPath += ParentInfo->FullPath;
		CurDirectory.FullPath += ParentInfo->FullPath;

		hFind = INVALID_HANDLE_VALUE;
		dwError = 0;
		CloseHandle();
	}

	TNodeEnumeratorWin32()
	{

	}

	TNodeEnumeratorWin32(TDirectoryInfo* parent)
	{
		Setup(parent);
	}

	void CloseHandle()
	{
		if (FindDataOpen)
		{
			FindClose(hFind);
			FindDataOpen = false;
		}
	}

	bool MoveNext()
	{
		if (NoFilesLeft)
		{
			return false;
		}

nextfile:
		if (FindDataOpen)
		{
			if ( FindNextFile(hFind,&ffd) == 0 )
			{
				NoFilesLeft = true;
				CloseHandle();
				return false;
			}
		}
		else
		{
			hFind = FindFirstFileW(FullPath,&ffd);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				throw Exception("Something wrong with directory enumerator");
			}
			FindDataOpen = true;
		}

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (EnumerateFilter == FEF_FILES)
			{
				goto nextfile;
			}
			else
			{
				// SKIP . and .. Folders, isn't this dumb to have these in first place Microsoft?
				if (ffd.cFileName[0] == '.')
				{
					if (ffd.cFileName[1] == '.')
					{
						if (ffd.cFileName[2] == 0)
						{
							return MoveNext();
						}
					}
					else if (ffd.cFileName[1] == 0)
					{
						return MoveNext();
					}
				}

				CurDirectory.FullPath.TruncateDerived(ParentInfo->FullPath);
				CurDirectory.FullPath += ffd.cFileName;
				CurDirectory.FullPath.AppendASCII(TPath::DirectorySeprator);
				return true;
			}
		}
		else
		{
			if (EnumerateFilter == FEF_DIRECTORIES)
			{
				goto nextfile;
			}
			else
			{
				CurFile.FullPath.TruncateDerived(ParentInfo->FullPath);
				CurFile.FullPath += ffd.cFileName;
			}
		}

		return true;
	}
};


class TFolderEnumeratorWin32: public IFolderEnumerator
{
public:
	TNodeEnumeratorWin32 mainTool;

	TFolderEnumeratorWin32(TDirectoryInfo* parent)
	{
		mainTool.ParentInfo =  parent;
		mainTool.EnumerateFilter = TNodeEnumeratorWin32::FEF_DIRECTORIES;
		Reset();
	}

	void Reset()
	{
		mainTool.Setup(mainTool.ParentInfo);
	}

	bool MoveNext()
	{
		bool result = mainTool.MoveNext();
		if (result)
		{
			Current = &mainTool.CurDirectory;
		}
		return result;
	}
};

class TFileEnumeratorWin32: public IFileEnumerator
{
public:
	TNodeEnumeratorWin32 mainTool;

	TFileEnumeratorWin32(TDirectoryInfo* parent)
	{
		mainTool.ParentInfo =  parent;
		mainTool.EnumerateFilter = TNodeEnumeratorWin32::FEF_FILES;
		Reset();
	}

	void Reset()
	{
		mainTool.Setup(mainTool.ParentInfo);
	}

	bool MoveNext()
	{
		bool result = mainTool.MoveNext();
		if (result)
		{
			Current = &mainTool.CurFile;
		}
		return result;
	}
};

IFolderEnumerator* TDirectoryInfo::EnumerateDirectories()
{
	TFolderEnumeratorWin32* result = new TFolderEnumeratorWin32(this);
	return result;
}

IFileEnumerator* TDirectoryInfo::EnumerateFiles()
{
	TFileEnumeratorWin32* result = new TFileEnumeratorWin32(this);
	return result;
}

#endif