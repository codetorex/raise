#ifndef TFILE_H
#define TFILE_H

#include "tstring.h"
#include "tfilestream.h"

class TBuffer;

class TFile
{
public:

	static inline TFileStream* Open(const TString& path,FileMode mode)
	{
		return new TFileStream(path, mode);
	}

	static inline TFileStream* OpenRead(const TString& path)
	{
		return new TFileStream(path, fm_Read);
	}

	static inline TFileStream* OpenWrite(const TString& path)
	{
		return new TFileStream(path, fm_Write);
	}

	/**
	 * Reads contents of a file and returns how much bytes read.
	 */
	static ui32 ReadAllBytes(const TString& path, void* target, ui32 capacity);

	/**
	 * Reads contents of a file to a buffer object. Returns how much bytes read.
	 */
	static ui32 ReadAllBytes(const TString& path, TBuffer& buffer);
};

typedef TFile File;
/*
	AppendAllLines(String, IEnumerable<String>) 	Appends lines to a file, and then closes the file. 
	AppendAllLines(String, IEnumerable<String>, Encoding) 	Appends lines to a file by using a specified encoding, and then closes the file. 
	AppendAllText(String, String) 	Opens a file, appends the specified string to the file, and then closes the file. If the file does not exist, this method creates a file, writes the specified string to the file, then closes the file. 
	AppendAllText(String, String, Encoding) 	Appends the specified string to the file, creating the file if it does not already exist. 
	AppendText 	Creates a StreamWriter that appends UTF-8 encoded text to an existing file. 
	Copy(String, String) 	Copies an existing file to a new file. Overwriting a file of the same name is not allowed. 
	Copy(String, String, Boolean) 	Copies an existing file to a new file. Overwriting a file of the same name is allowed. 
	Create(String) 	Creates or overwrites a file in the specified path. 
	Create(String, Int32) 	Creates or overwrites the specified file. 
	Create(String, Int32, FileOptions) 	Creates or overwrites the specified file, specifying a buffer size and a FileOptions value that describes how to create or overwrite the file. 
	Create(String, Int32, FileOptions, FileSecurity) 	Creates or overwrites the specified file with the specified buffer size, file options, and file security. 
	CreateText 	Creates or opens a file for writing UTF-8 encoded text. 
	Decrypt 	Decrypts a file that was encrypted by the current account using the Encrypt method. 
	Delete 	Deletes the specified file. 
	Encrypt 	Encrypts a file so that only the account used to encrypt the file can decrypt it. 
	Exists 	Determines whether the specified file exists. 
	GetAccessControl(String) 	Gets a FileSecurity object that encapsulates the access control list (ACL) entries for a specified file. 
	GetAccessControl(String, AccessControlSections) 	Gets a FileSecurity object that encapsulates the specified type of access control list (ACL) entries for a particular file. 
	GetAttributes 	Gets the FileAttributes of the file on the path. 
	GetCreationTime 	Returns the creation date and time of the specified file or directory. 
	GetCreationTimeUtc 	Returns the creation date and time, in coordinated universal time (UTC), of the specified file or directory. 
	GetLastAccessTime 	Returns the date and time the specified file or directory was last accessed. 
	GetLastAccessTimeUtc 	Returns the date and time, in coordinated universal time (UTC), that the specified file or directory was last accessed. 
	GetLastWriteTime 	Returns the date and time the specified file or directory was last written to. 
	GetLastWriteTimeUtc 	Returns the date and time, in coordinated universal time (UTC), that the specified file or directory was last written to. 
	Move 	Moves a specified file to a new location, providing the option to specify a new file name. 
	Open(String, FileMode) 	Opens a FileStream on the specified path with read/write access. 
	Open(String, FileMode, FileAccess) 	Opens a FileStream on the specified path, with the specified mode and access. 
	Open(String, FileMode, FileAccess, FileShare) 	Opens a FileStream on the specified path, having the specified mode with read, write, or read/write access and the specified sharing option. 
	OpenRead 	Opens an existing file for reading. 
	OpenText 	Opens an existing UTF-8 encoded text file for reading. 
	OpenWrite 	Opens an existing file or creates a new file for writing. 
	ReadAllBytes 	Opens a binary file, reads the contents of the file into a byte array, and then closes the file. 
	ReadAllLines(String) 	Opens a text file, reads all lines of the file, and then closes the file. 
	ReadAllLines(String, Encoding) 	Opens a file, reads all lines of the file with the specified encoding, and then closes the file. 
	ReadAllText(String) 	Opens a text file, reads all lines of the file, and then closes the file. 
	ReadAllText(String, Encoding) 	Opens a file, reads all lines of the file with the specified encoding, and then closes the file. 
	ReadLines(String) 	Reads the lines of a file. 
	ReadLines(String, Encoding) 	Read the lines of a file that has a specified encoding. 
	Replace(String, String, String) 	Replaces the contents of a specified file with the contents of another file, deleting the original file, and creating a backup of the replaced file. 
	Replace(String, String, String, Boolean) 	Replaces the contents of a specified file with the contents of another file, deleting the original file, and creating a backup of the replaced file and optionally ignores merge errors. 
	SetAccessControl 	Applies access control list (ACL) entries described by a FileSecurity object to the specified file. 
	SetAttributes 	Sets the specified FileAttributes of the file on the specified path. 
	SetCreationTime 	Sets the date and time the file was created. 
	SetCreationTimeUtc 	Sets the date and time, in coordinated universal time (UTC), that the file was created. 
	SetLastAccessTime 	Sets the date and time the specified file was last accessed. 
	SetLastAccessTimeUtc 	Sets the date and time, in coordinated universal time (UTC), that the specified file was last accessed. 
	SetLastWriteTime 	Sets the date and time that the specified file was last written to. 
	SetLastWriteTimeUtc 	Sets the date and time, in coordinated universal time (UTC), that the specified file was last written to. 
	WriteAllBytes 	Creates a new file, writes the specified byte array to the file, and then closes the file. If the target file already exists, it is overwritten. 
	WriteAllLines(String, IEnumerable<String>) 	Creates a new file, writes a collection of strings to the file, and then closes the file. 
	WriteAllLines(String, String[]) 	Creates a new file, write the specified string array to the file, and then closes the file. 
	WriteAllLines(String, IEnumerable<String>, Encoding) 	Creates a new file by using the specified encoding, writes a collection of strings to the file, and then closes the file. 
	WriteAllLines(String, String[], Encoding) 	Creates a new file, writes the specified string array to the file by using the specified encoding, and then closes the file. 
	WriteAllText(String, String) 	Creates a new file, writes the specified string to the file, and then closes the file. If the target file already exists, it is overwritten. 
	WriteAllText(String, String, Encoding) 	Creates a new file, writes the specified string to the file using the specified encoding, and then closes the file. If the target file already exists, it is overwritten.
*/
#endif