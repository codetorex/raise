#ifndef TFILE_H
#define TFILE_H

#include "tstring.h"
#include "tfilestream.h"

class TFile
{
public:

	static inline TFileStream* Open(const str8& path,FileMode mode)
	{
		return new TFileStream(path, mode);
	}

	static inline TFileStream* OpenRead(const str8& path)
	{
		return new TFileStream(path, fm_Read);
	}

	static inline TFileStream* OpenWrite(const str8& path)
	{
		return new TFileStream(path, fm_Write);
	}

	/*// Summary:
	//     Opens a file, appends the specified string to the file, and then closes the
	//     file. If the file does not exist, this method creates a file, writes the
	//     specified string to the file, then closes the file.
	//
	// Parameters:
	//   path:
	//     The file to append the specified string to.
	//
	//   contents:
	//     The string to append to the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void AppendAllText(string path, string contents);
	//
	// Summary:
	//     Appends the specified string to the file, creating the file if it does not
	//     already exist.
	//
	// Parameters:
	//   path:
	//     The file to append the specified string to.
	//
	//   contents:
	//     The string to append to the file.
	//
	//   encoding:
	//     The character encoding to use.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void AppendAllText(string path, string contents, Encoding encoding);
	//
	// Summary:
	//     Creates a System.IO.StreamWriter that appends UTF-8 encoded text to an existing
	//     file.
	//
	// Parameters:
	//   path:
	//     The path to the file to append to.
	//
	// Returns:
	//     A StreamWriter that appends UTF-8 encoded text to an existing file.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static StreamWriter AppendText(string path);
	//
	// Summary:
	//     Copies an existing file to a new file. Overwriting a file of the same name
	//     is not allowed.
	//
	// Parameters:
	//   sourceFileName:
	//     The file to copy.
	//
	//   destFileName:
	//     The name of the destination file. This cannot be a directory or an existing
	//     file.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     sourceFileName or destFileName is a zero-length string, contains only white
	//     space, or contains one or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//      -or- sourceFileName or destFileName specifies a directory.
	//
	//   System.ArgumentNullException:
	//     sourceFileName or destFileName is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The path specified in sourceFileName or destFileName is invalid (for example,
	//     it is on an unmapped drive).
	//
	//   System.IO.FileNotFoundException:
	//     sourceFileName was not found.
	//
	//   System.IO.IOException:
	//     destFileName exists.  -or- An I/O error has occurred.
	//
	//   System.NotSupportedException:
	//     sourceFileName or destFileName is in an invalid format.
	public static void Copy(string sourceFileName, string destFileName);
	//
	// Summary:
	//     Copies an existing file to a new file. Overwriting a file of the same name
	//     is allowed.
	//
	// Parameters:
	//   sourceFileName:
	//     The file to copy.
	//
	//   destFileName:
	//     The name of the destination file. This cannot be a directory.
	//
	//   overwrite:
	//     true if the destination file can be overwritten; otherwise, false.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     sourceFileName or destFileName is a zero-length string, contains only white
	//     space, or contains one or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//      -or- sourceFileName or destFileName specifies a directory.
	//
	//   System.ArgumentNullException:
	//     sourceFileName or destFileName is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The path specified in sourceFileName or destFileName is invalid (for example,
	//     it is on an unmapped drive).
	//
	//   System.IO.FileNotFoundException:
	//     sourceFileName was not found.
	//
	//   System.IO.IOException:
	//     destFileName is read-only, or destFileName exists and overwrite is false.
	//      -or- An I/O error has occurred.
	//
	//   System.NotSupportedException:
	//     sourceFileName or destFileName is in an invalid format.
	public static void Copy(string sourceFileName, string destFileName, bool overwrite);
	//
	// Summary:
	//     Creates or overwrites a file in the specified path.
	//
	// Parameters:
	//   path:
	//     The path and name of the file to create.
	//
	// Returns:
	//     A System.IO.FileStream that provides read/write access to the file specified
	//     in path.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while creating the file.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream Create(string path);
	//
	// Summary:
	//     Creates or overwrites the specified file.
	//
	// Parameters:
	//   path:
	//     The name of the file.
	//
	//   bufferSize:
	//     The number of bytes buffered for reads and writes to the file.
	//
	// Returns:
	//     A System.IO.FileStream with the specified buffer size that provides read/write
	//     access to the file specified in path.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while creating the file.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream Create(string path, int bufferSize);
	//
	// Summary:
	//     Creates or overwrites the specified file, specifying a buffer size and a
	//     System.IO.FileOptions value that describes how to create or overwrite the
	//     file.
	//
	// Parameters:
	//   path:
	//     The name of the file.
	//
	//   bufferSize:
	//     The number of bytes buffered for reads and writes to the file.
	//
	//   options:
	//     One of the System.IO.FileOptions values that describes how to create or overwrite
	//     the file.
	//
	// Returns:
	//     A new file with the specified buffer size.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only. -or- System.IO.FileOptions.Encrypted is specified
	//     for options and file encryption is not supported on the current platform.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while creating the file.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.
	public static FileStream Create(string path, int bufferSize, FileOptions options);
	//
	// Summary:
	//     Creates or overwrites the specified file with the specified buffer size,
	//     file options, and file security.
	//
	// Parameters:
	//   path:
	//     The name of the file.
	//
	//   bufferSize:
	//     The number of bytes buffered for reads and writes to the file.
	//
	//   options:
	//     One of the System.IO.FileOptions values that describes how to create or overwrite
	//     the file.
	//
	//   fileSecurity:
	//     One of the System.Security.AccessControl.FileSecurity values that determines
	//     the access control and audit security for the file.
	//
	// Returns:
	//     A new file with the specified buffer size, file options, and file security.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.  -or- System.IO.FileOptions.Encrypted is specified
	//     for options and file encryption is not supported on the current platform.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while creating the file.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     file that is read-only.
	public static FileStream Create(string path, int bufferSize, FileOptions options, FileSecurity fileSecurity);
	//
	// Summary:
	//     Creates or opens a file for writing UTF-8 encoded text.
	//
	// Parameters:
	//   path:
	//     The file to be opened for writing.
	//
	// Returns:
	//     A System.IO.StreamWriter that writes to the specified file using UTF-8 encoding.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static StreamWriter CreateText(string path);
	//
	// Summary:
	//     Decrypts a file that was encrypted by the current account using the System.IO.File.Encrypt(System.String)
	//     method.
	//
	// Parameters:
	//   path:
	//     A path that describes a file to decrypt.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     The path parameter is a zero-length string, contains only white space, or
	//     contains one or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     The path parameter is null.
	//
	//   System.IO.DriveNotFoundException:
	//     An invalid drive was specified.
	//
	//   System.IO.FileNotFoundException:
	//     The file described by the path parameter could not be found.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file. For example, the encrypted
	//     file is already open. -or- This operation is not supported on the current
	//     platform.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.PlatformNotSupportedException:
	//     The current operating system is not Microsoft Windows NT or later.
	//
	//   System.NotSupportedException:
	//     The file system is not NTFS.
	//
	//   System.UnauthorizedAccessException:
	//     The path parameter specified a file that is read-only.  -or- This operation
	//     is not supported on the current platform.  -or- The path parameter specified
	//     a directory.  -or- The caller does not have the required permission.
	public static void Decrypt(string path);
	//
	// Summary:
	//     Deletes the specified file. An exception is not thrown if the specified file
	//     does not exist.
	//
	// Parameters:
	//   path:
	//     The name of the file to be deleted.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     The specified file is in use.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path is a directory.
	//      -or- path specified a read-only file.
	public static void Delete(string path);
	//
	// Summary:
	//     Encrypts a file so that only the account used to encrypt the file can decrypt
	//     it.
	//
	// Parameters:
	//   path:
	//     A path that describes a file to encrypt.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     The path parameter is a zero-length string, contains only white space, or
	//     contains one or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     The path parameter is null.
	//
	//   System.IO.DriveNotFoundException:
	//     An invalid drive was specified.
	//
	//   System.IO.FileNotFoundException:
	//     The file described by the path parameter could not be found.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.  -or- This operation is not
	//     supported on the current platform.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.PlatformNotSupportedException:
	//     The current operating system is not Microsoft Windows NT or later.
	//
	//   System.NotSupportedException:
	//     The file system is not NTFS.
	//
	//   System.UnauthorizedAccessException:
	//     The path parameter specified a file that is read-only.  -or- This operation
	//     is not supported on the current platform.  -or- The path parameter specified
	//     a directory.  -or- The caller does not have the required permission.
	public static void Encrypt(string path);
	//
	// Summary:
	//     Determines whether the specified file exists.
	//
	// Parameters:
	//   path:
	//     The file to check.
	//
	// Returns:
	//     true if the caller has the required permissions and path contains the name
	//     of an existing file; otherwise, false. This method also returns false if
	//     path is null, an invalid path, or a zero-length string. If the caller does
	//     not have sufficient permissions to read the specified file, no exception
	//     is thrown and the method returns false regardless of the existence of path.
	public static bool Exists(string path);
	//
	// Summary:
	//     Gets a System.Security.AccessControl.FileSecurity object that encapsulates
	//     the access control list (ACL) entries for a specified file.
	//
	// Parameters:
	//   path:
	//     The path to a file containing a System.Security.AccessControl.FileSecurity
	//     object that describes the file's access control list (ACL) information.
	//
	// Returns:
	//     A System.Security.AccessControl.FileSecurity object that encapsulates the
	//     access control rules for the file described by the path parameter.
	//
	// Exceptions:
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.Runtime.InteropServices.SEHException:
	//     The path parameter is null.
	//
	//   System.SystemException:
	//     The file could not be found.
	//
	//   System.UnauthorizedAccessException:
	//     The path parameter specified a file that is read-only.  -or- This operation
	//     is not supported on the current platform.  -or- The path parameter specified
	//     a directory.  -or- The caller does not have the required permission.
	public static FileSecurity GetAccessControl(string path);
	//
	// Summary:
	//     Gets a System.Security.AccessControl.FileSecurity object that encapsulates
	//     the specified type of access control list (ACL) entries for a particular
	//     file.
	//
	// Parameters:
	//   path:
	//     The path to a file containing a System.Security.AccessControl.FileSecurity
	//     object that describes the file's access control list (ACL) information.
	//
	//   includeSections:
	//     One of the System.Security.AccessControl.AccessControlSections values that
	//     specifies the type of access control list (ACL) information to receive.
	//
	// Returns:
	//     A System.Security.AccessControl.FileSecurity object that encapsulates the
	//     access control rules for the file described by the path parameter.
	//
	// Exceptions:
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.Runtime.InteropServices.SEHException:
	//     The path parameter is null.
	//
	//   System.SystemException:
	//     The file could not be found.
	//
	//   System.UnauthorizedAccessException:
	//     The path parameter specified a file that is read-only.  -or- This operation
	//     is not supported on the current platform.  -or- The path parameter specified
	//     a directory.  -or- The caller does not have the required permission.
	public static FileSecurity GetAccessControl(string path, AccessControlSections includeSections);
	//
	// Summary:
	//     Gets the System.IO.FileAttributes of the file on the path.
	//
	// Parameters:
	//   path:
	//     The path to the file.
	//
	// Returns:
	//     The System.IO.FileAttributes of the file on the path.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is empty, contains only white spaces, or contains invalid characters.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.IO.FileNotFoundException:
	//     path represents a file and is invalid, such as being on an unmapped drive,
	//     or the file cannot be found.
	//
	//   System.IO.DirectoryNotFoundException:
	//     path represents a directory and is invalid, such as being on an unmapped
	//     drive, or the directory cannot be found.
	public static FileAttributes GetAttributes(string path);
	//
	// Summary:
	//     Returns the creation date and time of the specified file or directory.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain creation date and time information.
	//
	// Returns:
	//     A System.DateTime structure set to the creation date and time for the specified
	//     file or directory. This value is expressed in local time.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static DateTime GetCreationTime(string path);
	//
	// Summary:
	//     Returns the creation date and time, in coordinated universal time (UTC),
	//     of the specified file or directory.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain creation date and time information.
	//
	// Returns:
	//     A System.DateTime structure set to the creation date and time for the specified
	//     file or directory. This value is expressed in UTC time.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static DateTime GetCreationTimeUtc(string path);
	//
	// Summary:
	//     Returns the date and time the specified file or directory was last accessed.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain access date and time information.
	//
	// Returns:
	//     A System.DateTime structure set to the date and time that the specified file
	//     or directory was last accessed. This value is expressed in local time.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static DateTime GetLastAccessTime(string path);
	//
	// Summary:
	//     Returns the date and time, in coordinated universal time (UTC), that the
	//     specified file or directory was last accessed.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain access date and time information.
	//
	// Returns:
	//     A System.DateTime structure set to the date and time that the specified file
	//     or directory was last accessed. This value is expressed in UTC time.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static DateTime GetLastAccessTimeUtc(string path);
	//
	// Summary:
	//     Returns the date and time the specified file or directory was last written
	//     to.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain write date and time information.
	//
	// Returns:
	//     A System.DateTime structure set to the date and time that the specified file
	//     or directory was last written to. This value is expressed in local time.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static DateTime GetLastWriteTime(string path);
	//
	// Summary:
	//     Returns the date and time, in coordinated universal time (UTC), that the
	//     specified file or directory was last written to.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain write date and time information.
	//
	// Returns:
	//     A System.DateTime structure set to the date and time that the specified file
	//     or directory was last written to. This value is expressed in UTC time.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static DateTime GetLastWriteTimeUtc(string path);
	//
	// Summary:
	//     Moves a specified file to a new location, providing the option to specify
	//     a new file name.
	//
	// Parameters:
	//   sourceFileName:
	//     The name of the file to move.
	//
	//   destFileName:
	//     The new path for the file.
	//
	// Exceptions:
	//   System.IO.IOException:
	//     The destination file already exists.
	//
	//   System.ArgumentNullException:
	//     sourceFileName or destFileName is null.
	//
	//   System.ArgumentException:
	//     sourceFileName or destFileName is a zero-length string, contains only white
	//     space, or contains invalid characters as defined in System.IO.Path.InvalidPathChars.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     sourceFileName was not found.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The path specified in sourceFileName or destFileName is invalid, (for example,
	//     it is on an unmapped drive).
	//
	//   System.NotSupportedException:
	//     sourceFileName or destFileName is in an invalid format.
	public static void Move(string sourceFileName, string destFileName);
	//
	// Summary:
	//     Opens a System.IO.FileStream on the specified path with read/write access.
	//
	// Parameters:
	//   path:
	//     The file to open.
	//
	//   mode:
	//     A System.IO.FileMode value that specifies whether a file is created if one
	//     does not exist, and determines whether the contents of existing files are
	//     retained or overwritten.
	//
	// Returns:
	//     A System.IO.FileStream opened in the specified mode and path, with read/write
	//     access and not shared.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.ArgumentOutOfRangeException:
	//     mode specified an invalid value.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream Open(string path, FileMode mode);
	//
	// Summary:
	//     Opens a System.IO.FileStream on the specified path, with the specified mode
	//     and access.
	//
	// Parameters:
	//   path:
	//     The file to open.
	//
	//   mode:
	//     A System.IO.FileMode value that specifies whether a file is created if one
	//     does not exist, and determines whether the contents of existing files are
	//     retained or overwritten.
	//
	//   access:
	//     A System.IO.FileAccess value that specifies the operations that can be performed
	//     on the file.
	//
	// Returns:
	//     An unshared System.IO.FileStream that provides access to the specified file,
	//     with the specified mode and access.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//      -or- access specified Read and mode specified Create, CreateNew, Truncate,
	//     or Append.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only and access is not Read.  -or- path
	//     specified a directory.  -or- The caller does not have the required permission.
	//
	//   System.ArgumentOutOfRangeException:
	//     mode or access specified an invalid value.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream Open(string path, FileMode mode, FileAccess access);
	//
	// Summary:
	//     Opens a System.IO.FileStream on the specified path, having the specified
	//     mode with read, write, or read/write access and the specified sharing option.
	//
	// Parameters:
	//   path:
	//     The file to open.
	//
	//   mode:
	//     A System.IO.FileMode value that specifies whether a file is created if one
	//     does not exist, and determines whether the contents of existing files are
	//     retained or overwritten.
	//
	//   access:
	//     A System.IO.FileAccess value that specifies the operations that can be performed
	//     on the file.
	//
	//   share:
	//     A System.IO.FileShare value specifying the type of access other threads have
	//     to the file.
	//
	// Returns:
	//     A System.IO.FileStream on the specified path, having the specified mode with
	//     read, write, or read/write access and the specified sharing option.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//      -or- access specified Read and mode specified Create, CreateNew, Truncate,
	//     or Append.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only and access is not Read.  -or- path
	//     specified a directory.  -or- The caller does not have the required permission.
	//
	//   System.ArgumentOutOfRangeException:
	//     mode, access, or share specified an invalid value.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream Open(string path, FileMode mode, FileAccess access, FileShare share);
	//
	// Summary:
	//     Opens an existing file for reading.
	//
	// Parameters:
	//   path:
	//     The file to be opened for reading.
	//
	// Returns:
	//     A read-only System.IO.FileStream on the specified path.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.UnauthorizedAccessException:
	//     path specified a directory.  -or- The caller does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream OpenRead(string path);
	//
	// Summary:
	//     Opens an existing UTF-8 encoded text file for reading.
	//
	// Parameters:
	//   path:
	//     The file to be opened for reading.
	//
	// Returns:
	//     A System.IO.StreamReader on the specified path.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static StreamReader OpenText(string path);
	//
	// Summary:
	//     Opens an existing file for writing.
	//
	// Parameters:
	//   path:
	//     The file to be opened for writing.
	//
	// Returns:
	//     An unshared System.IO.FileStream object on the specified path with System.IO.FileAccess.Write
	//     access.
	//
	// Exceptions:
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.  -or- path specified a
	//     read-only file or directory.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static FileStream OpenWrite(string path);
	//
	// Summary:
	//     Opens a binary file, reads the contents of the file into a byte array, and
	//     then closes the file.
	//
	// Parameters:
	//   path:
	//     The file to open for reading.
	//
	// Returns:
	//     A byte array containing the contents of the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     This operation is not supported on the current platform.  -or- path specified
	//     a directory.  -or- The caller does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static byte[] ReadAllBytes(string path);
	//
	// Summary:
	//     Opens a text file, reads all lines of the file, and then closes the file.
	//
	// Parameters:
	//   path:
	//     The file to open for reading.
	//
	// Returns:
	//     A string array containing all lines of the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static string[] ReadAllLines(string path);
	//
	// Summary:
	//     Opens a file, reads all lines of the file with the specified encoding, and
	//     then closes the file.
	//
	// Parameters:
	//   path:
	//     The file to open for reading.
	//
	//   encoding:
	//     The encoding applied to the contents of the file.
	//
	// Returns:
	//     A string array containing all lines of the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static string[] ReadAllLines(string path, Encoding encoding);
	//
	// Summary:
	//     Opens a text file, reads all lines of the file, and then closes the file.
	//
	// Parameters:
	//   path:
	//     The file to open for reading.
	//
	// Returns:
	//     A string containing all lines of the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static string ReadAllText(string path);
	//
	// Summary:
	//     Opens a file, reads all lines of the file with the specified encoding, and
	//     then closes the file.
	//
	// Parameters:
	//   path:
	//     The file to open for reading.
	//
	//   encoding:
	//     The encoding applied to the contents of the file.
	//
	// Returns:
	//     A string containing all lines of the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static string ReadAllText(string path, Encoding encoding);
	//
	// Summary:
	//     Replaces the contents of a specified file with the contents of another file,
	//     deleting the original file, and creating a backup of the replaced file.
	//
	// Parameters:
	//   sourceFileName:
	//     The name of a file that replaces the file specified by destinationFileName.
	//
	//   destinationFileName:
	//     The name of the file being replaced.
	//
	//   destinationBackupFileName:
	//     The name of the backup file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     The path described by the destinationFileName parameter was not of a legal
	//     form.  -or- The path described by the destinationBackupFileName parameter
	//     was not of a legal form.
	//
	//   System.ArgumentNullException:
	//     The destinationFileName parameter is null.
	//
	//   System.IO.DriveNotFoundException:
	//     An invalid drive was specified.
	//
	//   System.IO.FileNotFoundException:
	//     The file described by the current System.IO.FileInfo object could not be
	//     found.  -or- The file described by the destinationBackupFileName parameter
	//     could not be found.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.  - or - The sourceFileName
	//     and destinationFileName parameters specify the same file.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.PlatformNotSupportedException:
	//     The operating system is Windows 98 Second Edition or earlier and the files
	//     system is not NTFS.
	//
	//   System.UnauthorizedAccessException:
	//     The sourceFileName or destinationFileName parameter specifies a file that
	//     is read-only.  -or- This operation is not supported on the current platform.
	//      -or- Source or destination parameters specify a directory instead of a file.
	//      -or- The caller does not have the required permission.
	public static void Replace(string sourceFileName, string destinationFileName, string destinationBackupFileName);
	//
	// Summary:
	//     Replaces the contents of a specified file with the contents of another file,
	//     deleting the original file, and creating a backup of the replaced file and
	//     optionally ignores merge errors.
	//
	// Parameters:
	//   sourceFileName:
	//     The name of a file that replaces the file specified by destinationFileName.
	//
	//   destinationFileName:
	//     The name of the file being replaced.
	//
	//   destinationBackupFileName:
	//     The name of the backup file.
	//
	//   ignoreMetadataErrors:
	//     true to ignore merge errors (such as attributes and access control lists
	//     (ACLs)) from the replaced file to the replacement file; otherwise, false.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     The path described by the destinationFileName parameter was not of a legal
	//     form.  -or- The path described by the destinationBackupFileName parameter
	//     was not of a legal form.
	//
	//   System.ArgumentNullException:
	//     The destinationFileName parameter is null.
	//
	//   System.IO.DriveNotFoundException:
	//     An invalid drive was specified.
	//
	//   System.IO.FileNotFoundException:
	//     The file described by the current System.IO.FileInfo object could not be
	//     found.  -or- The file described by the destinationBackupFileName parameter
	//     could not be found.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.  - or - The sourceFileName
	//     and destinationFileName parameters specify the same file.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.PlatformNotSupportedException:
	//     The operating system is Windows 98 Second Edition or earlier and the files
	//     system is not NTFS.
	//
	//   System.UnauthorizedAccessException:
	//     The sourceFileName or destinationFileName parameter specifies a file that
	//     is read-only.  -or- This operation is not supported on the current platform.
	//      -or- Source or destination parameters specify a directory instead of a file.
	//      -or- The caller does not have the required permission.
	public static void Replace(string sourceFileName, string destinationFileName, string destinationBackupFileName, bool ignoreMetadataErrors);
	//
	// Summary:
	//     Applies access control list (ACL) entries described by a System.Security.AccessControl.FileSecurity
	//     object to the specified file.
	//
	// Parameters:
	//   path:
	//     A file to add or remove access control list (ACL) entries from.
	//
	//   fileSecurity:
	//     A System.Security.AccessControl.FileSecurity object that describes an ACL
	//     entry to apply to the file described by the path parameter.
	//
	// Exceptions:
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.Runtime.InteropServices.SEHException:
	//     The path parameter is null.
	//
	//   System.SystemException:
	//     The file could not be found.
	//
	//   System.UnauthorizedAccessException:
	//     The path parameter specified a file that is read-only.  -or- This operation
	//     is not supported on the current platform.  -or- The path parameter specified
	//     a directory.  -or- The caller does not have the required permission.
	//
	//   System.ArgumentNullException:
	//     The fileSecurity parameter is null.
	public static void SetAccessControl(string path, FileSecurity fileSecurity);
	//
	// Summary:
	//     Sets the specified System.IO.FileAttributes of the file on the specified
	//     path.
	//
	// Parameters:
	//   path:
	//     The path to the file.
	//
	//   fileAttributes:
	//     The desired System.IO.FileAttributes, such as Hidden, ReadOnly, Normal, and
	//     Archive.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is empty, contains only white spaces, contains invalid characters, or
	//     the file attribute is invalid.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid, (for example, it is on an unmapped drive).
	//
	//   System.IO.FileNotFoundException:
	//     The file cannot be found.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	public static void SetAttributes(string path, FileAttributes fileAttributes);
	//
	// Summary:
	//     Sets the date and time the file was created.
	//
	// Parameters:
	//   path:
	//     The file for which to set the creation date and time information.
	//
	//   creationTime:
	//     A System.DateTime containing the value to set for the creation date and time
	//     of path. This value is expressed in local time.
	//
	// Exceptions:
	//   System.IO.FileNotFoundException:
	//     The specified path was not found.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while performing the operation.
	//
	//   System.ArgumentOutOfRangeException:
	//     creationTime specifies a value outside the range of dates, times, or both
	//     permitted for this operation.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static void SetCreationTime(string path, DateTime creationTime);
	//
	// Summary:
	//     Sets the date and time, in coordinated universal time (UTC), that the file
	//     was created.
	//
	// Parameters:
	//   path:
	//     The file for which to set the creation date and time information.
	//
	//   creationTimeUtc:
	//     A System.DateTime containing the value to set for the creation date and time
	//     of path. This value is expressed in UTC time.
	//
	// Exceptions:
	//   System.IO.FileNotFoundException:
	//     The specified path was not found.
	//
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.IOException:
	//     An I/O error occurred while performing the operation.
	//
	//   System.ArgumentOutOfRangeException:
	//     creationTime specifies a value outside the range of dates, times, or both
	//     permitted for this operation.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	public static void SetCreationTimeUtc(string path, DateTime creationTimeUtc);
	//
	// Summary:
	//     Sets the date and time the specified file was last accessed.
	//
	// Parameters:
	//   path:
	//     The file for which to set the access date and time information.
	//
	//   lastAccessTime:
	//     A System.DateTime containing the value to set for the last access date and
	//     time of path. This value is expressed in local time.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.FileNotFoundException:
	//     The specified path was not found.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.ArgumentOutOfRangeException:
	//     lastAccessTime specifies a value outside the range of dates or times permitted
	//     for this operation.
	public static void SetLastAccessTime(string path, DateTime lastAccessTime);
	//
	// Summary:
	//     Sets the date and time, in coordinated universal time (UTC), that the specified
	//     file was last accessed.
	//
	// Parameters:
	//   path:
	//     The file for which to set the access date and time information.
	//
	//   lastAccessTimeUtc:
	//     A System.DateTime containing the value to set for the last access date and
	//     time of path. This value is expressed in UTC time.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.FileNotFoundException:
	//     The specified path was not found.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.ArgumentOutOfRangeException:
	//     lastAccessTimeUtc specifies a value outside the range of dates or times permitted
	//     for this operation.
	public static void SetLastAccessTimeUtc(string path, DateTime lastAccessTimeUtc);
	//
	// Summary:
	//     Sets the date and time that the specified file was last written to.
	//
	// Parameters:
	//   path:
	//     The file for which to set the date and time information.
	//
	//   lastWriteTime:
	//     A System.DateTime containing the value to set for the last write date and
	//     time of path. This value is expressed in local time.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.FileNotFoundException:
	//     The specified path was not found.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.ArgumentOutOfRangeException:
	//     lastWriteTime specifies a value outside the range of dates or times permitted
	//     for this operation.
	public static void SetLastWriteTime(string path, DateTime lastWriteTime);
	//
	// Summary:
	//     Sets the date and time, in coordinated universal time (UTC), that the specified
	//     file was last written to.
	//
	// Parameters:
	//   path:
	//     The file for which to set the date and time information.
	//
	//   lastWriteTimeUtc:
	//     A System.DateTime containing the value to set for the last write date and
	//     time of path. This value is expressed in UTC time.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.FileNotFoundException:
	//     The specified path was not found.
	//
	//   System.UnauthorizedAccessException:
	//     The caller does not have the required permission.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.ArgumentOutOfRangeException:
	//     lastWriteTimeUtc specifies a value outside the range of dates or times permitted
	//     for this operation.
	public static void SetLastWriteTimeUtc(string path, DateTime lastWriteTimeUtc);
	//
	// Summary:
	//     Creates a new file, writes the specified byte array to the file, and then
	//     closes the file. If the target file already exists, it is overwritten.
	//
	// Parameters:
	//   path:
	//     The file to write to.
	//
	//   bytes:
	//     The bytes to write to the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null or the byte array is empty.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void WriteAllBytes(string path, byte[] bytes);
	//
	// Summary:
	//     Creates a new file, write the specified string array to the file, and then
	//     closes the file. If the target file already exists, it is overwritten.
	//
	// Parameters:
	//   path:
	//     The file to write to.
	//
	//   contents:
	//     The string array to write to the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void WriteAllLines(string path, string[] contents);
	//
	// Summary:
	//     Creates a new file, writes the specified string array to the file using the
	//     specified encoding, and then closes the file. If the target file already
	//     exists, it is overwritten.
	//
	// Parameters:
	//   path:
	//     The file to write to.
	//
	//   contents:
	//     The string array to write to the file.
	//
	//   encoding:
	//     An System.Text.Encoding object that represents the character encoding applied
	//     to the string array.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null or contents string is empty.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void WriteAllLines(string path, string[] contents, Encoding encoding);
	//
	// Summary:
	//     Creates a new file, writes the specified string to the file, and then closes
	//     the file. If the target file already exists, it is overwritten.
	//
	// Parameters:
	//   path:
	//     The file to write to.
	//
	//   contents:
	//     The string to write to the file.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void WriteAllText(string path, string contents);
	//
	// Summary:
	//     Creates a new file, writes the specified string to the file using the specified
	//     encoding, and then closes the file. If the target file already exists, it
	//     is overwritten.
	//
	// Parameters:
	//   path:
	//     The file to write to.
	//
	//   contents:
	//     The string to write to the file.
	//
	//   encoding:
	//     An System.Text.Encoding object that represents the encoding to apply to the
	//     string.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more invalid characters as defined by System.IO.Path.InvalidPathChars.
	//
	//   System.ArgumentNullException:
	//     path is null or contents string is empty.
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	//
	//   System.IO.DirectoryNotFoundException:
	//     The specified path is invalid (for example, it is on an unmapped drive).
	//
	//   System.IO.IOException:
	//     An I/O error occurred while opening the file.
	//
	//   System.UnauthorizedAccessException:
	//     path specified a file that is read-only.  -or- This operation is not supported
	//     on the current platform.  -or- path specified a directory.  -or- The caller
	//     does not have the required permission.
	//
	//   System.IO.FileNotFoundException:
	//     The file specified in path was not found.
	//
	//   System.NotSupportedException:
	//     path is in an invalid format.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permission.
	public static void WriteAllText(string path, string contents, Encoding encoding);*/
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